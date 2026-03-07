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
#include "WidgetDeploy/widgetdeploy.h"
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
        action->setCheckable(true);
        connect(action, &QAction::triggered, this, [this, action]() { menuThemeSelect(action); });
    }
    for (QAction *action : ui->menulanguage->actions())
    {
        action->setCheckable(true);
        connect(action, &QAction::triggered, this, [this, action]() { menuLanguageSelect(action); });
    }
    m_data.theme = SETTING_GET(CFG_GROUP_MAIN, CFG_MAIN_THEME);
    m_data.language = SETTING_GET(CFG_GROUP_MAIN, CFG_MAIN_LANGUAGE);
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
                language->setChecked(true);
                setLanguage(language->text());
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
                theme->setChecked(true);
                setTheme(theme->text());
                WIDGET_LOG_INFO(QString("use default theme: %1").arg(theme->text()));
                break;
            }
        }
    }
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
    WidgetDeploy *widgetDeploy = new WidgetDeploy();
    ui->gLayLabel->addWidget(widgetLabel);
    ui->gLayAddData->addWidget(widgetAddData);
    ui->gLayFilter->addWidget(widgetFilter);
    ui->gLayTrain->addWidget(widgetTrain);
    ui->gLayTest->addWidget(widgetTest);
    ui->gLayExport->addWidget(widgetExport);
    ui->gLayInfo->addWidget(widgetMainInfo);
    ui->gLayDeploy->addWidget(widgetDeploy);
    ui->gLaySetting->addWidget(widgetMainSetting);
    connect(&MY_WIDGET_LOG, &WidgetLog::signalHide, this, &MainWindow::slotWidgetTestHide);
    connect(&MY_WIDGET_LOG, &WidgetLog::windowClose, this, [this]() { ui->checkBoxShowLog->setChecked(false); });
}

void MainWindow::menuThemeSelect(QAction *selectedAction)
{
    for (QAction *theme : ui->menuTheme->actions())
    {
        theme->setChecked(false);
    }
    selectedAction->setChecked(true);
    setTheme(selectedAction->text());
}

void MainWindow::setTheme(const QString &theme)
{
    m_data.theme = theme;
    SETTING_SET(CFG_GROUP_MAIN, CFG_MAIN_THEME, m_data.theme);
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
        language->setChecked(false);
    }
    selectedAction->setChecked(true);
    setLanguage(selectedAction->text());
}

void MainWindow::setLanguage(const QString &language)
{
    m_data.language = language;
    SETTING_SET(CFG_GROUP_MAIN, CFG_MAIN_LANGUAGE, m_data.language);
    QTranslator *translator = new QTranslator(this);
    if (translator->load(QString(":/res/i18n/%1.qm").arg(language)))
    {
        qApp->installTranslator(translator);
        ui->retranslateUi(this);
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
