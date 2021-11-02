#include "renamefilesdialog.h"
#include "ui_renamefilesdialog.h"

#include <QFileDialog>

RenameFilesDialog::RenameFilesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RenameFilesDialog)
{
    ui->setupUi(this);
}

RenameFilesDialog::~RenameFilesDialog()
{
    delete ui;
}

unsigned int RenameFilesDialog::numDigits() const
{
    return ui->numDigitsSpinBox->value();
}

QString RenameFilesDialog::pattern() const
{
    return ui->patternEdit->text();
}

unsigned RenameFilesDialog::startNumber() const
{
    return ui->startNumSpinBox->value();
}

QString RenameFilesDialog::prefix() const
{
    return ui->prefixEdit->text();
}

QString RenameFilesDialog::directory() const
{
    return ui->dirEdit->text();
}

RenameFilesDialog::SortBy RenameFilesDialog::sortBy() const
{
    if (ui->sortByCombo->currentIndex() == 0) {
        return SB_DATE_ASCENDING;
    }
    return SB_DATE_DESCENDING;
}

void RenameFilesDialog::on_dirBrowse_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "Select Input Directory", ui->dirEdit->text());
    if (path.isEmpty()) {
        return;
    }
    ui->dirEdit->setText(QDir::toNativeSeparators(path));
}


void RenameFilesDialog::on_prefixEdit_textEdited(const QString &arg1)
{
    ui->renameBtn->setEnabled(!ui->dirEdit->text().isEmpty() && !ui->prefixEdit->text().isEmpty());
}


void RenameFilesDialog::on_renameBtn_clicked()
{
    accept();
}


void RenameFilesDialog::on_cancelBtn_clicked()
{
    reject();
}

