#ifndef WIDGETEXPORT_H
#define WIDGETEXPORT_H

#include <QProcess>
#include <QWidget>

namespace Ui
{
class WidgetExport;
}

class WidgetExport : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetExport(QWidget *parent = nullptr);
    ~WidgetExport();
    struct ini_export
    {
        QStringList list_type = {};
        QString type = "";
        QString pt_model = "";
        int model_h;
        int model_w;
        int model_batch;
    };

private slots:
    void initExport();
    void on_btnStartExport_clicked();
    void on_btnAddType_clicked();
    void on_comboBoxType_currentTextChanged(const QString &type);

private:
    Ui::WidgetExport *ui;
    ini_export m_data;
};

#endif  // WIDGETEXPORT_H
