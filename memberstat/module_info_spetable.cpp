#include "module_info_spetable.h"

module_info_spetable::module_info_spetable(QStringList _names, QStringList _namesDB, QString _tableDB, QString _relidNameDB, int _relid, QWidget *_parent, bool _displayMsgAutoSave) : QWidget(_parent)
{
    names=_names;
    namesDB=_namesDB;
    tableDB=_tableDB;
    relidNameDB=_relidNameDB;
    relid=_relid;

    button_add = new QPushButton(QIcon(ICON_PLUS), tr("Add"), this);
        connect(button_add, SIGNAL(released()), this, SLOT(add()));
    button_edit = new QPushButton(QIcon(ICON_EDIT), tr("Edit"), this);
        connect(button_edit, SIGNAL(released()), this, SLOT(edit()));
    button_remove = new QPushButton(QIcon(ICON_TIME), tr("Remove"), this);
        connect(button_remove, SIGNAL(released()), this, SLOT(remove()));
    result = new QLabel(this);
        result->setStyleSheet("font-weight: bold;");
    table = new QTableWidget(0, names.count()+1, this);
        table->setAlternatingRowColors(true);
        table->setStyleSheet("alternate-background-color: rgb(210,240,250);");
        table->setColumnHidden(0, true);
        table->verticalHeader()->setHidden(true);
        table->setSelectionMode(QAbstractItemView::SingleSelection);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        QStringList horizontalHeaders; horizontalHeaders << "id";
        horizontalHeaders.append(names);
        table->setHorizontalHeaderLabels(horizontalHeaders);
        table->resizeColumnsToContents();
        table->sortByColumn(1, Qt::AscendingOrder);
        table->setSortingEnabled(true);
        table->verticalHeader()->setDefaultSectionSize(40);
        table->setMinimumHeight(0);
        connect(table, SIGNAL(itemSelectionChanged()), this, SLOT(selChange()));
        //connect(table, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(openMember(QTableWidgetItem*)));*

    QLabel *labelAutoSave = new QLabel(tr("Actions in this group are auto-saved !"), this);
    QGridLayout *layout = new QGridLayout(this);
        if(_displayMsgAutoSave)
        { layout->addWidget(labelAutoSave, 0, 0, 1, 4); }
        else { labelAutoSave->setVisible(false); }
        layout->addWidget(result, 1, 0);
        layout->addWidget(button_add, 1, 1);
        layout->addWidget(button_edit, 1, 2);
        layout->addWidget(button_remove, 1, 3);
        layout->addWidget(table, 2, 0, 1, 4);
    setLayout(layout);

    selChange();

//Setup bwin Dialog
    bwin_addedit = new QDialog(this);
        bwin_addedit_cancel = new QPushButton(tr("Cancel"), bwin_addedit);
            connect(bwin_addedit_cancel, SIGNAL(released()), bwin_addedit, SLOT(close()));
        bwin_addedit_confirm = new QPushButton("", bwin_addedit);
            bwin_addedit_confirm_connection = connect(bwin_addedit_confirm, SIGNAL(released()), bwin_addedit, SLOT(close()));
        bwin_addedit_spacer = new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
        bwin_addedit_layout_button = new QHBoxLayout();
            bwin_addedit_layout_button->addItem(bwin_addedit_spacer);
            bwin_addedit_layout_button->addWidget(bwin_addedit_confirm);
            bwin_addedit_layout_button->addWidget(bwin_addedit_cancel);
        bwin_addedit_layout = new QGridLayout();
            bwin_addedit_layout->addLayout(bwin_addedit_layout_button, 2, 0, 1, names.count());
        for(int i=0; i<names.count(); i++)
        {
            bwin_addedit_labels.append(new QLabel(names.at(i), bwin_addedit));
            bwin_addedit_ledits.append(new QLineEdit(bwin_addedit));
            bwin_addedit_ledits.at(i)->setMaxLength(250);
            bwin_addedit_layout->addWidget(bwin_addedit_labels.at(i), 0, i);
            bwin_addedit_layout->addWidget(bwin_addedit_ledits.at(i), 1, i);
        }
    bwin_addedit->setLayout(bwin_addedit_layout);
}

QStringList module_info_spetable::getNames()
{
    return names;
}

