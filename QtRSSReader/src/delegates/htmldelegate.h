#pragma once

#include <QStyledItemDelegate>
#include <QPainter>
#include <QTextDocument>

class HtmlDelegate : public QStyledItemDelegate
{
public:
    explicit HtmlDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};
