#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QPixmap>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "bookwidget.h"   // 헤더파일 불러옴
BookWidget::BookWidget(const QString &title,  // 헤더파일에서 선언만 해뒀던 클래스의 생성자를 자세히 구현해준다.
                       const QString &author,
                       const QString &coverUrl,
                       const QString &storySummary,
                       QWidget *parent)
    :  QWidget(parent), coverUrl(coverUrl), storySummary(storySummary)
{
    coverLabel = new QLabel(this);
    coverLabel->setFixedSize(100, 150);
    coverLabel->setAlignment(Qt::AlignCenter);

    titleLabel = new QLabel(title, this); // 책 제목 표시
    authorLabel = new QLabel(author, this); // 저자 표시
    summaryLabel = new QLabel(storySummary, this);  // 사용자 요약 표시
    summaryLabel->setWordWrap(true);
    summaryLabel->setFixedHeight(55); // 크기 지정

    selectButton = new QPushButton("선택", this);
    selectButton->setFixedSize(50, 30);

    connect(selectButton, &QPushButton::clicked, this, [=]() {
        emit bookSelected(titleLabel->text(), storySummary,
                          coverLabel->pixmap(Qt::ReturnByValue));
    });


    QVBoxLayout *infoLayout = new QVBoxLayout;
    infoLayout->addWidget(titleLabel);
    infoLayout->addWidget(authorLabel);
    infoLayout->addWidget(summaryLabel);
    infoLayout->addStretch();
    infoLayout->addWidget(selectButton, 0, Qt::AlignLeft);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(coverLabel);
    mainLayout->addLayout(infoLayout);
    setLayout(mainLayout);

    if (!coverUrl.isEmpty()) {
        imageManager = new QNetworkAccessManager(this);
        connect(imageManager, &QNetworkAccessManager::finished, this, [=](QNetworkReply *reply) {
            if (reply->error() == QNetworkReply::NoError) {
                QPixmap pix;
                pix.loadFromData(reply->readAll());

                coverLabel->setPixmap(
                    pix.scaled(100, 150, Qt::KeepAspectRatio)
                    );
            }
            reply->deleteLater();
        });

        QUrl url(coverUrl);
        imageManager->get(QNetworkRequest(url));
    }

}

BookWidget::~BookWidget() {}
