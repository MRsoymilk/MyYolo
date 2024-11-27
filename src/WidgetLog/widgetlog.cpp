#include "widgetlog.h"
#include "ui_widgetlog.h"
#include "../include/funcdef.h"

WidgetLog::WidgetLog(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetLog)
{
    ui->setupUi(this);
}

WidgetLog &WidgetLog::getInstance()
{
    static WidgetLog instance;
    return instance;
}

WidgetLog::~WidgetLog()
{
    delete ui;
}

void WidgetLog::log(const QString &log)
{
    ui->txtEditLog->append(log);
}

void WidgetLog::logTrace(const QString &log)
{
    QColor color = Qt::black;

    QTextCharFormat format;
    format.setForeground(color);

    QTextCursor cursor = ui->txtEditLog->textCursor();
    cursor.movePosition(QTextCursor::End);
    cursor.insertText(QString("[Trace %1]: %2\n").arg(TIMESTAMP(), log), format);

    ui->txtEditLog->moveCursor(QTextCursor::End);
}

void WidgetLog::logInfo(const QString &log)
{
    QColor color = Qt::darkBlue;

    QTextCharFormat format;
    format.setForeground(color);

    QTextCursor cursor = ui->txtEditLog->textCursor();
    cursor.movePosition(QTextCursor::End);
    cursor.insertText(QString("[Info %1]: %2\n").arg(TIMESTAMP(), log), format);

    ui->txtEditLog->moveCursor(QTextCursor::End);
}

void WidgetLog::logWarn(const QString &log) {
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
    this->hide();
}


void WidgetLog::on_btnClear_clicked()
{
    ui->txtEditLog->clear();
}
