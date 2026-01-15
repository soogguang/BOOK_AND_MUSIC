#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class QLineEdit;
class QPushButton;
class QVBoxLayout;
class QWidget;

class MainWindow : public QMainWindow // QT에서 제공하는 클래스를 상속받아 원하는 뱡향으로 기능을 확장시킨다.
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void onBookSelected(const QString &title,
                        const QString &storySummary,
                        const QPixmap &cover);

    void onSearchClicked();              //  검색 버튼 클릭 시 호출
    void onSearchFinished(QNetworkReply *reply); // API 응답 완료 시 호출

private:
    QLineEdit *searchEdit;
    QPushButton *searchButton;
    QWidget *scrollContent;
    QVBoxLayout *scrollLayout;

    QNetworkAccessManager *networkManager; // 네트워크 매니저 멤버
};

#endif // MAINWINDOW_H
