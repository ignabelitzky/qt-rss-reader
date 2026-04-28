#include "htmldelegate.h"

HtmlDelegate::HtmlDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

void HtmlDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    QTextDocument doc;
    doc.setHtml("<div style='color:white;'>" + index.data().toString() + "</div>");

    doc.setTextWidth(option.rect.width());

    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);
    opt.text = "";

    opt.widget->style()->drawControl(QStyle::CE_ItemViewItem, &opt, painter);

    painter->translate(option.rect.topLeft());

    QRect clip(0, 0, option.rect.width(), option.rect.height());
    doc.drawContents(painter, clip);

    painter->restore();
}

QSize HtmlDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QTextDocument doc;
    doc.setHtml("<div style='color:white;'>" + index.data().toString() + "</div>");
    doc.setTextWidth(option.rect.width());
    return QSize(doc.idealWidth(), doc.size().height());
}
