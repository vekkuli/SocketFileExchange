#include "fileexchangeitemdelegate.h"
#include <QPainter>
#include <QApplication>

FileExchangeItemDelegate::FileExchangeItemDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

void FileExchangeItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() == 1) {
        int progress = index.data().toInt();

        QStyleOptionProgressBar progressBarOption;
        progressBarOption.rect = option.rect;
        progressBarOption.minimum = 0;
        progressBarOption.maximum = 100;
        progressBarOption.progress = progress;
        progressBarOption.text = QString::number(progress) + "%";
        progressBarOption.textVisible = true;
        progressBarOption.textAlignment = Qt::AlignCenter;

        QApplication::style()->drawControl(QStyle::CE_ProgressBar,
                                           &progressBarOption, painter);
    } else
        QStyledItemDelegate::paint(painter, option, index);

}

