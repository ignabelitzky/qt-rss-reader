#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include "dialogs/addfeeddialog.h"
#include "src/delegates/htmldelegate.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    showMaximized();
    setWindowTitle("Qt RSS Reader");

    QFont font = ui->searchLineEdit->font();
    font.setPointSize(14);
    ui->searchLineEdit->setFont(font);

    m_searchTimer = new QTimer(this);
    m_searchTimer->setInterval(150);
    m_searchTimer->setSingleShot(true);

    HtmlDelegate *delegate = new HtmlDelegate(this);
    ui->rssTableView->setItemDelegate(delegate);

    m_repo = new FeedRepository();
    m_feedModel = new FeedTableModel(m_repo, this);
    m_fetcher = new RssFetcher(this);
    m_rssModel = new RssModel(this);

    ui->feedTableView->setModel(m_feedModel);
    ui->feedTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->feedTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->feedTableView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->feedTableView->horizontalHeader()->setStretchLastSection(true);
    ui->feedTableView->resizeColumnsToContents();

    ui->rssTableView->setModel(m_rssModel);
    ui->rssTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->rssTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->rssTableView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->rssTableView->setColumnWidth(0, 600);
    ui->rssTableView->setColumnWidth(1, 600);
    ui->rssTableView->setColumnWidth(2, 300);
    ui->rssTableView->setColumnWidth(3, 150);
    ui->rssTableView->resizeRowsToContents();
    ui->rssTableView->horizontalHeader()->setStretchLastSection(true);

    setupConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onRefresh()
{
    ui->searchLineEdit->clear();
    reloadFeedsPreserveSelection();
    ui->searchLineEdit->setFocus();
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
    Q_UNUSED(previous);
    if (!current.isValid())
        return;

    Feed feed = m_feedModel->feedAt(current.row());

    if (!feed.enabled || feed.url.isEmpty())
        return;

    m_currentFeedId = feed.id;

    m_rssModel->setItems({});
    m_fetcher->fetch(feed.id, QUrl(feed.url));
}

void MainWindow::onContactActionTriggered()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Contact");

    msgBox.setTextFormat(Qt::RichText);
    msgBox.setTextInteractionFlags(Qt::TextBrowserInteraction);

    msgBox.setText(
        "<h3>Qt RSS Reader</h3>"
        "<p>A simple desktop RSS reader built with Qt.</p>"
        "<p><b>Author:</b> Ignacio Belitzky</p>"
        "<p><b>GitHub:</b> "
        "<a href='https://github.com/ignabelitzky/qt-rss-reader'>"
        "Project Repository</a></p>"
    );

    msgBox.setStandardButtons(QMessageBox::Ok);

    msgBox.exec();
}

void MainWindow::setupConnections()
{
    connect(ui->refreshButton, &QPushButton::clicked, this, &MainWindow::onRefresh);
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::onAddFeed);
    connect(ui->removeButton, &QPushButton::clicked, this, &MainWindow::onRemoveFeed);

    connect(ui->feedTableView->selectionModel(),
            &QItemSelectionModel::currentRowChanged,
            this, &MainWindow::onFeedSelected);

    connect(m_fetcher, &RssFetcher::feedReady,
            this, [this](int feedId, const QVector<RssItem>& items) {

                if (feedId != m_currentFeedId)
                    return;

                m_rssModel->setItems(items);
                ui->rssTableView->resizeRowsToContents();

                Feed feed = m_repo->getById(feedId);
                feed.lastFetched = QDateTime::currentDateTime();
                m_repo->update(feed);

                reloadFeedsPreserveSelection();
            });

    connect(m_fetcher, &RssFetcher::errorOcurred,
            this, [](int feedId, const QString& err) {
                qDebug() << "Feed error:" << feedId << err;
            });

    connect(ui->rssTableView, &QTableView::doubleClicked,
            this, [this](const QModelIndex& index) {
                if (!index.isValid())
                    return;

                const RssItem& item = m_rssModel->itemAt(index.row());
                QDesktopServices::openUrl(QUrl(item.link()));
            });

    connect(m_searchTimer, &QTimer::timeout,
            this, [this]() {
                const QString searchTerm = ui->searchLineEdit->text();
                m_feedModel->setSearchTerm(searchTerm);
            });

    connect(ui->searchLineEdit, &QLineEdit::textChanged, m_searchTimer, qOverload<>(&QTimer::start));

    connect(ui->addFeedAction, &QAction::triggered, this, &MainWindow::onAddFeed);
    connect(ui->exitAction, &QAction::triggered, this, &QApplication::quit);
    connect(ui->contactAction, &QAction::triggered, this, &MainWindow::onContactActionTriggered);
}

void MainWindow::reloadFeedsPreserveSelection()
{
    auto* selModel = ui->feedTableView->selectionModel();
    selModel->blockSignals(true);

    QModelIndex current = ui->feedTableView->currentIndex();

    int selectedId = -1;

    if (current.isValid())
    {
        selectedId = m_feedModel->feedAt(current.row()).id;
    }

    m_feedModel->load();

    if (selectedId != -1)
    {
        for (int row = 0; row < m_feedModel->rowCount(); ++row)
        {
            if (m_feedModel->feedAt(row).id == selectedId)
            {
                QModelIndex index = m_feedModel->index(row, 0);

                selModel->setCurrentIndex(
                    index,
                    QItemSelectionModel::Select | QItemSelectionModel::Rows
                    );
                break;
            }
        }
    }

    selModel->blockSignals(false);
}

