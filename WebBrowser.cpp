#include "WebBrowser.h"

WebBrowser::WebBrowser() : QWidget(){
    this->setAttribute(Qt::WA_DeleteOnClose,true); // флаг  освобождения памяти при закрытии
    pcmdAdd = new QPushButton(QIcon(":/images/add.png"), tr(""), this);
    pcmdConfig = new QPushButton (QIcon(":/images/config.png"), tr(""), this);

    menuconfig = new QMenu;
    newWinAct = new QAction(QIcon(":/images/new.png"),tr("&Новое окно"), this);
    connect(newWinAct, SIGNAL(triggered()), this, SLOT(newWindow()));

    addAct = new QAction(QIcon(":/images/add.png"),tr("&Новая вкладка"), this);
    connect(addAct, SIGNAL(triggered()), this, SLOT(createNewTab()));

    aboutAct = new QAction(QIcon(":/images/about.png"),tr("&О программе"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
    pcmdConfig->setMaximumSize(25,25);

    pcmdConfig->setMenu(menuconfig);
    menuconfig->addAction(addAct);
    menuconfig->addAction(newWinAct);
    menuconfig->addSeparator();
    menuconfig->addAction(aboutAct);
    ftab = new TabPage();

    tabWidget = new QTabWidget;
    tabWidget->addTab(ftab , tr("Новая вкладка"));
    tabWidget->setMovable(true);
    tabWidget->setTabsClosable(true);

    htablayout = new QHBoxLayout;
    htablayout->addWidget(pcmdAdd);
    htablayout->addWidget(pcmdConfig);

    vtablayout = new QVBoxLayout;
    vtablayout->addLayout(htablayout);
    vtablayout->addWidget(tabWidget);
    setLayout(vtablayout);

    connect(pcmdAdd, SIGNAL(clicked()), SLOT(createNewTab()));
    connect(tabWidget,SIGNAL(tabCloseRequested(int)),SLOT(closeTab(int)));
}

void WebBrowser::closeTab(int index){

    TabPage *tab = qobject_cast<TabPage*>(tabWidget->widget(index)); // приведение типов
    tabWidget->removeTab(index);
    tab->close();
}

void WebBrowser::createNewTab(){
    TabPage* ptab = new TabPage();
    tabWidget->addTab(ptab, tr("Новая вкладка"));
}
void WebBrowser::newWindow(){
    WebBrowser* newWin = new WebBrowser;
    newWin->showMaximized();

}
void WebBrowser::about(){
   QMessageBox::about(this, tr("О программе"), tr("Курсовая работа по программированию <br> Браузер"));
}
//--------------------------------------------------------------------------------------------------------
TabPage::TabPage() : QWidget(){
    this->setAttribute(Qt::WA_DeleteOnClose,true);
    page = new QWebView;
    m_ptxt = new QLineEdit("http://www.google.com");
    m_pcmdBack = new QPushButton(QIcon(":/images/back.png"), tr(""), this);
    m_pcmdForward = new QPushButton(QIcon(":/images/forward.png"), tr(""), this);
    m_pcmdForward->setEnabled(false);
    pprb = new QProgressBar;
    pcmdGo = new QPushButton(QIcon(":/images/go.png"), tr(""), this);
    pcmdStop = new QPushButton(QIcon(":/images/stop.png"), tr(""), this);
    pcmdRefresh = new QPushButton(QIcon(":/images/reload.png"), tr(""), this);

    download = new QAction("Download", page);
    copy = page->pageAction(QWebPage::Copy);
    copy->setShortcuts(QKeySequence::Copy);
    paste = page->pageAction(QWebPage::Paste);
    paste->setShortcuts(QKeySequence::Paste);

    connect (pcmdGo, SIGNAL(clicked()), SLOT(slotGo()));
    connect (m_ptxt, SIGNAL(returnPressed()), SLOT(slotGo()));
    connect (m_pcmdBack, SIGNAL(clicked()), page, SLOT(back()));
    connect (m_pcmdForward, SIGNAL(clicked()), page, SLOT(forward()));
    connect (pcmdRefresh, SIGNAL(clicked()), page, SLOT(reload()));
    connect (pcmdStop, SIGNAL(clicked()), page, SLOT(stop()));
    connect (page, SIGNAL(loadProgress(int)), pprb, SLOT(setValue(int)));
    connect (page, SIGNAL(loadFinished(bool)), SLOT(slotFinished()));
    connect (download, SIGNAL(triggered()), this, SLOT(downloadFile()));

     page->load(QUrl(m_ptxt->text()));

     page->addAction(download);
     page->addAction(copy);
     page->addAction(paste);
     page->setContextMenuPolicy(Qt::ActionsContextMenu);

     //Layout setur
    phbx = new QHBoxLayout;
    phbx->addWidget(m_pcmdBack);
    phbx->addWidget(m_pcmdForward);
    phbx->addWidget(pcmdRefresh);
    phbx->addWidget(pcmdStop);
    phbx->addWidget(m_ptxt);
    phbx->addWidget(pcmdGo);
    phbx->addWidget(pprb);

    playout = new QVBoxLayout;
    playout->addLayout(phbx);
    playout->addWidget(page);
    setLayout(playout);
}


void TabPage::slotGo(){ // проверка на волидность ввода в адресную строку
    if (!m_ptxt->text().startsWith("ftp:/") && !m_ptxt->text().startsWith("http://")) m_ptxt->setText("http://" + m_ptxt->text());
    page->load(QUrl(m_ptxt->text()));
}
void TabPage::slotFinished(){
    m_ptxt->setText(page->url().toString());
    m_pcmdBack->setEnabled(page->page()->history()->canGoBack());
    m_pcmdForward->setEnabled(page->page()->history()->canGoForward());
    page->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks); // аквтивация перехода по ссылкам
    connect(page, SIGNAL(linkClicked(QUrl)),this, SLOT(setLink(QUrl)));
}
void TabPage::setLink(QUrl linkUrl){
 page->load(linkUrl);
 downloadUrl=linkUrl.toString();
}
void TabPage::downloadFile(){
    if (downloadUrl!=0){
    QUrl aUrl(downloadUrl);
    QFileInfo fileInfo=aUrl.path();
    fileName = fileInfo.fileName();
    if (QFile::exists(fileName)) {
        if (QMessageBox::question(this, tr("Загрузка"),
                                  tr("Файл %1 уже существует в "
                                     "текущем каталоге. Перезаписать?").arg(fileName),
                                  QMessageBox::Yes|QMessageBox::No, QMessageBox::No)
            == QMessageBox::No)
            return;
        QFile::remove(fileName);
    }
    progressDialog = new QProgressDialog;
    m_NetworkMngr = new QNetworkAccessManager(this);
    reply = m_NetworkMngr->get(QNetworkRequest(downloadUrl)); // загрузка
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)),this, SLOT(updateDataReadProgress(qint64,qint64))); //процесс
    connect(progressDialog, SIGNAL(canceled()), this, SLOT(cancelDownload()));
    progressDialog->setWindowTitle(tr("Загрузка"));
    progressDialog->setLabelText(tr("Загружается %1.").arg(fileName));
    progressDialog->show();

    QEventLoop loop;
    connect(reply, SIGNAL(finished()),&loop, SLOT(quit()));
    loop.exec();

    QFile file(fileInfo.fileName());
    file.open(QIODevice::WriteOnly);
    file.write(reply->readAll());

    delete reply;
    }
}
void TabPage::updateDataReadProgress(qint64 bytesRead, qint64 totalBytes){
    progressDialog->setMaximum(totalBytes);
    progressDialog->setValue(bytesRead);
}
void TabPage::cancelDownload(){
    reply->abort();
    //QFile::remove(fileName);
}


