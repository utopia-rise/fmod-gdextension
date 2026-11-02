"""Downloads the FMOD Studio API of one platform and installs it into libs/<sdk platform>/, where SConstruct reads it.

Usage: python get_fmod.py <FMOD account> <FMOD password> <windows|linux|macos|ios|android> <version, e.g. 20400>

Unpacking needs 7-Zip for the Windows installer and macOS (hdiutil) for the macOS and iOS disk images.
"""
import re
import shutil
import subprocess
import sys
import tarfile
import tempfile
from pathlib import Path

import requests

LIBS_DIR = Path(__file__).resolve().parent / "libs"

# platform: (FMOD download folder, file name suffix in the download link, saved file suffix, folder under libs/)
PLATFORMS = {
    "windows": ("Windows", "win-installer.exe", "win-installer.exe", "windows"),
    "linux": ("Linux", "linux.tar.gz", "linux.tar.gz", "linux"),
    "macos": ("Mac", "mac-installer.dmg", "osx.dmg", "osx"),
    "ios": ("iOS", "ios-installer.dmg", "ios.dmg", "ios"),
    "android": ("Android", "android.tar.gz", "android.tar.gz", "android"),
}


def download(user: str, password: str, platform: str, version: str, archive: Path) -> None:
    folder, link_suffix, _, _ = PLATFORMS[platform]

    login = requests.post("https://www.fmod.com/api-login", auth=(user, password))
    login.raise_for_status()
    token = login.json()["token"]

    link = requests.get(
        "https://www.fmod.com/api-get-download-link",
        params={"path": f"files/fmodstudio/api/{folder}/", "filename": f"fmodstudioapi{version}{link_suffix}", "user": user},
        headers={"Authorization": f"Bearer {token}"},
    )
    link.raise_for_status()

    print(f"Downloading {archive.name}...")
    with requests.get(link.json()["url"], stream=True) as response:
        response.raise_for_status()
        with archive.open("wb") as file:
            for chunk in response.iter_content(chunk_size=1 << 20):
                file.write(chunk)


def extract_api(platform: str, version: str, archive: Path, work_dir: Path) -> Path:
    if platform == "windows":
        seven_zip = shutil.which("7z") or shutil.which("7za") or next(
            (path for path in ("C:/Program Files/7-Zip/7z.exe", "C:/Program Files (x86)/7-Zip/7z.exe") if Path(path).exists()), None
        )
        if seven_zip is None:
            sys.exit(f"7-Zip is needed to unpack {archive.name}: install it, or run the installer and copy its api folder to libs/windows.")
        subprocess.run([seven_zip, "x", "-y", f"-o{work_dir}", str(archive)], check=True, stdout=subprocess.DEVNULL)
        return work_dir / "api"
    if platform in ("linux", "android"):
        with tarfile.open(archive) as tar:
            tar.extractall(work_dir, filter="data")
        return work_dir / f"fmodstudioapi{version}{platform}" / "api"

    mount_point = work_dir / "mount"
    subprocess.run(["hdiutil", "attach", "-nobrowse", "-mountpoint", str(mount_point), str(archive)], check=True)
    try:
        shutil.copytree(mount_point / "FMOD Programmers API" / "api", work_dir / "api", symlinks=True)
    finally:
        subprocess.run(["hdiutil", "detach", str(mount_point)], check=True)
    return work_dir / "api"


def installed_version(sdk_dir: Path) -> str:
    header = (sdk_dir / "core" / "inc" / "fmod_common.h").read_text(errors="ignore")
    return f"{int(re.search(r'#define FMOD_VERSION\s+0x([0-9a-fA-F]{8})', header).group(1), 16):x}".zfill(5)


def main() -> None:
    if len(sys.argv) != 5 or sys.argv[3] not in PLATFORMS:
        sys.exit(__doc__)
    user, password, platform, version = sys.argv[1:]
    sdk_dir = LIBS_DIR / PLATFORMS[platform][3]
    LIBS_DIR.mkdir(exist_ok=True)

    with tempfile.TemporaryDirectory(dir=LIBS_DIR) as temp:
        work_dir = Path(temp)
        archive = work_dir / f"fmodstudioapi{version}{PLATFORMS[platform][2]}"
        download(user, password, platform, version, archive)
        api = extract_api(platform, version, archive, work_dir)
        if sdk_dir.exists():
            shutil.rmtree(sdk_dir)
        shutil.move(str(api), sdk_dir)

    found = installed_version(sdk_dir)
    if found != version:
        sys.exit(f"Installed {sdk_dir}, but its fmod_common.h reports version {found}, not {version}.")
    print(f"FMOD {version} for {platform} installed in {sdk_dir}.")


if __name__ == "__main__":
    main()
