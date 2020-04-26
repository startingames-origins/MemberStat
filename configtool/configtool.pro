QT += core
QT += widgets
QT += sql

QMAKE_LFLAGS_WINDOWS += "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\""

TRANSLATIONS = configtool_fr.ts

SOURCES += \
    ../common/engine_db.cpp \
    ../common/engine_settings.cpp \
    db_migration/db_migration_engine.cpp \
    main.cpp \
    window_wizard.cpp

HEADERS += \
    ../common/common.h \
    ../common/engine_db.h \
    ../common/engine_settings.h \
    ../common/icons.h \
    db_migration/db_migration_0.h \
    db_migration/db_migration_1.h \
    db_migration/db_migration_engine.h \
    db_migration/db_migration_generic.h \
    db_migration/db_migration_register.h \
    main.h \
    window_wizard.h

RESOURCES += \
    qressources.qrc

FORMS += \
    widget_database1.ui \
    widget_database2.ui \
    widget_database3.ui \
    widget_endConfiguration1.ui \
    widget_settings1.ui \
    widget_welcome1.ui \
    widget_welcome2.ui \
    window_wizard.ui
