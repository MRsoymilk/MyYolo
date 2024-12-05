#include "widgetmaininfo.h"

#include "ui_widgetmaininfo.h"

WidgetMainInfo::WidgetMainInfo(QWidget *parent) : QWidget(parent), ui(new Ui::WidgetMainInfo) { ui->setupUi(this); }

WidgetMainInfo::~WidgetMainInfo() { delete ui; }
