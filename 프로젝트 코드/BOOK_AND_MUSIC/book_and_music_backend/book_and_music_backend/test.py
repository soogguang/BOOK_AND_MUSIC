# 토큰 확인용 코드!! 프로젝트할 때 확인용으로 사용했던 코드입니다!

from dotenv import load_dotenv
import os, requests

load_dotenv()

def refresh_access_token():
    print("🔄 Refreshing token")
    response = requests.post(
        "https://accounts.spotify.com/api/token",
        data={
            "grant_type": "refresh_token",
            "refresh_token": os.getenv("SPOTIFY_REFRESH_TOKEN"),
            "client_id": os.getenv("SPOTIFY_CLIENT_ID"),
            "client_secret": os.getenv("SPOTIFY_CLIENT_SECRET"),
        }
    )
    print("📦 응답:", response.status_code, response.text)
    if response.status_code == 200:
        access_token = response.json().get("access_token")
        print("✅ 새 토큰:", access_token)
        return access_token
    else:
        print("❌ 실패:", response.text)

access_token = refresh_access_token()

headers = {"Authorization": f"Bearer {access_token}"}
r = requests.get("https://api.spotify.com/v1/me", headers=headers)
print("👤 사용자 정보 요청 결과:", r.status_code, r.text)
