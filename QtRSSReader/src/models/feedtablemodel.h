#pragma once

#include <QAbstractTableModel>
#include "src/core/repositories/feedrepository.h"

enum Columns {
    Id = 0,
    Name,
    Url,
    Enabled,
    LastFetched,
    ColumnCount
};

class FeedTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit FeedTableModel(FeedRepository* repo, QObject *parent = nullptr);

    // Qt overrides
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;

    // Custom API
    void load();
    void addFeed(const Feed& feed);
    void removeFeed(int row);
    const Feed& feedAt(int row) const;

private:
    QList<Feed> m_feeds;
    FeedRepository* m_repo;
};
