#include "myprocess.h"

#include <QThread>

#include "../include/funcdef.h"
#include "processthread.h"

MyProcess &MyProcess::getInstance()
{
    static MyProcess instance;
    return instance;
}

MyProcess::MyProcess(QObject *parent)
{
    m_process = new QProcess();
    connect(m_process, &QProcess::readyReadStandardOutput, this, &MyProcess::handleStandardOutput);
    connect(m_process, &QProcess::readyReadStandardError, this, &MyProcess::handleStandardError);
    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
            &MyProcess::handleProcessFinished);
}

void MyProcess::startAttach(const QString &program, const QStringList &arguments)
{
    auto *thread = new ProcessThread(program, arguments, this);
    thread->start();
}

bool MyProcess::startDetach(const QString &program, const QStringList &arguments)
{
    return m_process->startDetached(program, arguments);
}

void MyProcess::stopScript(const int &wait_time)
{
    if (m_process && m_process->state() == QProcess::Running)
    {
        m_process->terminate();
        if (!m_process->waitForFinished(wait_time))
        {
            m_process->kill();
        }
        m_process->deleteLater();
    }
}

void MyProcess::handleStandardOutput()
{
    QString output = m_process->readAllStandardOutput();
}

void MyProcess::handleStandardError()
{
    QString error = m_process->readAllStandardError();
}

void MyProcess::handleProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    QString message = (exitStatus == QProcess::NormalExit) ? QString("Process finished with exit code %1").arg(exitCode)
                                                           : "Process crashed.";
}
