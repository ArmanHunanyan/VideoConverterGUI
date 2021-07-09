#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralwidget->setAcceptDrops(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_compressButton_clicked()
{
    QString outputDir = ui->outputDirEdit->text();
    QString suffix = ui->suffixEdit->text();
    if (outputDir.isEmpty()) {
        QMessageBox::information(this, "Output Dir is Empty", "Please select output directory");
        return;
    }
    this->setDisabled(true);
    QVector<VideoTable::CompressItem> items = this->ui->mainTable->compressItems();
    for (const auto& item: items) {
        QFileInfo info(item.path);
        auto outputFile = outputDir.trimmed();
        if (!outputFile.endsWith(QDir::separator())) {
            outputFile.append(QDir::separator());
        }
        outputFile.append(info.baseName() + suffix + '.' + info.completeSuffix());
        outputFile = QDir::toNativeSeparators(outputFile);

        QStringList arguments = QStringList() << "-i" << QDir::toNativeSeparators(item.path) << "-vcodec"
                << (item.format == VideoTable::H264 ? "libx264" : "libx265")
                << "-crf" << QString::number(item.crf);
        if (item.copyMeta) {
            arguments.append("-map_metadata");
            arguments.append("0");
        }
        arguments.append(outputFile);
        QProcess process(this);
        process.setProgram("ffmpeg");
        process.setArguments(arguments);
        process.start();
        process.waitForFinished();
        QFileInfo targetInfo(outputFile);
        ui->mainTable->setItemTargetSize(item.id, targetInfo.size());
        qDebug() << process.arguments().join(" ");
    }
    this->setDisabled(false);
}


void MainWindow::on_outputDirBrowse_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "Select Output Directory", ui->outputDirEdit->text());
    if (path.isEmpty()) {
        return;
    }
    ui->outputDirEdit->setText(QDir::toNativeSeparators(path));
}


void MainWindow::on_actionClear_triggered()
{
    this->ui->mainTable->setRowCount(0);
}

