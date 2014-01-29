#ifndef WEBBROWSER_H
#define WEBBROWSER_H

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
        void closeTab(int index); 
        void createNewTab(); 
        void newWindow(); 
        void about(); 
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
    void slotGo(); 
    void slotFinished();
    void setLink(QUrl linkUrl);
    void updateDataReadProgress(qint64 bytesRead, qint64 totalBytes);
    void cancelDownload(); 
    void downloadFile();
};

#endif // WEBBROWSER_H
