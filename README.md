# 📚🎵 BOOK & MUSIC  
AI-based Book Mood Playlist Generator

사용자가 선택한 책의 분위기와 감정을 분석하여  
책에 어울리는 음악 플레이리스트를 자동으로 생성하고  
실제 Spotify 플레이리스트로 제공하는 데스크톱 애플리케이션입니다

---

## 1. Project Motivation

독서 중 음악을 함께 듣는 경우는 많지만,  
기존 독서용 플레이리스트는 대부분 단조롭고 비슷한 분위기의 음악으로 구성되어 있습니다  

하지만 연구 결과에 따르면  
텍스트의 감정과 음악의 감정이 일치할수록 독자의 몰입도와 선호도가 높아집니다  

이 프로젝트는  
**“지금 읽고 있는 책의 분위기에 정말 어울리는 음악을 자동으로 추천할 수 없을까?”**  
라는 문제의식에서 출발했습니다  

📌 Reference  
- https://pmc.ncbi.nlm.nih.gov/articles/PMC9836297/

---

## 2. System Overview

본 프로젝트는  
**Qt 기반 데스크톱 애플리케이션 + Flask 서버 + 외부 API 연동** 구조로 설계되었습니다  

### Workflow
1. 사용자가 Qt UI에서 책 선택  
2. 책 정보(줄거리, 분위기)를 Flask 서버로 전달  
3. ChatGPT가 책의 감정·장르·분위기를 분석  
4. Spotify API를 통해 어울리는 음악 플레이리스트 생성  
5. 생성된 Spotify 플레이리스트 URL을 Qt 앱으로 반환  

---

## 3. Architecture
```text
[ Qt Desktop App (C++) ]   
          ↓    
[ Flask Server (Python) ]    
          ↓    
[ ChatGPT API ] ── Book mood analysis    
          ↓    
[ Spotify API ] ── Playlist generation     
```
---

## 4. Key Features

### 📖 Book-based Mood Analysis
- 단순 장르 분류가 아닌 책의 분위기와 감정을 언어적으로 해석
- ChatGPT의 자연어 이해 능력을 활용한 감성 분석

### 🎵 Automatic Playlist Generation
- 분석 결과를 기반으로 음악 리스트 자동 생성
- 실제 Spotify 플레이리스트로 생성되어 즉시 감상 가능

### 🖥 Desktop UI (Qt)
- Qt Widget 기반 데스크톱 UI
- 책 검색, 선택, 플레이리스트 생성 과정 시각화
- Spotify 링크를 통해 바로 음악 재생 가능

---

## 5. Implementation Details

### Frontend (C++ / Qt)
- Qt Widget 기반 UI 구성
- `BookWidget`, `MainWindow` 등 역할별 클래스 분리
- Header 파일에는 인터페이스 선언
- cpp 파일에서 기능 구현 → 객체지향 구조 유지
- `main.cpp`에서는 전체 흐름만 관리

### Backend (Python / Flask)
- Qt 앱과 통신하는 REST API 서버
- `/generate_playlist` 엔드포인트에서 핵심 로직 수행
- ChatGPT API로 책 분위기 분석
- Spotify API를 활용해 플레이리스트 생성 후 URL 반환

---

## 6. Tech Stack

### Frontend
- C++
- Qt (Qt Widgets)

### Backend
- Python
- Flask

### APIs
- ChatGPT API
- Spotify Web API
- Aladin Open API (Book information)

### Tools
- Qt Creator
- Visual Studio Code

---

## 7. User Interface

실제 실행 화면 예시는 아래와 같이 구성됩니다  

![UI Screenshot](./music_book_window.png)

## 8. Expected Effects
책 분위기에 맞는 음악 추천을 통한 독서 몰입도 향상   
AI 기반 개인화 콘텐츠 추천 구조 학습    
실제 음악 플랫폼과 연동된 실용적인 서비스 구현    
플레이리스트 길이, 음악 유형 등 확장 가능한 구조    

## 9. Author
GitHub: soogguang
Project Type: Personal / Academic Project
Focus: Desktop Application, API Integration, AI-based Recommendation
