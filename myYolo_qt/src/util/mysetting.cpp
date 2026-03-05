#include "mysetting.h"

MySetting &MySetting::getInstance()
{
    static MySetting c_setting;
    return c_setting;
}

MySetting::~MySetting() {}

void MySetting::init(const QString &path) { m_settings = new QSettings(path, QSettings::IniFormat); }

void MySetting::setValue(const QString &group, const QString &key, const QString &val)
{
    m_settings->setValue(QString("%1/%2").arg(group, key), val);
}

QString MySetting::getValue(const QString &group, const QString &key, const QString &val_dft)
{
    QString val = m_settings->value(QString("%1/%2").arg(group, key)).toString();
    if (val.isEmpty())
    {
        val = val_dft;
        m_settings->setValue(QString("%1/%2").arg(group, key), val_dft);
    }
    return val;
}

MySetting::MySetting() {}
