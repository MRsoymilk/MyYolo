#ifndef WIDGETLOG_H
#define WIDGETLOG_H

#include <QWidget>

namespace Ui {
class WidgetLog;
}

class WidgetLog : public QWidget
{
    Q_OBJECT

public:
    static WidgetLog& getInstance();
    ~WidgetLog();
    void log(const QString &log);
    void logTrace(const QString &log);
    void logInfo(const QString &log);
    void logWarn(const QString &log);
    void logError(const QString &log);
private slots:
    void on_btnHide_clicked();
    void on_btnClear_clicked();
private:
    explicit WidgetLog(QWidget *parent = nullptr);

private:
    Ui::WidgetLog *ui;
};

#define MY_WIDGET_LOG WidgetLog::getInstance()
#define MY_WIDGET_LOG_TRACE(msg)    MY_WIDGET_LOG.logTrace(msg)
#define MY_WIDGET_LOG_INFO(msg)     MY_WIDGET_LOG.logInfo(msg)
#define MY_WIDGET_LOG_WARN(msg)     MY_WIDGET_LOG.logWarn(msg)
#define MY_WIDGET_LOG_ERROR(msg)    MY_WIDGET_LOG.logError(msg)
#endif // WIDGETLOG_H
