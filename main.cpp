#include <QApplication>
#include <QtGui>
#include <QTextCodec>
#include "WebBrowser.h"

int main(int argc, char* argv[]){
    QApplication app(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("cp1251"));
    app.setWindowIcon (QIcon(":/images/icon.png"));
    WebBrowser webBrowser;
    QWebSettings::globalSettings()->setAttribute(QWebSettings::AutoLoadImages,true);
            QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptEnabled, true);
            QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, true);
   webBrowser.showMaximized();
    return app.exec();
}
