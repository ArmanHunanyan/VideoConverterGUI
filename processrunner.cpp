#include "processrunner.h"

#include <QTextStream>
#include <QDebug>
#include <QTimer>

ProcessRunner::ProcessRunner(QObject *parent)
    : QObject(parent)
    , m_process(new QProcess(this))
{
    bool cres;
    cres = QObject::connect(m_process, SIGNAL(started()), this, SLOT(onProcessStarted()));
    Q_ASSERT(cres);
    cres = QObject::connect(m_process, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(onProcessError(QProcess::ProcessError)));
    Q_ASSERT(cres);
    cres = QObject::connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                            [=](int exitCode, QProcess::ExitStatus exitStatus){onProcessFinished(exitCode, exitStatus);});
    Q_ASSERT(cres);
    cres = QObject::connect(m_process, SIGNAL(readyReadStandardError()), this, SLOT(onReadyReadStandardError()));
    Q_ASSERT(cres);
    cres = QObject::connect(m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(onReadyReadStandardOutput()));
    Q_ASSERT(cres);
}

void ProcessRunner::clear()
{
    m_ids.clear();
    m_apps.clear();
    m_args.clear();
}

void ProcessRunner::addTask(int id, const QString &app, const QStringList &args)
{
    m_ids.push_back(id);
    m_apps.push_back(app);
    m_args.push_back(args);
}

void ProcessRunner::run()
{
    m_process->setProgram(m_apps.front());
    m_process->setArguments(m_args.front());
    m_process->start();
    Q_EMIT batchStarted();
}

void ProcessRunner::onProcessStarted()
{
    QString cmdline = m_apps.front() + " " + m_args.front().join(" ");
    Q_EMIT processStarted(m_ids.front(), cmdline);
}

void ProcessRunner::onProcessError(QProcess::ProcessError error)
{
    QString errorMsg;
    switch (error) {
    case QProcess::FailedToStart:
        errorMsg = "Application failed to start";
        break;
    case QProcess::Crashed:
        errorMsg = "Application crashed";
        break;
    case QProcess::Timedout:
        errorMsg = "Application timedout";
        break;
    case QProcess::WriteError:
    case QProcess::ReadError:
        errorMsg = "Application IO error";
        break;
    case QProcess::UnknownError:
    default:
        errorMsg = "Unknown error";
        break;
    }
    Q_EMIT(processError(errorMsg));
}

void ProcessRunner::onProcessFinished(int code, QProcess::ExitStatus status)
{
    QString cmdline = m_apps.front() + " " + m_args.front().join(" ");
    Q_EMIT processFinished(m_ids.front(), cmdline, status == QProcess::NormalExit && code == 0);

    m_ids.pop_front();
    m_apps.pop_front();
    m_args.pop_front();
    if (m_apps.isEmpty()) {
        Q_EMIT batchFinished();
        return;
    }

    QTimer::singleShot(0, this, [&](){
        m_process->close();
        m_process->setProgram(m_apps.front());
        m_process->setArguments(m_args.front());
        m_process->start();
    });

}

void ProcessRunner::onReadyReadStandardError()
{
    QString err;
    m_process->setReadChannel(QProcess::StandardError);
    QTextStream stream(m_process);
    QString cont = stream.readAll();
    Q_EMIT readyReadStandardError(cont);
}

void ProcessRunner::onReadyReadStandardOutput()
{
    QString err;
    m_process->setReadChannel(QProcess::StandardOutput);
    QTextStream stream(m_process);
    QString cont = stream.readAll();
    Q_EMIT readyReadStandardOutput(cont);
}
