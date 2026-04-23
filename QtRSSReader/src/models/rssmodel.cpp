#include "rssmodel.h"

RssModel::RssModel(QObject *parent)
    : QAbstractTableModel{parent}
{
}

int RssModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_items.size();
}

int RssModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 4; // Title, Description, Link, Date
}

QVariant RssModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return {};

    const RssItem& item = m_items.at(index.row());

    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
        case 0:
            return item.title();
        case 1:
            return item.description();
        case 2:
            return item.link();
        case 3:
            return item.pubDate().toString();
        }
    }

    return {};
}

QVariant RssModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return {};

    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case 0:
            return "Title";
        case 1:
            return "Description";
        case 2:
            return "Link";
        case 3:
            return "Date";
        }
    }

    return {};
}

void RssModel::setItems(const QVector<RssItem> &items)
{
    beginResetModel();
    m_items = items;
    endResetModel();
}

const RssItem &RssModel::itemAt(int row) const
{
    return m_items.at(row);
}
