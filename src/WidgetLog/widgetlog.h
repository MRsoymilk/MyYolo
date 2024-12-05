#ifndef WIDGETLOG_H
#define WIDGETLOG_H

#include <QWidget>

namespace Ui
{
class WidgetLog;
}

class WidgetLog : public QWidget
{
    Q_OBJECT

   public:
    static WidgetLog &getInstance();
    ~WidgetLog();
    void log(const QString &log);
    void logTrace(const QString &log);
    void logInfo(const QString &log);
    void logWarn(const QString &log);
    void logError(const QString &log);
   signals:
    void signalHide();
   private slots:
    void on_btnHide_clicked();
    void on_btnClear_clicked();

   private:
    explicit WidgetLog(QWidget *parent = nullptr);

   private:
    Ui::WidgetLog *ui;
};

#define MY_WIDGET_LOG WidgetLog::getInstance()

#endif  // WIDGETLOG_H
