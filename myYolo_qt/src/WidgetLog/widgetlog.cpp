#include "widgetlog.h"

#include "../include/funcdef.h"
#include "../include/keydef.h"
#include "ui_widgetlog.h"

WidgetLog::WidgetLog(QWidget *parent) : QWidget(parent), ui(new Ui::WidgetLog)
{
    ui->setupUi(this);
    initLog();
}

void WidgetLog::initLog()
{
    m_maxLength = SETTING_GET(CFG_GROUP_LOG, CFG_LOG_LENGTH, "10000").toInt();
    connect(ui->txtEditLog, &QTextEdit::textChanged, this, &WidgetLog::slotTextChanged);
}

void WidgetLog::closeEvent(QCloseEvent *event) { emit windowClose(); }

void WidgetLog::slotTextChanged()
{
    if (ui->txtEditLog->toPlainText().length() > m_maxLength)
    {
        QString text = ui->txtEditLog->toPlainText().right(m_maxLength);
        ui->txtEditLog->setPlainText(text);
    }
}

WidgetLog *WidgetLog::getInstance()
{
    static WidgetLog* instance = new WidgetLog();
    return instance;
}

WidgetLog::~WidgetLog() { delete ui; }

void WidgetLog::setMaxLength(const int &length) { m_maxLength = length; }

void WidgetLog::log(const QString &log) { ui->txtEditLog->append(log); }

void WidgetLog::logTrace(const QString &log)
{
    QColor color = Qt::darkGray;

    QTextCharFormat format;
    format.setForeground(color);

    QTextCursor cursor = ui->txtEditLog->textCursor();
    cursor.movePosition(QTextCursor::End);
    cursor.insertText(QString("[Trace %1]: %2\n").arg(TIMESTAMP(), log), format);

    ui->txtEditLog->moveCursor(QTextCursor::End);
}

void WidgetLog::logInfo(const QString &log)
{
    QColor color = Qt::black;

    QTextCharFormat format;
    format.setForeground(color);

    QTextCursor cursor = ui->txtEditLog->textCursor();
    cursor.movePosition(QTextCursor::End);
    cursor.insertText(QString("[Info %1]: %2\n").arg(TIMESTAMP(), log), format);

    ui->txtEditLog->moveCursor(QTextCursor::End);
}

void WidgetLog::logWarn(const QString &log)
{
    QColor color = Qt::darkYellow;

    QTextCharFormat format;
    format.setForeground(color);

    QTextCursor cursor = ui->txtEditLog->textCursor();
    cursor.movePosition(QTextCursor::End);
    cursor.insertText(QString("[Warn %1]: %2\n").arg(TIMESTAMP(), log), format);

    ui->txtEditLog->moveCursor(QTextCursor::End);
}

void WidgetLog::logError(const QString &log)
{
    QColor color = Qt::red;

    QTextCharFormat format;
    format.setForeground(color);

    QTextCursor cursor = ui->txtEditLog->textCursor();
    cursor.movePosition(QTextCursor::End);
    cursor.insertText(QString("[Error %1]: %2\n").arg(TIMESTAMP(), log), format);

    ui->txtEditLog->moveCursor(QTextCursor::End);
}

void WidgetLog::on_btnHide_clicked()
{
    emit signalHide();
}

void WidgetLog::on_btnClear_clicked() { ui->txtEditLog->clear(); }
