#include "videotable.h"
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDebug>
#include <QHeaderView>
#include <QFileInfo>

VideoTable::VideoTable(QWidget* parent)
    : QTableWidget(parent)
{
    setAcceptDrops(true);
    this->horizontalHeader()->setStretchLastSection(true);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
}

QVector<VideoTable::CompressItem> VideoTable::compressItems()
{
    QVector<VideoTable::CompressItem> res;
    for (int row = 0; row < rowCount(); ++row) {
        res.append(
               CompressItem(
                    row,
                    item(row, FullPathCol)->text(),
                    item(row, FormatCol)->text() == "H.264" ? H264 : H265,
                    item(row, CRFCol)->text().toUInt(),
                    item(row, CopyMetaCol)->checkState() == Qt::Checked
                        )
            );
    }
    return res;
}

VideoTable::CompressItem VideoTable::compressItem(int id)
{
    return CompressItem(
                id,
                item(id, FullPathCol)->text(),
                item(id, FormatCol)->text() == "H.264" ? H264 : H265,
                item(id, CRFCol)->text().toUInt(),
                item(id, CopyMetaCol)->checkState() == Qt::Checked
                    );
}

void VideoTable::setItemTargetSize(int id, qint64 size)
{
    QTableWidgetItem* item = new QTableWidgetItem(
                QString::number(double(size) / (1024.0 * 1024.0), 'f', 2) + "Mb");
    item->setFlags(item->flags() &  ~Qt::ItemIsEditable);
    this->setItem(id, NewSizeCol, item);
}

void VideoTable::setItemError(int id)
{
    QTableWidgetItem* item = new QTableWidgetItem(
                "Error");
    item->setForeground(QColorConstants::Red);
    item->setFlags(item->flags() &  ~Qt::ItemIsEditable);
    this->setItem(id, NewSizeCol, item);
}

void VideoTable::selectById(int id)
{
    selectRow(id);
}

void VideoTable::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void VideoTable::dropEvent(QDropEvent *event)
{
    auto urls = event->mimeData()->urls();

    for (const auto& url: urls) {
        int rowIdx = this->rowCount();
        this->insertRow(rowIdx);
        QTableWidgetItem* item = new QTableWidgetItem(url.fileName());
        item->setFlags(item->flags() &  ~Qt::ItemIsEditable);
        this->setItem(rowIdx, NameCol, item);
        QFileInfo info(url.toLocalFile());
        item = new QTableWidgetItem(QString::number(double(info.size()) / (1024.0 * 1024.0), 'f', 2) + "Mb");
        item->setFlags(item->flags() &  ~Qt::ItemIsEditable);
        this->setItem(rowIdx, SizeCol, item);
        item = new QTableWidgetItem(QString::number(23));
        item->setFlags(item->flags() &  ~Qt::ItemIsEditable);
        this->setItem(rowIdx, CRFCol, item);
        item = new QTableWidgetItem(QString("H.264"));
        item->setFlags(item->flags() &  ~Qt::ItemIsEditable);
        this->setItem(rowIdx, FormatCol, item);
        item = new QTableWidgetItem();
        item->setCheckState(Qt::Checked);
        item->setFlags(item->flags() &  ~Qt::ItemIsUserCheckable);
        this->setItem(rowIdx, CopyMetaCol, item);
        item = new QTableWidgetItem(url.toLocalFile());
        item->setFlags(item->flags() &  ~Qt::ItemIsEditable);
        this->setItem(rowIdx, FullPathCol, item);
    }

    event->acceptProposedAction();
}

void VideoTable::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}
