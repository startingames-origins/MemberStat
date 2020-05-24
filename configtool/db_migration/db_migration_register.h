#ifndef DB_MIGRATION_REGISTER_H
#define DB_MIGRATION_REGISTER_H

//Add include for your migration
#include "db_migration_0.h"
#include "db_migration_1.h"
#include "db_migration_2.h"

//Add your migration
//          addMigration(new [yourclass]()); //Please add an '\' if not the last add
#define DB_MIGRATION_REGISTER_ADD \
    addMigration(new db_migration_0()); \
    addMigration(new db_migration_1()); \
    addMigration(new db_migration_2());
#endif // DB_MIGRATION_REGISTER_H
