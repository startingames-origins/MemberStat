
#ifndef COMMON_H
#define COMMON_H

#include "icons.h"

#define APP_SETTINGS "settings.ini"
#define APP_FILEFLAG_DESKTOPINSTALL "desktopMode.flag"
#define APP_FONT    "resources/memberstat.TTF"
#define APP_TRANSLATION_FOLDER "resources/translations"

#define APP_LICENSE         "resources/license.txt"
#define APP_LICENSE_BRAND   "resources/license-startingames.txt"

#define APP_DB_VERSION  2

#define USER_PASSWORD_MACRO
#define USER_PASSWORD_MACRO2    userpass
#define SETTINGS_PASSWORD_MACRO password
#define SETTINGS_ENCRYPT_MACRO  QByteArray dataCrypt = QByteArray(data)
#define SETTINGS_DECRYPT_MACRO  QByteArray data = QByteArray(dataCrypt)

#if defined __has_include
#  if __has_include ("version.h")
#    include "version.h"
#  else
#    define APP_VER "\"Version.h Missing\""
#  endif
#endif

#if defined __has_include
#  if __has_include ("secret/secret.h")
#    include "secret/secret.h"
#    define APP_VERSION APP_VER
#  else
#    define DEV_MODE_ENABLE
#    define APP_VERSION APP_VER" - DEV MODE"
#  endif
#endif

#endif //COMMON_H
