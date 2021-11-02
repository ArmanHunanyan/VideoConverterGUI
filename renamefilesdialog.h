#ifndef RENAMEFILESDIALOG_H
#define RENAMEFILESDIALOG_H

#include <QDialog>

namespace Ui {
class RenameFilesDialog;
}

class RenameFilesDialog : public QDialog
{
    Q_OBJECT
public:
    enum SortBy {
        SB_DATE_ASCENDING,
        SB_DATE_DESCENDING
    };

public:
    explicit RenameFilesDialog(QWidget *parent = nullptr);
    ~RenameFilesDialog();

    unsigned int numDigits()const;
    QString pattern()const;
    unsigned startNumber()const;
    QString prefix()const;
    QString directory()const;
    SortBy sortBy() const;

private slots:
    void on_dirBrowse_clicked();

    void on_prefixEdit_textEdited(const QString &arg1);

    void on_renameBtn_clicked();

    void on_cancelBtn_clicked();

private:
    Ui::RenameFilesDialog *ui;
};

#endif // RENAMEFILESDIALOG_H
