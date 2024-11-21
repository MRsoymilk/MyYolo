#include "widgetfilter.h"
#include "ui_widgetfilter.h"

#include "../include/keydef.h"

WidgetFilter::WidgetFilter(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetFilter)
{
    ui->setupUi(this);
    initFilter();
}

WidgetFilter::~WidgetFilter()
{
    delete ui;
}

void WidgetFilter::initFilter() {}

void WidgetFilter::getCfgData() {}

void WidgetFilter::save2Cfg() {}

void WidgetFilter::show2Ui() {}

void WidgetFilter::getUiData() {}
