#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "WidgetAddData/widgetadddata.h"
#include "WidgetExport/widgetexport.h"
#include "WidgetFilter/widgetfilter.h"
#include "WidgetLabel/widgetlabel.h"
#include "WidgetMain/Info/widgetmaininfo.h"
#include "WidgetMain/Setting/widgetmainsetting.h"
#include "WidgetTest/widgettest.h"
#include "WidgetTrain/widgettrain.h"

#include <QFileDialog>
#include <QTranslator>

#include "include/funcdef.h"
#include "include/keydef.h"
#include "util/mysetting.h"

#include "util/mylog.h"
#include "util/mydir.h"

MY_GLOBAL GLOBAL;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initMain();
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::initMain() {
    addBasicWidget();
    for (QAction* action : ui->menuTheme->actions()) {
        connect(action, &QAction::triggered, this, [this, action]() {
            menuThemeSelect(action);
        });
    }
    ui->actionLite->trigger();

    for(QAction* action : ui->menulanguage->actions()) {
        connect(action, &QAction::triggered, this, [this, action]() {
            menuLanguageSelect(action);
        });
    }
    ui->actionen->trigger();
}

void MainWindow::addBasicWidget() {
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
}

void MainWindow::menuThemeSelect(QAction* selectedAction) {
    for(QAction* theme : ui->menuTheme->actions()) {
        if (theme == selectedAction && theme->isChecked()) {
            return;
        }
        theme->setChecked(false);
        theme->setIcon(QIcon());
    }
    selectedAction->setChecked(true);
    selectedAction->setIcon(QIcon(":res/icons/yes.svg"));
    setTheme(selectedAction->text());
}

void MainWindow::setTheme(const QString& theme)
{
    QFile file(QString(":/res/themes/%1.qss").arg(theme));
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QString style = file.readAll();
        qApp->setStyleSheet(style);
    }
}

void MainWindow::menuLanguageSelect(QAction *selectedAction) {
    for(QAction* language : ui->menulanguage->actions()) {
        if (language == selectedAction && language->isChecked()) {
            return;
        }
        language->setChecked(false);
        language->setIcon(QIcon());
    }
    selectedAction->setChecked(true);
    selectedAction->setIcon(QIcon(":res/icons/yes.svg"));
    setLanguage(selectedAction->text());
}

void MainWindow::setLanguage(const QString& language)
{
    QTranslator* translator = new QTranslator(this);
    if (translator->load(QString(":/res/i18n/%1.qm").arg(language))) {
        qApp->installTranslator(translator);
        ui->retranslateUi(this);
    }
}
