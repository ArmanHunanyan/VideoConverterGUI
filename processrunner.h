#ifndef PROCESSRUNNER_H
#define PROCESSRUNNER_H

#include <QObject>
#include <QProcess>

class ProcessRunner : public QObject
{
    Q_OBJECT
public:
    explicit ProcessRunner(QObject *parent = nullptr);

    void clear();
    void addTask(int id, const QString& app, const QStringList& args);
    void run();

signals:
    void processStarted(int id, const QString& cmdline);
    void processFinished(int id, const QString& cmdline, bool normal);
    void processError(const QString& msg);
    void batchFinished();
    void batchStarted();
    void readyReadStandardError(const QString& content);
    void readyReadStandardOutput(const QString& content);

private slots:
    void onProcessStarted();
    void onProcessError(QProcess::ProcessError);
    void onProcessFinished(int,QProcess::ExitStatus);
    void onReadyReadStandardError();
    void onReadyReadStandardOutput();

private:
    QVector<int> m_ids;
    QVector<QString> m_apps;
    QVector<QStringList> m_args;
    QProcess* m_process;

};

#endif // PROCESSRUNNER_H
