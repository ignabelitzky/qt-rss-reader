#pragma once

#include <QMainWindow>
#include "../core/rssfetcher.h"
#include "../models/rssmodel.h"
#include "../models/feedtablemodel.h"
#include "../core/repositories/feedrepository.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onRefresh();
    void onAddFeed();
    void onRemoveFeed();
    void onFeedSelected(const QModelIndex& current, const QModelIndex& previous);

private:
    void setupConnections();
    void reloadFeedsPreserveSelection();

private:
    Ui::MainWindow *ui;
    RssFetcher* m_fetcher;
    RssModel* m_rssModel;
    FeedRepository* m_repo;
    FeedTableModel* m_feedModel;
    int m_currentFeedId = -1;
};
