import requests
import sys

argv = sys.argv[1:] 

user = argv[0]
password = argv[1]
platform = argv[2]
fmod_version = argv[3]

fmodlink = "https://www.fmod.com/api-login"

if platform == 'linux':
    # linux
    filename = f'fmodstudioapi{fmod_version}linux.tar.gz'
    downloadlink = f'https://www.fmod.com/api-get-download-link?path=files/fmodstudio/api/Linux/&filename=fmodstudioapi{fmod_version}linux.tar.gz&user='
elif platform == 'macos':
    # OS X
    filename = f'fmodstudioapi{fmod_version}osx.dmg'
    downloadlink = f'https://www.fmod.com/api-get-download-link?path=files/fmodstudio/api/Mac/&filename=fmodstudioapi{fmod_version}mac-installer.dmg&user='
elif platform == 'windows':
    # Windows...
    filename = f'fmodstudioapi{fmod_version}win-installer.exe'
    downloadlink = f'https://www.fmod.com/api-get-download-link?path=files/fmodstudio/api/Win/&filename=fmodstudioapi{fmod_version}win-installer.exe&user='
elif platform == 'android':
    # Android...
    filename = f'fmodstudioapi{fmod_version}android.tar.gz'
    downloadlink = f'https://www.fmod.com/api-get-download-link?path=files/fmodstudio/api/Android/&filename=fmodstudioapi{fmod_version}android.tar.gz&user='
elif platform == 'ios':
    # iOS...
    filename = f'fmodstudioapi{fmod_version}ios.dmg'
    downloadlink = f'https://www.fmod.com/api-get-download-link?path=files/fmodstudio/api/iOS/&filename=fmodstudioapi{fmod_version}ios-installer.dmg&user=$1'

downloadlink += user

# First login and get a token!
response = requests.post(fmodlink, auth = (user, password)).json()
token = response["token"]

print("Received token from FMOD login API.")

# Next request a download link using the token!
response = requests.get(downloadlink, headers = {"Authorization": f"Bearer {token}"}).json()
url = response["url"]

# Download FMOD!
response = requests.get(url, allow_redirects=True)
open(filename, 'wb').write(response.content)

print("Downloading FMOD using the requested download link.")
print(response)