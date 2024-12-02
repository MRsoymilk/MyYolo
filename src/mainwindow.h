#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "common/confighandler.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow, ConfigHandler {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  struct ini_main {
    QString path = "";
    QString theme;
    QString language;
  };

 private:
  void addBasicWidget();
  void initMain();

 private:
  Ui::MainWindow *ui;
  ini_main m_data;
  void menuThemeSelect(QAction *selectedAction);
  void setTheme(const QString &theme);
  void menuLanguageSelect(QAction *selectedAction);
  void setLanguage(const QString &language);

  // ConfigHandler interface
 public:
  void getCfgData() override;
  void save2Cfg() override;
  void show2Ui() override;
  void getUiData() override;
 private slots:
  void on_checkBoxShowLog_checkStateChanged(const Qt::CheckState &arg1);
  void slotWidgetTestHide();

  // QWidget interface
 protected:
  void closeEvent(QCloseEvent *event) override;
};
#endif  // MAINWINDOW_H
