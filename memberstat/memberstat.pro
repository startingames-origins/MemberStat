QT += core
QT += widgets
QT += sql

TRANSLATIONS = memberstat_fr.ts

SOURCES += \
    ../common/engine_settings.cpp \
    ../common/engine_db.cpp \
    global.cpp \
    main.cpp \
    manager.cpp \
    member_gender.cpp \
    module_info_spetable.cpp \
    module_search_event.cpp \
    module_search_member.cpp \
    module_select_display.cpp \
    spoilerwidget.cpp \
    user.cpp \
    window_config_gentype.cpp \
    window_event.cpp \
    window_login.cpp \
    window_main.cpp \
    window_member.cpp \
    window_search_event.cpp \
    window_search_member.cpp \
    window_user_rights.cpp \
    window_users.cpp

HEADERS += \
    ../common/common.h \
    ../common/engine_settings.h \
    ../common/icons.h \
    ../common/engine_db.h \
    global.h \
    main.h \
    manager.h \
    member_gender.h \
    module_info_spetable.h \
    module_search_event.h \
    module_search_member.h \
    module_select_display.h \
    spoilerwidget.h \
    user.h \
    window_config_gentype.h \
    window_event.h \
    window_login.h \
    window_main.h \
    window_member.h \
    window_search_event.h \
    window_search_member.h \
    window_user_rights.h \
    window_users.h

RESOURCES += \
    qressources.qrc

DISTFILES += \
    version.txt

RC_ICONS = icon.ico
