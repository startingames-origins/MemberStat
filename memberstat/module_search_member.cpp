#include "module_search_member.h"

module_search_member::module_search_member(QWidget *_parent) : QWidget(_parent)
{
    QLabel *regNumLabel = new QLabel(tr("Registration Number"), this);
    QLabel *genderLabel = new QLabel(tr("Gender"), this);
    QLabel *memberTypeLabel = new QLabel(tr("Member Type"), this);
    QLabel *firstNameLabel = new QLabel(tr("First Name"), this);
    QLabel *lastNameLabel = new QLabel(tr("Last Name"), this);
    QLabel *ageLabel = new QLabel(tr("< Age <"), this);

    regNum = new QLineEdit(this);
    gender = new QComboBox(this);
    memberType = new QComboBox(this);
    firstName = new QLineEdit(this);
    otherName = new QCheckBox(tr("Search also in Other Name"), this);
    lastName = new QLineEdit(this);
    autoLast = new QCheckBox(tr("Same as First Name"), this);
        connect(autoLast, SIGNAL(stateChanged(int)), this, SLOT(autoLastChanged(int)));
        autoLast->setChecked(true);
    ageMin = new QSpinBox(this);
        ageMin->setRange(0, 1000);
        ageMin->setValue(0);
    ageMax = new QSpinBox(this);
        ageMin->setRange(0, 1000);
        ageMin->setValue(0);

    result = new QLabel(tr("Ready"), this);
        result->setStyleSheet("font-weight: bold;");
    search = new QPushButton(QIcon(ICON_SEARCH), tr("Search"), this);
        connect(search, SIGNAL(released()), this, SLOT(searchExec()));
    table = new QTableWidget(0, 10, this);
        table->setAlternatingRowColors(true);
        table->setStyleSheet("alternate-background-color: rgb(210,240,250);");
        table->setColumnHidden(0, true);
        table->verticalHeader()->setHidden(true);
        table->setSelectionMode(QAbstractItemView::SingleSelection);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        QStringList horizontalHeaders;
        horizontalHeaders << "id" << tr("Reg Num") << tr("First Name") << tr("Other Name") << tr("Last Name") << tr("Age") << tr("Birth Date") << tr("Birth Location") << tr("Member Type") << tr("Gender");
        table->setHorizontalHeaderLabels(horizontalHeaders);
        table->resizeColumnsToContents();
        table->sortByColumn(1, Qt::AscendingOrder);
        table->verticalHeader()->setDefaultSectionSize(40);
        //connect(table, SIGNAL(itemSelectionChanged()), this, SLOT(versionSelChange()));
        connect(table, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(openMember(QTableWidgetItem*)));

    QHBoxLayout *topLayout = new QHBoxLayout();
        topLayout->addWidget(regNumLabel);
        topLayout->addWidget(regNum);
        topLayout->addWidget(genderLabel);
        topLayout->addWidget(gender);
        topLayout->addWidget(memberTypeLabel);
        topLayout->addWidget(memberType);

    QHBoxLayout *bottomLayout = new QHBoxLayout();
        bottomLayout->addWidget(ageMin);
        bottomLayout->addWidget(ageLabel);
        bottomLayout->addWidget(ageMax);
            QSpacerItem *bottomSapcer = new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
        bottomLayout->addItem(bottomSapcer);
        bottomLayout->addWidget(search);

    QGridLayout *mainLayout = new QGridLayout(this);
        mainLayout->addLayout(topLayout, 0, 0, 1, 3);
        mainLayout->addWidget(firstNameLabel, 1, 0);
        mainLayout->addWidget(firstName, 1, 1);
        mainLayout->addWidget(otherName, 1, 2);
        mainLayout->addWidget(lastNameLabel, 2, 0);
        mainLayout->addWidget(lastName, 2, 1);
        mainLayout->addWidget(autoLast, 2, 2);
        mainLayout->addLayout(bottomLayout, 3, 0, 1, 3);
        mainLayout->addWidget(result, 4, 0, 1, 3);
        mainLayout->addWidget(table, 5, 0, 1, 3);

    setLayout(mainLayout);

    user::updateDB();

    gender->addItem("", -1);
    for(int i=0; i<user::gender_text.size(); i++)
    {
        gender->addItem(user::gender_text.at(i), user::gender_id.at(i));
    }

    memberType->addItem("", -1);
    for(int i=0; i<user::type_text.size(); i++)
    {
        memberType->addItem(user::type_text.at(i), user::type_id.at(i));
    }
}

void module_search_member::autoLastChanged(int newValue)
{
    if(newValue)    { lastName->setDisabled(true); }
    else            { lastName->setDisabled(false); }
}

