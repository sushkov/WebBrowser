#ifndef WEBBROWSER_H
#define WEBBROWSER_H
//подключение всех элементов
#include <QWidget>
#include <QLineEdit>
#include <QtGui>
#include <QtCore>
#include <QtWebKit>
#include <QWebPage>
#include <QWebSettings>
#include <QPushButton>
#include <QProgressBar>
#include <QLayout>
#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QCoreApplication>
#include <QUrl>
#include <QNetworkRequest>
#include <QFile>
#include <QMenu>
#include <QEventLoop>
#include <QAction>

class TabPage;
class QProgressDialog;
class QAction;

class WebBrowser : public QWidget{
    Q_OBJECT
public:
        WebBrowser ();
private:
        QHBoxLayout* htablayout;
        QVBoxLayout* vtablayout;
        TabPage* ftab;
        QTabWidget *tabWidget;
        QAction* addAct;
        QAction* newWinAct;
        QAction* aboutAct;
        QPushButton* pcmdAdd;
        QPushButton* pcmdConfig;
        QMenu* menuconfig;
private slots:
        void closeTab(int index); // закрытие вкладки по индексу
        void createNewTab(); // создание ново вкладки
        void newWindow(); // открытие нового окна
        void about(); // вывод информации о программе
};
//-------------------------------------------------------------------
class TabPage : public QWidget{
    Q_OBJECT
private:
    QHBoxLayout* phbx;
    QVBoxLayout* playout;
    QWebView* page;
    QLineEdit* m_ptxt;
    QPushButton* m_pcmdBack;
    QPushButton* m_pcmdForward;
    QProgressBar* pprb;
    QPushButton* pcmdGo;
    QPushButton* pcmdStop;
    QPushButton* pcmdRefresh;
    QProgressDialog *progressDialog;
    QString fileName;
    QMenu* contextMenu;
    QNetworkAccessManager* m_NetworkMngr;
    QNetworkReply *reply;
    QAction *copy;
    QAction *paste;
    QAction *download;
    QString downloadUrl;
public:
    TabPage();
private slots:
    void slotGo(); // переход по ссылке
    void slotFinished(); // конец загрузки страницы
    void setLink(QUrl linkUrl); // загрузка по внешней ссылке и сохранение ссылки загрузки
    void updateDataReadProgress(qint64 bytesRead, qint64 totalBytes); // отображение процесса загрузки файла
    void cancelDownload(); // отмена загрузки файла
    void downloadFile(); // загрузка файла
};

#endif // WEBBROWSER_H
