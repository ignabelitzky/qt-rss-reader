#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include "dialogs/addfeeddialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_repo = new FeedRepository();
    m_feedModel = new FeedTableModel(m_repo, this);
    m_fetcher = new RssFetcher(this);
    m_rssModel = new RssModel(this);

    ui->feedTableView->setModel(m_feedModel);
    ui->feedTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->feedTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->feedTableView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->feedTableView->horizontalHeader()->setStretchLastSection(true);

    ui->rssTableView->setModel(m_rssModel);
    ui->rssTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->rssTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->rssTableView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->rssTableView->horizontalHeader()->setStretchLastSection(true);

    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::onAddFeed);
    connect(ui->removeButton, &QPushButton::clicked, this, &MainWindow::onRemoveFeed);

    connect(ui->feedTableView->selectionModel(),
            &QItemSelectionModel::currentRowChanged,
            this, &MainWindow::onFeedSelected);

    connect(m_fetcher, &RssFetcher::feedReady,
            this, [this](const QVector<RssItem>& items) {
        m_rssModel->setItems(items);
    });

    connect(m_fetcher, &RssFetcher::errorOcurred,
            this, [](const QString& err) {
        qDebug() << "Error:" << err;
    });

    connect(ui->rssTableView, &QTableView::doubleClicked,
            this, [this](const QModelIndex& index) {
        if (!index.isValid())
            return;

        const RssItem& item = m_rssModel->itemAt(index.row());
        QDesktopServices::openUrl(QUrl(item.link()));
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onAddFeed()
{
    AddFeedDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted)
    {
        Feed feed = dialog.getFeed();

        if (feed.url.isEmpty())
            return;

        m_feedModel->addFeed(feed);
    }
}

void MainWindow::onRemoveFeed()
{
    QModelIndex index = ui->feedTableView->currentIndex();

    if (!index.isValid())
    {
        QMessageBox::warning(this, "Remove Feed", "No feed selected.");
        return;
    }

    int row = index.row();
    auto reply = QMessageBox::question(
        this,
        "Remove Feed",
        "Are you sure you want to remove the selected feed?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply != QMessageBox::Yes)
        return;

    m_feedModel->removeFeed(row);
}

void MainWindow::onFeedSelected(const QModelIndex& current, const QModelIndex& previous)
{
    if (!current.isValid())
        return;

    int row = current.row();
    const Feed& feed = m_feedModel->feedAt(row);

    if (!feed.enabled || feed.url.isEmpty())
        return;

    m_rssModel->setItems({});

    m_fetcher->fetch(feed.url);
}