void module_search_member::openMember(QTableWidgetItem *item)
{
    emit doubleCliked(table->item(item->row(), 0)->text().toUInt());
}

void module_search_member::searchExec()
{
    result->setText(tr("Searching..."));
    int count = 0;
    table->setSortingEnabled(false);
    table->setRowCount(0);

    uint8_t flags=0;
    if(!regNum->text().isEmpty()) { flags+=1; }
    if(gender->currentData().toInt()!=-1) { flags+=2; }
    if(memberType->currentData().toInt()!=-1) { flags+=4; }
    if(!firstName->text().isEmpty()) { flags+=8; }
    if(otherName->isChecked()) { flags+=16; }
    if(!lastName->text().isEmpty() && autoLast->isChecked()) { flags+=32; }
    if(ageMin->value()>0) { flags+=64; }
    if(ageMax->value()>0) { flags+=128; }

    QString queryString = "SELECT * FROM members";

    if(flags>0)
    {
        queryString+=" WHERE ";
        if(flags&1)
        {
            queryString+= "regNum LIKE '%"+engine_db::echap(regNum->text())+"%'";
            flags-=1;
            if(flags>0) { queryString+=" AND "; }
        }
        if(flags&2)
        {
            queryString+= "gender="+QString::number(gender->currentData().toInt());
            flags-=2;
            if(flags>0) { queryString+=" AND "; }
        }
        if(flags&4)
        {
            queryString+= "type="+QString::number(memberType->currentData().toInt());
            flags-=4;
            if(flags>0) { queryString+=" AND "; }
        }
        if(flags&(8+16+32)) { queryString+="( "; }
        if(flags&8)
        {
            queryString+= "firstName LIKE '%"+engine_db::echap(firstName->text())+"%'";
            if(flags&(16+32)) { queryString+=" OR "; }
        }
        if(flags&16)
        {
            queryString+= "otherName LIKE '%"+engine_db::echap(firstName->text())+"%'";
            if(flags&32) { queryString+=" OR "; }
        }
        if(flags&32)
        {
            if(autoLast->isChecked())
            {
                queryString+="lastName LIKE '%"+engine_db::echap(firstName->text())+"%'";
            }
            else
            {
                queryString+="lastName LIKE '%"+engine_db::echap(lastName->text())+"%'";
            }
        }
        if(flags&(8+16+32))
        {
            queryString+=" )";
            flags=flags&0b11000111;
            if(flags>0) { queryString+=" AND "; }
        }
        if(flags&64)
        {
            queryString+= "TIMESTAMPDIFF(YEAR, birthDate, CURDATE()) > "+QString::number(ageMin->value());
            flags-=64;
            if(flags&128) { queryString+=" AND "; }
        }
        if(flags&128)
        {
            queryString+= "TIMESTAMPDIFF(YEAR, birthDate, CURDATE()) < "+QString::number(ageMax->value());
            flags-=128;
        }
    }

    user::updateDB();

    QSqlQuery query;
    query.prepare(queryString);
    if(query.exec())
    {
       while(query.next())
        {
            table->setRowCount(count+1);
            table->setItem(count, 0, new QTableWidgetItem(query.record().value("id").toString()));
            table->setItem(count, 1, new QTableWidgetItem(query.record().value("regNum").toString()));
            table->setItem(count, 2, new QTableWidgetItem(query.record().value("firstName").toString()));
            table->setItem(count, 3, new QTableWidgetItem(query.record().value("otherName").toString()));
            table->setItem(count, 4, new QTableWidgetItem(query.record().value("lastName").toString()));
            table->setItem(count, 5, new QTableWidgetItem(QString::number(global::age(query.record().value("birthDate").toDate()))));
            table->setItem(count, 6, new QTableWidgetItem(query.record().value("birthDate").toDate().toString()));
            table->setItem(count, 7, new QTableWidgetItem(query.record().value("birthLocation").toString()));
            table->setItem(count, 8, new QTableWidgetItem(query.record().value("type").toString()));
            int temp = user::type_id.indexOf(query.record().value("type").toInt());
            if(temp<1) { temp=0; }
            table->setItem(count, 8, new QTableWidgetItem(user::type_text.at(temp)));
            temp = user::gender_id.indexOf(query.record().value("gender").toInt());
            if(temp<1) { temp=0; }
            table->setItem(count, 9, new QTableWidgetItem(user::gender_text.at(temp)));
            count++;
        }
    }
    else
    {
        engine_db::dispERR(this, query);
    }

    result->setText(tr("%n Result(s) Found", "", count));
    table->resizeColumnsToContents();
    table->sortByColumn(2, Qt::AscendingOrder);
    table->setSortingEnabled(true);
}
