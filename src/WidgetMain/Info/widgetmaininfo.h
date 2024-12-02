#ifndef WIDGETMAININFO_H
#define WIDGETMAININFO_H

#include <QWidget>

namespace Ui {
class WidgetMainInfo;
}

class WidgetMainInfo : public QWidget {
  Q_OBJECT

 public:
  explicit WidgetMainInfo(QWidget *parent = nullptr);
  ~WidgetMainInfo();

 private:
  Ui::WidgetMainInfo *ui;
};

#endif  // WIDGETMAININFO_H
