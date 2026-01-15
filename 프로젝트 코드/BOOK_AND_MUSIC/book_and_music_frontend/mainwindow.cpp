#include "mainwindow.h"

#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QLabel>
#include <QDialog>
#include <QTimer>
#include "bookwidget.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    // 메인 위젯과 레이아웃 생성
    QWidget *central = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    // 검색창과 버튼을 담을 수평 레이아웃
    searchEdit = new QLineEdit(this);
    searchButton = new QPushButton("검색", this);

    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchLayout->addWidget(searchEdit);
    searchLayout->addWidget(searchButton);

    // 책 리스트가 담길 스크롤 영역
    scrollContent = new QWidget(this);
    scrollLayout = new QVBoxLayout(scrollContent);
    scrollContent->setLayout(scrollLayout);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(scrollContent);
    scrollArea->setWidgetResizable(true);

    // 전체 레이아웃에 추가
    mainLayout->addLayout(searchLayout);
    mainLayout->addWidget(scrollArea);

    networkManager = new QNetworkAccessManager(this);  // 🔹 1. 매니저 생성

    connect(searchButton, &QPushButton::clicked, this, &MainWindow::onSearchClicked); // 🔹 2. 검색 버튼 연결
    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onSearchFinished); // 🔹 3. 응답 처리 연결

    // central widget 설정
    setCentralWidget(central);

    resize(800, 700);  // 원하는 크기 (예: 900x700픽셀)

}

void MainWindow::onSearchClicked() {
    QString query = searchEdit->text().trimmed();
    if (query.isEmpty()) return;

    QString encodedQuery = QString::fromUtf8(QUrl::toPercentEncoding(query));
    QString url = QString("https://www.aladin.co.kr/ttb/api/ItemSearch.aspx?"
                          "ttbkey=알라딘apikey&Query=%1&QueryType=Keyword&MaxResults=10" // 원래 여기에 알라딘 api키가 들어가야 하는데 혹시 몰라서 지웠습니다!
                          "&start=1&SearchTarget=Book&Sort=SalesPoint&output=js&Version=20131101") // 원하는 기준에 맞춰 api를 불러온다!
                      .arg(encodedQuery);


    QUrl qurl(url);
    QNetworkRequest request(qurl);
    networkManager->get(request); // ✅ OK!
}


