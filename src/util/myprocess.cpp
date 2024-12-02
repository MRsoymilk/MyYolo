#include "myprocess.h"
#include "../include/funcdef.h"

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
    connect(m_process,
            QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this,
            &MyProcess::handleProcessFinished);
}

void MyProcess::startAttach(const QString &program, const QStringList &arguments)
{
    m_process->start(program, arguments);
}

bool MyProcess::startDetach(const QString &program, const QStringList &arguments)
{
    return m_process->startDetached(program, arguments);
}

void MyProcess::stopScript(const int &wait_time)
{
    if (m_process && m_process->state() == QProcess::Running) {
        m_process->terminate();
        if (!m_process->waitForFinished(wait_time)) {
            m_process->kill();
        }
        m_process->deleteLater();
        WIDGET_LOG_INFO("Script stopped.");
    } else {
        WIDGET_LOG_INFO("No Script is running.");
    }
}

void MyProcess::handleStandardOutput()
{
    QString output = m_process->readAllStandardOutput();
    WIDGET_LOG_TRACE(output);
}

void MyProcess::handleStandardError()
{
    QString error = m_process->readAllStandardError();
    WIDGET_LOG_ERROR(error);
}

void MyProcess::handleProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    QString message = (exitStatus == QProcess::NormalExit)
                          ? QString("Process finished with exit code %1").arg(exitCode)
                          : "Process crashed.";
    WIDGET_LOG_INFO(message);
}
