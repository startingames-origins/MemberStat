#include "window_config_gentype.h"

window_config_gentype::window_config_gentype(QString _genType, QWidget *_parent) : QMainWindow(_parent)
{
    genType=_genType;
    setWindowTitle(APP_NAME" - "+tr("Config")+" "+_genType);

    table = new QTableWidget(0, 4, this);
        table->setAlternatingRowColors(true);
        table->setStyleSheet("alternate-background-color: rgb(210,240,250);");
        //table->setColumnHidden(0, true);
        table->verticalHeader()->setHidden(true);
        table->setSelectionMode(QAbstractItemView::SingleSelection);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        QStringList horizontalHeaders;
        horizontalHeaders << "id" << tr("Priority") << tr("Name") << tr("Description");
        table->setHorizontalHeaderLabels(horizontalHeaders);
        table->resizeColumnsToContents();
        table->sortByColumn(1, Qt::AscendingOrder);
        table->verticalHeader()->setDefaultSectionSize(40);
        connect(table, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChanged()));
        //connect(table, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(openMember(QTableWidgetItem*)));

    setCentralWidget(table);

    QToolBar *toolBar = new QToolBar(this);
        actionRefresh = new QAction(QIcon(ICON_REFRESH), tr("Refresh"), this);
            connect(actionRefresh, SIGNAL(triggered()), this, SLOT(update()));
        toolBar->addAction(actionRefresh);
        toolBar->addSeparator();
        actionAdd = new QAction(QIcon(ICON_PLUS), tr("Add"), this);
            connect(actionAdd, SIGNAL(triggered()), this, SLOT(add()));
        toolBar->addAction(actionAdd);
        actionEdit = new QAction(QIcon(ICON_EDIT), tr("Edit"), this);
            connect(actionEdit, SIGNAL(triggered()), this, SLOT(edit()));
        toolBar->addAction(actionEdit);
        actionDelete = new QAction(QIcon(ICON_TRASH), tr("Delete"), this);
            connect(actionDelete, SIGNAL(triggered()), this, SLOT(remove()));
        toolBar->addAction(actionDelete);

    addToolBar(toolBar);

    updateView();
}

void window_config_gentype::updateView()
{
    int count = 0;
    actionEdit->setEnabled(false);
    actionDelete->setEnabled(false);
    table->setRowCount(0);

    QSqlQuery query;
    query.prepare("SELECT * FROM "+genType+" ORDER BY priority DESC");
    //query.addBindValue(genType);
    if(query.exec())    //exec query
    {
        while(query.next())    //Get user infos
        {
            table->setRowCount(count+1);
            table->setItem(count, 0, new QTableWidgetItem(QString::number(query.record().value("id").toInt())));
            table->setItem(count, 1, new QTableWidgetItem(QString::number(query.record().value("priority").toInt())));
            table->setItem(count, 2, new QTableWidgetItem(query.record().value("name").toString()));
            table->setItem(count, 3, new QTableWidgetItem(query.record().value("desc").toString()));
            count++;
        }
    }
    else
    {
        engine_db::dispERR(nullptr, query);
    }

    table->resizeColumnsToContents();
}

void window_config_gentype::selectionChanged()
{
    if(table->selectedItems().count()>0)
    {
        actionEdit->setEnabled(true);
        actionDelete->setEnabled(true);
    }
}

void window_config_gentype::add()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Create a new ")+genType+" ?", tr("Are you sure to create a new ")+genType+" ?", QMessageBox::Yes|QMessageBox::No);
    if(reply == QMessageBox::Yes)
    {
        QSqlQuery query;
        query.prepare("INSERT INTO "+genType+" (priority, name) VALUES (1000, '__New "+genType+"')");
        if(!query.exec())    //exec query
        {
            engine_db::dispERR(nullptr, query);
        }

        updateView();
    }
}

void window_config_gentype::edit()
{
    bwin_edit_id = table->item(table->selectionModel()->selectedRows().takeFirst().row(),0)->text().toInt();
    QSqlQuery query;
    query.prepare("SELECT * FROM "+genType+" WHERE id = ?");
    query.addBindValue(bwin_edit_id);
    if(query.exec())    //exec query
    {
        if(query.next())    //Get user infos
        {
            bwin_edit = new QDialog(this, Qt::Tool);
            bwin_edit->setWindowTitle(tr("Edit item"));
            bwin_edit_priority = new QSpinBox(bwin_edit);
            bwin_edit_priority->setRange(1, 999);
            bwin_edit_priority->setValue(query.record().value("priority").toInt());
            bwin_edit_name = new QLineEdit(query.record().value("name").toString(), bwin_edit);
            bwin_edit_desc = new QLineEdit(query.record().value("desc").toString(), bwin_edit);
            bwin_edit_save = new QPushButton(tr("Save"), bwin_edit);
            connect(bwin_edit_save, SIGNAL(released()), this, SLOT(editExec()));
            QFormLayout *bwin_edit_layout = new QFormLayout();
            bwin_edit_layout->addRow(tr("Priority"), bwin_edit_priority);
            bwin_edit_layout->addRow(tr("Name"), bwin_edit_name);
            bwin_edit_layout->addRow(tr("Description"), bwin_edit_desc);
            bwin_edit_layout->addRow("", bwin_edit_save);
            bwin_edit->setLayout(bwin_edit_layout);
            bwin_edit->exec();
        }
    }
    else
    {
        engine_db::dispERR(nullptr, query);
    }
}

void window_config_gentype::editExec()
{
    QSqlQuery query;
    query.prepare("UPDATE "+genType+" SET `priority`= ? , `name`= ? , `desc`= ? WHERE id=?");
    query.addBindValue(bwin_edit_priority->value());
    query.addBindValue(bwin_edit_name->text());
    query.addBindValue(bwin_edit_desc->text());
    query.addBindValue(bwin_edit_id);
    if(!query.exec())
    {
        engine_db::dispERR(this, query);
    }

    bwin_edit->close();
    updateView();
}

void window_config_gentype::remove()
{
    if(table->selectedItems().count()<1) { return; }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Remove a ")+genType+" ?", tr("Warning !\nAssociated member will lost association and will be marked by '-'.\nSure to remove ?"), QMessageBox::Yes|QMessageBox::No);
    if(reply == QMessageBox::Yes)
    {
        QSqlQuery query;
        query.prepare("DELETE FROM "+genType+" WHERE id = ?");
        query.addBindValue(table->item(table->selectionModel()->selectedRows().takeFirst().row(),0)->text().toInt());
        if(!query.exec())    //exec query
        {
            engine_db::dispERR(nullptr, query);
        }

        updateView();
    }
}

