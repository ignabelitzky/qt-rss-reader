#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopServices>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_fetcher = new RssFetcher(this);
    m_model = new RssModel(this);

    ui->rssTableView->setModel(m_model);
    ui->rssTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->rssTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->rssTableView->horizontalHeader()->setStretchLastSection(true);

    connect(m_fetcher, &RssFetcher::feedReady,
            this, [this](const QVector<RssItem>& items) {
        m_model->setItems(items);
    });

    connect(m_fetcher, &RssFetcher::errorOcurred,
            this, [](const QString& err) {
        qDebug() << "Error:" << err;
    });

    connect(ui->loadButton, &QPushButton::clicked,
            this, [this]() {
        const QUrl url(ui->urlLineEdit->text());
        m_fetcher->fetch(url);
    });

    connect(ui->rssTableView, &QTableView::doubleClicked,
            this, [this](const QModelIndex& index) {
        if (!index.isValid())
            return;

        const RssItem& item = m_model->itemAt(index.row());
        QDesktopServices::openUrl(QUrl(item.link()));
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
