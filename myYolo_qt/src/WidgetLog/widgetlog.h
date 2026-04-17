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
    static WidgetLog *getInstance();
    ~WidgetLog();
    void setMaxLength(const int &length);
    void log(const QString &log);
    void logTrace(const QString &log);
    void logInfo(const QString &log);
    void logWarn(const QString &log);
    void logError(const QString &log);
signals:
    void signalHide();
    void windowClose();
private slots:
    void on_btnHide_clicked();
    void on_btnClear_clicked();

    void slotTextChanged();

private:
    explicit WidgetLog(QWidget *parent = nullptr);

private:
    Ui::WidgetLog *ui;
    void initLog();
    int m_maxLength;

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
};

#define MY_WIDGET_LOG WidgetLog::getInstance()

#endif  // WIDGETLOG_H
