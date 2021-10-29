#include "copymetadialog.h"
#include "ui_copymetadialog.h"
#include <QFileDialog>

CopyMetaDialog::CopyMetaDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::copymetadialog)
{
    ui->setupUi(this);
}

CopyMetaDialog::~CopyMetaDialog()
{
    delete ui;
}

QString CopyMetaDialog::fromFilePath() const
{
    return ui->fromEdit->text();
}

QString CopyMetaDialog::toFileDirectory() const
{
    return ui->toEdit->text();
}

QString CopyMetaDialog::copyMetaFromFilePath() const
{
    return ui->metaFromEdit->text();
}

QString CopyMetaDialog::prefix() const
{
    return ui->prefixEdit->text();
}

void CopyMetaDialog::on_toolButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "Select File to Copy", ui->fromEdit->text());
    if (path.isEmpty()) {
        return;
    }
    ui->fromEdit->setText(QDir::toNativeSeparators(path));
}

void CopyMetaDialog::on_toolButton_2_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "Select Output Directory", ui->toEdit->text());
    if (path.isEmpty()) {
        return;
    }
    ui->toEdit->setText(QDir::toNativeSeparators(path));
}

void CopyMetaDialog::on_toolButton_3_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "Select File with Metadata to Copy", ui->metaFromEdit->text());
    if (path.isEmpty()) {
        return;
    }
    ui->metaFromEdit->setText(QDir::toNativeSeparators(path));
}

void CopyMetaDialog::on_convertBtn_clicked()
{
    accept();
}


void CopyMetaDialog::on_cancelBtn_clicked()
{
    reject();
}


void CopyMetaDialog::on_metaFromEdit_textChanged(const QString &arg1)
{
    ui->convertBtn->setEnabled(!ui->fromEdit->text().isEmpty() &&
                               !ui->toEdit->text().isEmpty() &&
                               !ui->metaFromEdit->text().isEmpty());
}