void module_info_spetable::update()
{
    result->setText(tr("Searching..."));
    int count = 0;
    table->setSortingEnabled(false);
    table->setRowCount(0);

    QString queryText = "SELECT id";
    for(int i=0; i<namesDB.count(); i++)
    {
        queryText+=", "+namesDB.at(i);
    }
    queryText+=" FROM "+tableDB+" WHERE "+relidNameDB+" = "+QString::number(relid);
    QSqlQuery query;
    query.prepare(queryText);
    if(query.exec())
    {
       while(query.next())
        {
            table->setRowCount(count+1);
            table->setItem(count, 0, new QTableWidgetItem(query.record().value("id").toString()));
            for(int i=0; i<namesDB.count(); i++)
            {
                table->setItem(count, i+1, new QTableWidgetItem(query.record().value(namesDB.at(i)).toString()));
            }
            count++;
        }
    }
    else
    {
        engine_db::dispERR(this, query);
    }

    result->setText(tr("%n Result(s) Found", "", count));
    table->resizeColumnsToContents();
    table->resizeRowsToContents();
    //table->sortByColumn(2, Qt::AscendingOrder);
    table->setSortingEnabled(true);
}

void module_info_spetable::selChange()
{
    if(table->selectedItems().isEmpty())
    {
        button_edit->setEnabled(false);
        button_remove->setEnabled(false);
    }
    else
    {
        button_edit->setEnabled(true);
        button_remove->setEnabled(true);
    }
}

void module_info_spetable::add()
{
    disconnect(bwin_addedit_confirm_connection);
    bwin_addedit_confirm_connection = connect(bwin_addedit_confirm, SIGNAL(released()), this, SLOT(add_exec()));
    bwin_addedit_confirm->setText(tr("Add"));
    bwin_addedit_confirm->setDefault(true);
    bwin_addedit_ledits.at(0)->setFocus();
    for(int i=0; i<bwin_addedit_ledits.count(); i++)
    {
        bwin_addedit_ledits.at(i)->setText("");
    }
    bwin_addedit->exec();
}

void module_info_spetable::add_exec()
{
    bwin_addedit->close();

    QString queryText = "INSERT INTO "+tableDB+" ("+relidNameDB;
    for(int i=0; i<namesDB.count(); i++)
    {
        queryText+=", "+namesDB.at(i);
    }
    queryText+=") VALUES (?";
    for(int i=0; i<namesDB.count(); i++)
    {
        queryText+=" ,?";
    }
    queryText+=")";
    QSqlQuery query;
    query.prepare(queryText);
    query.addBindValue(relid);
    for(int i=0; i<namesDB.count(); i++)
    {
        query.addBindValue(bwin_addedit_ledits.at(i)->text());
    }
    if(!query.exec())
    {
        engine_db::dispERR(this, query);
    }

    update();
}

void module_info_spetable::edit()
{
    if(table->selectedItems().isEmpty()) { return; }

    disconnect(bwin_addedit_confirm_connection);
    bwin_addedit_confirm_connection = connect(bwin_addedit_confirm, SIGNAL(released()), this, SLOT(edit_exec()));
    bwin_addedit_confirm->setText(tr("Edit"));
    bwin_addedit_confirm->setDefault(true);
    bwin_addedit_ledits.at(0)->setFocus();
    for(int i=0; i<bwin_addedit_ledits.count(); i++)
    {
        bwin_addedit_ledits.at(i)->setText("");
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM "+tableDB+" WHERE id = ?");
    query.addBindValue(table->item(table->selectedItems().takeFirst()->row(), 0)->text().toUInt());
    if(query.exec())
    {
        if(query.next())
        {
            for(int i=0; i<namesDB.count(); i++)
            {
                bwin_addedit_ledits.at(i)->setText(query.record().value(namesDB.at(i)).toString());
            }
        }
    }
    else
    {
        engine_db::dispERR(this, query);
    }

    bwin_addedit->exec();
}

void module_info_spetable::edit_exec()
{
    bwin_addedit->close();

    QString queryText = "UPDATE "+tableDB+" SET ";
    for(int i=0; i<namesDB.count(); i++)
    {
        queryText+=namesDB.at(i)+"=?";
        if(i<namesDB.count()-1) { queryText+=","; }
        queryText+=" ";
    }
    queryText+="WHERE id=?";
    QSqlQuery query;
    query.prepare(queryText);
    for(int i=0; i<namesDB.count(); i++)
    {
        query.addBindValue(bwin_addedit_ledits.at(i)->text());
    }
    query.addBindValue(table->item(table->selectedItems().takeFirst()->row(), 0)->text().toUInt());
    if(!query.exec())
    {
        engine_db::dispERR(this, query);
    }

    update();
}

void module_info_spetable::remove()
{
    if(table->selectedItems().isEmpty()) { return; }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Remove an item ?"), tr("Are you sure to remove ?"), QMessageBox::Yes|QMessageBox::No);
    if(reply == QMessageBox::Yes)
    {
        QSqlQuery query;
        query.prepare("DELETE FROM "+tableDB+" WHERE id = ?");
        query.addBindValue(table->item(table->selectedItems().takeFirst()->row(), 0)->text().toUInt());
        if(!query.exec())    //exec query
        {
            engine_db::dispERR(nullptr, query);
        }
    }

    update();
}
