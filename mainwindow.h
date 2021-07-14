#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "processrunner.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QString targetFileName(const QString& inputFileName);
    void updateSetButtonState();

private slots:
    void on_compressButton_clicked();
    void on_outputDirBrowse_clicked();
    void on_actionClear_triggered();

    void onProcessStarted(int id, const QString& cmdline);
    void onProcessFinished(int id, const QString& cmdline, bool normal);
    void onProcessError(const QString& msg);
    void onBatchFinished();
    void onBatchStarted();
    void onReadyReadStandardError(const QString& content);
    void onReadyReadStandardOutput(const QString& content);

    void on_setOptionsButton_clicked();

    void on_sameForAllCheckBox_stateChanged(int arg1);

    void on_mainTable_itemSelectionChanged();

private:
    Ui::MainWindow *ui;
    ProcessRunner* m_processRunner;

};

#endif // MAINWINDOW_H
