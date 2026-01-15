import os
import requests
from dotenv import load_dotenv

# 🔐 .env에서 환경변수 불러오기
load_dotenv()

CLIENT_ID = os.getenv("SPOTIFY_CLIENT_ID")
CLIENT_SECRET = os.getenv("SPOTIFY_CLIENT_SECRET")
CODE = os.getenv("SPOTIFY_CODE")
REDIRECT_URI = "http://127.0.0.1:8888/callback"

response = requests.post(
    "https://accounts.spotify.com/api/token",
    data={
        "grant_type": "authorization_code",
        "code": CODE,
        "redirect_uri": REDIRECT_URI,
        "client_id": CLIENT_ID,
        "client_secret": CLIENT_SECRET,
    }
)

print(response.status_code)
print(response.json())
