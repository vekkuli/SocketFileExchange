#ifndef FILEEXCHANGEITEMDELEGATE_H
#define FILEEXCHANGEITEMDELEGATE_H

#include <QStyledItemDelegate>

class FileExchangeItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    FileExchangeItemDelegate(QObject* parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const Q_DECL_OVERRIDE;

//    QSize sizeHint(const QStyleOptionViewItem &option,
//                   const QModelIndex &index ) const Q_DECL_OVERRIDE;

signals:

public slots:
};

#endif // UPLOADITEMDELEGATE_H
