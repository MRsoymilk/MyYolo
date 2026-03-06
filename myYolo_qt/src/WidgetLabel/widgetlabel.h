#ifndef WIDGETLABEL_H
#define WIDGETLABEL_H

#include <QProcess>
#include <QWidget>

namespace Ui
{
class WidgetLabel;
}

class WidgetLabel : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetLabel(QWidget *parent = nullptr);
    ~WidgetLabel();
    struct ini_label
    {
        QString path_labelimg;
    };

private:
    void initLabel();

private slots:
    void on_btnStartLabel_clicked();

private:
    Ui::WidgetLabel *ui;
    ini_label m_data;
};

#endif  // WIDGETLABEL_H
