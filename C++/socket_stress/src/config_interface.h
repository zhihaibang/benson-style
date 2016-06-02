/*
* =====================================================================================
*
* @Filename: config_interface.h
* @Description: 读配置接口文件
*
* =====================================================================================
*/

#ifndef __CONFIG_INTERFACE_H__
#define __CONFIG_INTERFACE_H__

#include "IConfig.h"

#define CONFIG_LOAD(path) IConfig::instance()->loadConfigFile(path)
#define CONFIG_FREE() IConfig::instance()->free()
#define CONFIG_GETSTR(section, item) IConfig::instance()->getConfigStr(section, item)
#define CONFIG_GETINT(section, item) IConfig::instance()->getConfigInt(section, item)

#endif 

