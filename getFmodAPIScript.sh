TOKEN=$(curl -X POST -u $1:$2 https://www.fmod.com/api-login | jq -r '.token')
URL=$(curl -H 'Accept: application/json' -H "Authorization: Bearer ${TOKEN}" https://www.fmod.com/api-get-download-link\?path\=files/fmodstudio/api/Linux/\&filename\=fmodstudioapi11012linux.tar.gz\&user\=$1 | jq -r '.url')
wget -O fmodstudioapi11012linux.tar.gz $URL
