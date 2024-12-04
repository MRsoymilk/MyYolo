#ifndef PROCESSTHREAD_H
#define PROCESSTHREAD_H

#include <QProcess>
#include <QThread>

class ProcessThread : public QThread {
  Q_OBJECT

 public:
  explicit ProcessThread(const QString &program, const QStringList &arguments,
                         QObject *parent = nullptr)
      : QThread(parent), m_program(program), m_arguments(arguments) {}

 signals:
  void outputReceived(const QString &output);
  void errorReceived(const QString &error);
  void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

 protected:
  void run() override {
    QProcess process;
    connect(&process, &QProcess::readyReadStandardOutput,
            [&]() { emit outputReceived(process.readAllStandardOutput()); });
    connect(&process, &QProcess::readyReadStandardError,
            [&]() { emit errorReceived(process.readAllStandardError()); });
    process.start(m_program, m_arguments);
    if (!process.waitForFinished(-1)) {
      emit errorReceived(
          QString("Process failed: %1").arg(process.errorString()));
    } else {
      emit processFinished(process.exitCode(), process.exitStatus());
    }
  }

 private:
  QString m_program;
  QStringList m_arguments;
};

#endif  // PROCESSTHREAD_H
