#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_processRunner(new ProcessRunner(this))
{
    ui->setupUi(this);
    ui->centralwidget->setAcceptDrops(true);
    ui->outputDisplay->hide();
    ui->controlWidget->show();

    QObject::connect(m_processRunner, SIGNAL(processStarted(int, const QString&)), this, SLOT(onProcessStarted(int, const QString&)));
    QObject::connect(m_processRunner, SIGNAL(processFinished(int, const QString&, bool)), this, SLOT(onProcessFinished(int, const QString&, bool)));
    QObject::connect(m_processRunner, SIGNAL(processError(const QString&)), this, SLOT(onProcessError(const QString&)));
    QObject::connect(m_processRunner, SIGNAL(readyReadStandardError(const QString&)), this, SLOT(onReadyReadStandardError(const QString&)));
    QObject::connect(m_processRunner, SIGNAL(readyReadStandardOutput(const QString&)), this, SLOT(onReadyReadStandardOutput(const QString&)));
    QObject::connect(m_processRunner, SIGNAL(batchFinished()), this, SLOT(onBatchFinished()));
    QObject::connect(m_processRunner, SIGNAL(batchStarted()), this, SLOT(onBatchStarted()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::targetFileName(const QString& inputFileName)
{
    QString outputDir = ui->outputDirEdit->text();
    QFileInfo info(inputFileName);
    auto outputFile = outputDir.trimmed();
    if (!outputFile.endsWith(QDir::separator())) {
        outputFile.append(QDir::separator());
    }
    outputFile.append(info.baseName() + ui->suffixEdit->text() + '.' + info.completeSuffix());
    outputFile = QDir::toNativeSeparators(outputFile);
    return outputFile;
}

void MainWindow::on_compressButton_clicked()
{
    QString outputDir = ui->outputDirEdit->text();
    if (outputDir.isEmpty()) {
        QMessageBox::information(this, "Output Dir is Empty", "Please select output directory");
        return;
    }
    m_processRunner->clear();
    QVector<VideoTable::CompressItem> items = this->ui->mainTable->compressItems();
    for (const auto& item: items) {
        QString outputFile = targetFileName(item.path);

        QStringList arguments = QStringList() << "-y"
                << "-i" << QDir::toNativeSeparators(item.path) << "-vcodec"
                << (item.format == VideoTable::H264 ? "libx264" : "libx265")
                << "-crf" << QString::number(item.crf);
        if (item.copyMeta) {
            arguments.append("-map_metadata");
            arguments.append("0");
        }
        arguments.append(outputFile);

        m_processRunner->addTask(item.id, "ffmpeg", arguments);
        m_processRunner->run();
    }
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

void MainWindow::onProcessStarted(int id, const QString& cmdline)
{
    ui->outputDisplay->setTextColor(QColorConstants::Green);
    ui->outputDisplay->insertPlainText(cmdline + "\r\n");
    ui->outputDisplay->setTextColor(QColorConstants::Black);
    ui->outputDisplay->ensureCursorVisible();
    ui->mainTable->selectById(id);
}

void MainWindow::onProcessFinished(int id, const QString& cmdline, bool normal)
{
    if (normal) {
        ui->outputDisplay->setTextColor(QColorConstants::Green);
        ui->outputDisplay->insertPlainText("Successfully finished: " + cmdline + "\r\n");

        QString targetFile = targetFileName(ui->mainTable->compressItem(id).path);
        QFileInfo targetInfo(targetFile);
        ui->mainTable->setItemTargetSize(id, targetInfo.size());

    } else {
        ui->outputDisplay->setTextColor(QColorConstants::Red);
        ui->outputDisplay->insertPlainText("Finished with errors: " + cmdline + "\r\n");
        ui->mainTable->setItemError(id);
    }
    ui->outputDisplay->setTextColor(QColorConstants::Black);
    ui->outputDisplay->ensureCursorVisible();
}

void MainWindow::onProcessError(const QString& msg)
{

}

void MainWindow::onBatchFinished()
{
    ui->outputDisplay->hide();
    ui->controlWidget->show();
}

void MainWindow::onBatchStarted()
{
    ui->outputDisplay->show();
    ui->controlWidget->hide();
}

void MainWindow::onReadyReadStandardError(const QString& content)
{
    ui->outputDisplay->append(content);
}

void MainWindow::onReadyReadStandardOutput(const QString& content)
{
    ui->outputDisplay->append(content);
}

void MainWindow::on_setOptionsButton_clicked()
{
    if (!ui->sameForAllCheckBox->isChecked()) {
        ui->mainTable->setSelItemOptions(ui->crfEdit->text().toUInt(),
                               ui->formatEdit->currentText() == "H.264" ? VideoTable::H264 : VideoTable::H265,
                               ui->copyMetaCheckbox->isChecked());
    } else {
        ui->mainTable->setAllItemOptions(ui->crfEdit->text().toUInt(),
                               ui->formatEdit->currentText() == "H.264" ? VideoTable::H264 : VideoTable::H265,
                               ui->copyMetaCheckbox->isChecked());
    }
}


void MainWindow::on_sameForAllCheckBox_stateChanged(int arg1)
{
    updateSetButtonState();
}

void MainWindow::on_mainTable_itemSelectionChanged()
{
    updateSetButtonState();
}

void MainWindow::updateSetButtonState()
{
    if (!ui->sameForAllCheckBox->isChecked() && ui->mainTable->selectedItems().isEmpty()) {
        ui->setOptionsButton->setDisabled(true);
    } else {
        ui->setOptionsButton->setEnabled(true);
    }
}


