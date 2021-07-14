#ifndef VIDEOTABLE_H
#define VIDEOTABLE_H

#include <QTableWidget>

class VideoTable : public QTableWidget
{
private:
    enum Columns {
        NameCol = 0,
        SizeCol,
        CRFCol,
        FormatCol,
        CopyMetaCol,
        NewSizeCol,
        FullPathCol,
    };

public:
    VideoTable(QWidget* parent);

    enum Format
    {
        H264,
        H265
    };

    struct CompressItem
    {
        CompressItem() = default;
        CompressItem(int id, const QString& path, Format format, unsigned crf, bool copyMeta)
            : id(id)
            , path(path)
            , format(format)
            , crf(crf)
            , copyMeta(copyMeta)
        { }
        int id;
        QString path;
        Format format;
        unsigned crf;
        bool copyMeta;
    };

    QVector<CompressItem> compressItems();
    CompressItem compressItem(int id);

    void setItemTargetSize(int id, qint64 size);
    void setItemError(int id);
    void selectById(int id);
    void setSelItemOptions(unsigned crf, Format format, bool copyMeta);
    void setAllItemOptions(unsigned crf, Format format, bool copyMeta);
    void setItemOptions(int id, unsigned crf, Format format, bool copyMeta);

private:
    void dragEnterEvent(QDragEnterEvent *event) final override;
    void dropEvent(QDropEvent *event) final override;
    void dragMoveEvent(QDragMoveEvent *event) final override;
};

#endif // VIDEOTABLE_H
