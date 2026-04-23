#pragma once

#include <QAbstractTableModel>
#include "../core/rssitem.h"

class RssModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit RssModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void setItems(const QVector<RssItem>& items);
    const RssItem& itemAt(int row) const;

private:
    QVector<RssItem> m_items;
};
