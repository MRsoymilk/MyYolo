#ifndef WIDGETTEST_H
#define WIDGETTEST_H

#include <QLineEdit>
#include <QWidget>

namespace Ui {
class WidgetTest;
}

class WidgetTest : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetTest(QWidget *parent = nullptr);
    ~WidgetTest();

private slots:
    void on_tBtnModel_triggered(QAction *arg1);

    void on_btnStartTest_clicked();
    void openFile(QLineEdit *edit);
    void openFolder(QLineEdit *edit);

private:
    Ui::WidgetTest *ui;
    void initTest();
};

#endif // WIDGETTEST_H
