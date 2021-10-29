#ifndef COPYMETADIALOG_H
#define COPYMETADIALOG_H

#include <QDialog>

namespace Ui {
class copymetadialog;
}

class CopyMetaDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CopyMetaDialog(QWidget *parent = nullptr);
    ~CopyMetaDialog();

    QString fromFilePath() const;
    QString toFileDirectory() const;
    QString copyMetaFromFilePath() const;
    QString prefix() const;

private slots:
    void on_toolButton_clicked();
    void on_convertBtn_clicked();
    void on_cancelBtn_clicked();
    void on_toolButton_2_clicked();
    void on_toolButton_3_clicked();

    void on_metaFromEdit_textChanged(const QString &arg1);

private:
    Ui::copymetadialog *ui;
};

#endif // COPYMETADIALOG_H
