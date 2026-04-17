#ifndef WIDGETFILTER_H
#define WIDGETFILTER_H

#include <QWidget>

namespace Ui
{
class WidgetFilter;
}

class WidgetFilter : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetFilter(QWidget *parent = nullptr);
    ~WidgetFilter();
    struct ini_filter
    {
        QString dir_input = "";
        QString dir_output = "";
        double threshold_ssim = 0.9;
        int batch_ssim = 10;
    };

private:
    void initFilter();

private slots:
    void on_btnStartFilter_clicked();

private:
    Ui::WidgetFilter *ui;
    ini_filter m_data;
};

#endif  // WIDGETFILTER_H
