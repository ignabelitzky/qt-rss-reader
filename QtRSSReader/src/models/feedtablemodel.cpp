#include "feedtablemodel.h"

FeedTableModel::FeedTableModel(FeedRepository* repo, QObject *parent)
    : QAbstractTableModel{parent}
    , m_repo(repo)
{
    load();
}

int FeedTableModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_feeds.size();
}

int FeedTableModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return ColumnCount;
}

QVariant FeedTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return {};

    const Feed& feed = m_feeds.at(index.row());

    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
        case Id:
            return feed.id;
        case Name:
            return feed.name;
        case Url:
            return feed.url;
        case Enabled:
            return feed.enabled ? "Yes" : "No";
        case LastFetched:
            return feed.lastFetched.isValid()
                       ? feed.lastFetched.toString()
                       : "Never";
        }
    }

    if (role == Qt::EditRole)
    {
        switch (index.column())
        {
        case Name:
            return feed.name;
        case Url:
            return feed.url;
        case Enabled:
            return feed.enabled;
        case LastFetched:
            return feed.lastFetched;
        }
    }

    return {};
}

QVariant FeedTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return {};

    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case Id:
            return "ID";
        case Name:
            return "Name";
        case Url:
            return "URL";
        case Enabled:
            return "Enabled";
        case LastFetched:
            return "Last Fetched";
        }
    }

    return {};
}

Qt::ItemFlags FeedTableModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
    {
        return Qt::NoItemFlags;
    }

    Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;

    // Editable columns
    if (index.column() == Name ||
        index.column() == Url ||
        index.column() == Enabled)
    {
        flags |= Qt::ItemIsEditable;
    }

    return flags;
}

bool FeedTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
    {
        return false;
    }

    Feed& feed = m_feeds[index.row()];

    switch (index.column())
    {
    case Name:
        feed.name = value.toString();
        break;
    case Url:
        feed.url = value.toString();
        break;
    case Enabled:
        feed.enabled = value.toBool();
        break;
    default:
        return false;
    }

    if (!m_repo->update(feed))
    {
        return false;
    }

    emit dataChanged(index, index);
    return true;
}

void FeedTableModel::load()
{
    beginResetModel();
    m_feeds = m_repo->getAll();
    endResetModel();
}

void FeedTableModel::addFeed(const Feed& feed)
{
    int newRow = m_feeds.size();

    beginInsertRows(QModelIndex(), newRow, newRow);

    m_repo->add(feed);

    // reload to get DB-assigned ID
    m_feeds = m_repo->getAll();

    endInsertRows();
}

void FeedTableModel::removeFeed(int row)
{
    if (row < 0 || row >= m_feeds.size())
        return;

    beginRemoveRows(QModelIndex(), row, row);

    int id = m_feeds[row].id;
    m_repo->remove(id);
    m_feeds.removeAt(row);

    endRemoveRows();
}

const Feed& FeedTableModel::feedAt(int row) const
{
    return m_feeds.at(row);
}
