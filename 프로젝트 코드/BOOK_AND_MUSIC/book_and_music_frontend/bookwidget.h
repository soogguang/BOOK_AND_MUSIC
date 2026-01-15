#pragma once

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QPushButton>
#include <QNetworkAccessManager>

class BookWidget : public QWidget // QT가 제공하는 QWidget클래스 상속
                                  // -> 원하는 기능을 구현하기 위해 상속받아 원하는 방향으로 확장
{
    Q_OBJECT

public:
    explicit BookWidget(const QString &title,
                        const QString &author,
                        const QString &coverUrl,
                        const QString &storySummary,
                        QWidget *parent = nullptr);

    ~BookWidget();

signals:
    void bookSelected(const QString &title,
                      const QString &storySummary,
                      const QPixmap &cover);

private:
    QLabel *coverLabel;
    QLabel *titleLabel;
    QLabel *authorLabel;
    QLabel *summaryLabel;
    QPushButton *selectButton;

    QNetworkAccessManager *imageManager;

    QString coverUrl;  // 표지 URL 저장용
    QString storySummary;
};
// 구체적인 구현 없이 선언만 해둔다 -> 헤더파일을 인터페이스를 작성해두는 부분으로 활용
