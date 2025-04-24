#include "mainwindow.h"

#include <QFileDialog>
#include <QTranslator>

#include "./ui_mainwindow.h"
#include "WidgetAddData/widgetadddata.h"
#include "WidgetExport/widgetexport.h"
#include "WidgetFilter/widgetfilter.h"
#include "WidgetLabel/widgetlabel.h"
#include "WidgetLog/widgetlog.h"
#include "WidgetMain/Info/widgetmaininfo.h"
#include "WidgetMain/Setting/widgetmainsetting.h"
#include "WidgetTest/widgettest.h"
#include "WidgetTrain/widgettrain.h"
#include "include/funcdef.h"
#include "include/keydef.h"
#include "version.h"

MY_GLOBAL GLOBAL;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusbar->showMessage(QString("version: %1").arg(APP_VERSION));
    initMain();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::initMain()
{
    addBasicWidget();
    for (QAction *action : ui->menuTheme->actions())
    {
        connect(action, &QAction::triggered, this, [this, action]() { menuThemeSelect(action); });
    }
    for (QAction *action : ui->menulanguage->actions())
    {
        connect(action, &QAction::triggered, this, [this, action]() { menuLanguageSelect(action); });
    }
    getCfgData();
    show2Ui();
}

void MainWindow::addBasicWidget()
{
    WidgetMainInfo *widgetMainInfo = new WidgetMainInfo();
    WidgetMainSetting *widgetMainSetting = new WidgetMainSetting();
    WidgetFilter *widgetFilter = new WidgetFilter();
    WidgetAddData *widgetAddData = new WidgetAddData();
    WidgetTrain *widgetTrain = new WidgetTrain();
    WidgetTest *widgetTest = new WidgetTest();
    WidgetExport *widgetExport = new WidgetExport();
    WidgetLabel *widgetLabel = new WidgetLabel();
    ui->gLayLabel->addWidget(widgetLabel);
    ui->gLayAddData->addWidget(widgetAddData);
    ui->gLayFilter->addWidget(widgetFilter);
    ui->gLayTrain->addWidget(widgetTrain);
    ui->gLayTest->addWidget(widgetTest);
    ui->gLayExport->addWidget(widgetExport);
    ui->gLayInfo->addWidget(widgetMainInfo);
    ui->gLaySetting->addWidget(widgetMainSetting);
    connect(&MY_WIDGET_LOG, &WidgetLog::signalHide, this, &MainWindow::slotWidgetTestHide);
}

void MainWindow::menuThemeSelect(QAction *selectedAction)
{
    for (QAction *theme : ui->menuTheme->actions())
    {
        if (theme == selectedAction && theme->isChecked())
        {
            return;
        }
        theme->setChecked(false);
        theme->setIcon(QIcon());
    }
    selectedAction->setChecked(true);
    selectedAction->setIcon(QIcon(":res/icons/yes.svg"));
    setTheme(selectedAction->text());
}

void MainWindow::setTheme(const QString &theme)
{
    m_data.theme = theme;
    save2Cfg();
    QFile file(QString(":/res/themes/%1.qss").arg(theme));
    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QString style = file.readAll();
        qApp->setStyleSheet(style);
    }
}

void MainWindow::menuLanguageSelect(QAction *selectedAction)
{
    for (QAction *language : ui->menulanguage->actions())
    {
        if (language == selectedAction && language->isChecked())
        {
            return;
        }
        language->setChecked(false);
        language->setIcon(QIcon());
    }
    selectedAction->setChecked(true);
    selectedAction->setIcon(QIcon(":res/icons/yes.svg"));
    setLanguage(selectedAction->text());
}

void MainWindow::setLanguage(const QString &language)
{
    m_data.language = language;
    save2Cfg();
    QTranslator *translator = new QTranslator(this);
    if (translator->load(QString(":/res/i18n/%1.qm").arg(language)))
    {
        qApp->installTranslator(translator);
        ui->retranslateUi(this);
    }
}

void MainWindow::getCfgData()
{
    m_data.theme = SETTING_GET(CFG_GROUP_MAIN, CFG_MAIN_THEME);
    m_data.language = SETTING_GET(CFG_GROUP_MAIN, CFG_MAIN_LANGUAGE);
}

void MainWindow::save2Cfg()
{
    SETTING_SET(CFG_GROUP_MAIN, CFG_MAIN_THEME, m_data.theme);
    SETTING_SET(CFG_GROUP_MAIN, CFG_MAIN_LANGUAGE, m_data.language);
}

void MainWindow::show2Ui()
{
    if (m_data.language.isEmpty())
    {
        ui->actionen->trigger();
        WIDGET_LOG_INFO("use default language: en");
    }
    else
    {
        for (QAction *language : ui->menulanguage->actions())
        {
            if (language->text() == m_data.language)
            {
                language->trigger();
                WIDGET_LOG_INFO(QString("use default language: %1").arg(language->text()));
                break;
            }
        }
    }
    if (m_data.theme.isEmpty())
    {
        ui->actionLite->trigger();
        WIDGET_LOG_INFO("use default theme: Lite");
    }
    else
    {
        for (QAction *theme : ui->menuTheme->actions())
        {
            if (theme->text() == m_data.theme)
            {
                theme->trigger();
                WIDGET_LOG_INFO(QString("use default theme: %1").arg(theme->text()));
                break;
            }
        }
    }
}

void MainWindow::getUiData()
{
    for (QAction *language : ui->menulanguage->actions())
    {
        if (language->isChecked())
        {
            m_data.language = language->text();
            break;
        }
    }
    for (QAction *theme : ui->menuTheme->actions())
    {
        if (theme->isChecked())
        {
            m_data.language = theme->text();
            break;
        }
    }
}

void MainWindow::on_checkBoxShowLog_checkStateChanged(const Qt::CheckState &state)
{
    WIDGET_LOG_INFO("show log click");
    if (state == Qt::Checked)
    {
        MY_WIDGET_LOG.show();
        WIDGET_LOG_INFO("show log view");
    }
    else if (state == Qt::Unchecked)
    {
        MY_WIDGET_LOG.hide();
        WIDGET_LOG_INFO("hide log view");
    }
}

void MainWindow::slotWidgetTestHide() { ui->checkBoxShowLog->setChecked(false); }

void MainWindow::closeEvent(QCloseEvent *event)
{
    WIDGET_LOG_CLOSE();
    QMainWindow::closeEvent(event);
}
