"""Runs an exported web build of the tests in a browser and exits with the code ExportTestRunner.gd prints.

Usage: python run_in_browser.py <export directory> <browser executable> [--headless] [--timeout <seconds>] [--demo]

Serves the export with the cross-origin isolation headers threaded exports need, injects a script into index.html that
forwards the page's console to this process and simulates key presses, and launches the browser (Chromium-based or
Firefox) on a throwaway profile that lets audio start without a real user gesture.

With --demo, serves the export unchanged for someone to play it, until the browser window is closed.
"""
import argparse
import functools
import http.server
import os
import re
import shutil
import subprocess
import sys
import tempfile
import threading
import time
from pathlib import Path

EXIT_CODE_LINE = re.compile(r"Export tests exit code: (\d+)")
# Godot prints it, then stops, when the browser lacks what it needs (WebGL2, for one).
MISSING_FEATURES_LINE = "required to run Godot projects on the Web are missing"

CONSOLE_FORWARDER = """<script>
(() => {
    let lines = [];
    let sending = Promise.resolve();
    const flush = () => {
        if (lines.length === 0) return;
        const body = lines.join('\\n');
        lines = [];
        sending = sending.then(() => fetch('/log', { method: 'POST', body, keepalive: true }).catch(() => {}));
    };
    for (const level of ['log', 'info', 'warn', 'error']) {
        const original = console[level].bind(console);
        console[level] = (...args) => {
            lines.push(args.join(' '));
            original(...args);
        };
    }
    setInterval(flush, 200);
    // FMOD starts its audio on a user gesture, which the browser profile lets a script simulate.
    setInterval(() => window.dispatchEvent(new KeyboardEvent('keydown')), 1000);
    window.addEventListener('error', (event) => { lines.push(`Uncaught ${event.message}`); flush(); });
})();
</script>"""

FIREFOX_PREFERENCES = {
    "media.autoplay.default": 0,
    "media.autoplay.blocking_policy": 0,
    "browser.shell.checkDefaultBrowser": False,
    "browser.aboutwelcome.enabled": False,
    "datareporting.policy.dataSubmissionEnabled": False,
    "toolkit.telemetry.reportingpolicy.firstRun": False,
}


class Handler(http.server.SimpleHTTPRequestHandler):
    exit_code = None
    finished = threading.Event()
    demo = False

    def end_headers(self):
        self.send_header("Cross-Origin-Opener-Policy", "same-origin")
        self.send_header("Cross-Origin-Embedder-Policy", "require-corp")
        self.send_header("Cache-Control", "no-store")
        super().end_headers()

    def do_GET(self):
        if Handler.demo or self.path.split("?")[0] not in ("/", "/index.html"):
            return super().do_GET()
        page = (Path(self.directory) / "index.html").read_text(encoding="utf-8").replace("<head>", "<head>" + CONSOLE_FORWARDER, 1)
        body = page.encode("utf-8")
        self.send_response(200)
        self.send_header("Content-Type", "text/html; charset=utf-8")
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def do_POST(self):
        text = self.rfile.read(int(self.headers["Content-Length"])).decode("utf-8", errors="replace")
        self.send_response(204)
        self.end_headers()
        for line in text.splitlines():
            print(line, flush=True)
            match = EXIT_CODE_LINE.search(line)
            if match:
                Handler.exit_code = int(match.group(1))
                Handler.finished.set()
            elif MISSING_FEATURES_LINE in line:
                Handler.finished.set()

    def log_message(self, format, *args):
        pass


def browser_command(browser: str, url: str, profile: Path, headless: bool) -> list[str]:
    if "firefox" in Path(browser).name.lower():
        (profile / "user.js").write_text(
            "".join(f'user_pref("{key}", {str(value).lower() if isinstance(value, bool) else value});\n' for key, value in FIREFOX_PREFERENCES.items())
        )
        return [browser, "-no-remote", "-wait-for-browser", "-profile", str(profile), *(["-headless"] if headless else []), url]
    return [
        browser,
        f"--user-data-dir={profile}",
        "--no-first-run",
        "--no-default-browser-check",
        "--autoplay-policy=no-user-gesture-required",
        # Without a GPU, as on CI runners, Chrome blocks WebGL2 unless it may fall back to software rendering.
        "--ignore-gpu-blocklist",
        "--enable-unsafe-swiftshader",
        *(["--headless=new"] if headless else []),
        url,
    ]


def profile_processes(profile: Path, stop: bool) -> bool:
    """Tells whether processes started on the throwaway profile are running, stopping them when asked: browsers hand
    their windows over to processes outside the launched one's tree."""
    if os.name == "nt":
        action = "ForEach-Object { Stop-Process -Id $_.ProcessId -Force -ErrorAction SilentlyContinue }" if stop else "Measure-Object | ForEach-Object { $_.Count }"
        query = f"Get-CimInstance Win32_Process | Where-Object {{ $_.ProcessId -ne $PID -and $_.CommandLine -like '*{profile.name}*' }} | {action}"
        output = subprocess.run(["powershell", "-NoProfile", "-Command", query], capture_output=True, text=True).stdout
    else:
        output = subprocess.run(["pkill" if stop else "pgrep", "-f", profile.name], capture_output=True, text=True).stdout
    return output.strip() not in ("", "0")


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("export_directory")
    parser.add_argument("browser")
    parser.add_argument("--headless", action="store_true")
    parser.add_argument("--timeout", type=float, default=600)
    parser.add_argument("--demo", action="store_true")
    arguments = parser.parse_args()
    Handler.demo = arguments.demo

    server = http.server.ThreadingHTTPServer(("127.0.0.1", 0), functools.partial(Handler, directory=arguments.export_directory))
    threading.Thread(target=server.serve_forever, daemon=True).start()
    url = f"http://127.0.0.1:{server.server_address[1]}/index.html"

    profile = Path(tempfile.mkdtemp(prefix="fmod-web-tests-"))
    browser = subprocess.Popen(browser_command(arguments.browser, url, profile, arguments.headless))
    try:
        if arguments.demo:
            print(f"Serving {url} until the browser window is closed.", flush=True)
            time.sleep(10)
            while profile_processes(profile, stop=False):
                time.sleep(2)
        elif not Handler.finished.wait(arguments.timeout):
            print(f"ERROR: no test result after {arguments.timeout:.0f} seconds.", flush=True)
    finally:
        profile_processes(profile, stop=True)
        browser.kill()
        browser.wait()
        server.shutdown()
        shutil.rmtree(profile, ignore_errors=True)
    sys.exit(0 if arguments.demo else 1 if Handler.exit_code is None else Handler.exit_code)


if __name__ == "__main__":
    main()
