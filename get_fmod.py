import requests
import sys

argv = sys.argv[1:] 

user = argv[0]
password = argv[1]
platform = argv[2]

fmodlink = "https://www.fmod.com/api-login"

if platform == 'linux':
    # linux
    filename = 'fmodstudioapi20002linux.tar.gz'
    downloadlink = 'https://www.fmod.com/api-get-download-link?path=files/fmodstudio/api/Linux/&filename=fmodstudioapi20002linux.tar.gz&user='
elif platform == 'osx':
    # OS X
    filename = 'fmodstudioapi20002osx.dmg'
    downloadlink = 'https://www.fmod.com/api-get-download-link?path=files/fmodstudio/api/Mac/&filename=fmodstudioapi20002mac-installer.dmg&user='
elif platform == 'windows':
    # Windows...
    filename = 'fmodstudioapi20002win-installer.exe'
    downloadlink = 'https://www.fmod.com/api-get-download-link?path=files/fmodstudio/api/Win/&filename=fmodstudioapi20002win-installer.exe&user='
elif platform == 'android':
    # Android...
    filename = 'fmodstudioapi20002android.tar.gz'
    downloadlink = 'https://www.fmod.com/api-get-download-link?path=files/fmodstudio/api/Android/&filename=fmodstudioapi20002android.tar.gz&user='
elif platform == 'ios':
    # iOS...
    filename = 'fmodstudioapi20002ios.dmg'
    downloadlink = 'https://www.fmod.com/api-get-download-link?path=files/fmodstudio/api/iOS/&filename=fmodstudioapi20002ios-installer.dmg&user=$1'

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