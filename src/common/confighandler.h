#ifndef CONFIGHANDLER_H
#define CONFIGHANDLER_H

class ConfigHandler {
 public:
  ConfigHandler() = default;
  ~ConfigHandler() = default;
  virtual void getCfgData() = 0;
  virtual void save2Cfg() = 0;
  virtual void show2Ui() = 0;
  virtual void getUiData() = 0;
};

#endif  // CONFIGHANDLER_H