void MainWindow::onSearchFinished(QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "API Error:" << reply->errorString();
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject root = doc.object();
    QJsonArray items = root["item"].toArray();

    // 기존 위젯 제거
    QLayoutItem *item;
    while ((item = scrollLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    for (const QJsonValue &v : items) {
        QJsonObject obj = v.toObject();

        // 🔍 (1) 카테고리명 가져오기
        QString category = obj["categoryName"].toString();

        // ✅ (2) "국내도서" 또는 "외국도서"가 아닌 경우 필터링 : ebook이나 음반과 같은 중복되거나 도서가 아닌 결과를 제외함!
        if (!(category.contains("국내도서") || category.contains("외국도서"))) {
            continue;  // 스킵
        }

        QString title = obj["title"].toString();
        QString author = obj["author"].toString();
        QString imageUrl = obj["cover"].toString();
        QString isbn13 = obj["isbn13"].toString();

        // 1️⃣ 상세 줄거리 요청용 URL 생성
        QString lookupUrl = QString("https://www.aladin.co.kr/ttb/api/ItemLookUp.aspx?"
                                    "ttbkey=ttbsusuthelove1316001&itemIdType=ISBN13&ItemId=%1"
                                    "&output=js&Version=20131101").arg(isbn13);

        // 2️⃣ 비동기 요청
        QNetworkAccessManager *detailManager = new QNetworkAccessManager(this);
        QUrl url(lookupUrl);
        QNetworkRequest request(url);
        QNetworkReply *detailReply = detailManager->get(request);

        connect(detailReply, &QNetworkReply::finished, this, [=]() {
            QByteArray responseData = detailReply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(responseData);
            QJsonObject root = doc.object();
            QJsonArray items = root["item"].toArray();
            if (items.isEmpty()) {
                // qDebug() << "❌ 상세 정보 없음. 건너뜀.";
                detailReply->deleteLater();
                return;
            }
            QJsonObject detailObj = items[0].toObject();

            QString storySummary = detailObj["Story"].toString(); // 참고하고 싶은 우선순위에 따라 storySummary 내용을 바꿔준다.
            // 만약 api를 통해 불러온 결과 "Story"에 대한 내용이 비어 있다면 그 다음 호출을 통해 초기화 해준다.
            if (storySummary.isEmpty())
                storySummary = detailObj["description"].toString();
            if (storySummary.isEmpty())
                storySummary = detailObj["pubDescription"].toString();
            if (storySummary.isEmpty()) // 마지막까지 없으면 줄거리 정보 없음으로 초기화 -> chat gpt는 같이 전달해준 책 제목을 기반으로 노래를 추천해줄 것이다.
                storySummary = "(줄거리 정보 없음)";

            detailReply->deleteLater();

            BookWidget *book = new BookWidget(title, author, imageUrl, storySummary);
            scrollLayout->addWidget(book);
            connect(book, &BookWidget::bookSelected,
                    this, &MainWindow::onBookSelected);
        });

    }
}


void MainWindow::onBookSelected(const QString &title,
                                const QString &storySummary,
                                const QPixmap &cover) {
    // (1) 상태 메시지 다이얼로그
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("🎵 플레이리스트 생성 중");

    QLabel *imageLabel = new QLabel(dialog);
    imageLabel->setPixmap(cover.scaled(100, 150, Qt::KeepAspectRatio));

    QLabel *statusLabel = new QLabel("📖 \"" + title + "\"\n\n🎵 어울리는 플레이리스트를 생성 중입니다...\n\n⏱ 약 5-10초 정도 걸립니다...\n☺️ 시간이 걸려도 잠시만 기다려주세요", dialog);
    statusLabel->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->addWidget(imageLabel, 0, Qt::AlignCenter);
    layout->addWidget(statusLabel, 0, Qt::AlignCenter);
    dialog->setLayout(layout);
    dialog->resize(300, 300);
    dialog->show();

    // (2) Flask API로 요청 보내기
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url("http://127.0.0.1:5050/generate_playlist");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["book_title"] = title;
    json["book_story"] = storySummary;
    QNetworkReply *reply = manager->post(request, QJsonDocument(json).toJson());

    // (3) 요청 응답 후 상태 업데이트
    connect(reply, &QNetworkReply::finished, this, [=]() {
        QByteArray responseData = reply->readAll();
        qDebug() << "📦 응답 내용:" << QString(responseData);

        if (reply->error() == QNetworkReply::NoError) {
            // JSON 파싱
            QJsonDocument doc = QJsonDocument::fromJson(responseData);
            QString playlistUrl = doc.object()["playlist_url"].toString();

            if (!playlistUrl.isEmpty()) {
                QString clickableLink = QString(
                                            "✅ 플레이리스트가 완성되었습니다!<br><br>"
                                            "<a href=\"%1\">🔗 Spotify에서 열기</a>" // 바로 플레이리스트로 이동가능 하도록 링크!
                                            ).arg(playlistUrl);

                statusLabel->setTextFormat(Qt::RichText);
                statusLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
                statusLabel->setOpenExternalLinks(true);
                statusLabel->setText(clickableLink);
            } else {
                statusLabel->setText("✅ 플레이리스트가 완성되었습니다!");
            }
        } else {
            statusLabel->setText("❌ 오류: " + reply->errorString());
        }


        // 코딩 중 디버깅을 위해 작성했던 부분
        // qDebug() << "HTTP 상태 코드:"
        //          << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        // qDebug() << "에러 설명:" << reply->errorString();

        reply->deleteLater();
    });
}




