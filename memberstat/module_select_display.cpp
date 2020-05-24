#include "module_select_display.h"

module_select_display::module_select_display(QStringList names, int _col, QWidget *_parent) : QWidget(_parent)
{
    col=_col;
    for(int i=0; i<names.count(); i++)
    {
        listWidgets.append(new QCheckBox(names.at(i), this));
        connect(listWidgets.at(i), SIGNAL(stateChanged(int)), this, SLOT(emitSelChanged(int)));
        listDisplayed.append(false);
    }

    layout = new QGridLayout(this);
    setLayout(layout);
}

int module_select_display::count()
{
    return listWidgets.count();
}

bool module_select_display::getState(int at)
{
    return listWidgets.at(at)->isChecked();
}

void module_select_display::setState(int at, bool state)
{
    listWidgets.at(at)->setChecked(state);
}

void module_select_display::setDisplayed(int at, bool displayed)
{
    listDisplayed.replace(at, displayed);
    int elem = 0;
    for(int i=0; i<listDisplayed.count(); i++)
    {
        if(listDisplayed.at(i)) { elem++; }
    }

    int row = elem/col;
    if(row*col<elem) { row++; }

    QLayoutItem *item;
    while((item = layout->takeAt(0)))
    {
        layout->removeItem(item);
    }

    int crow=0;
    int ccol=0;
    for(int i=0; i<listDisplayed.count(); i++)
    {
        if(listDisplayed.at(i))
        {
            listWidgets.at(i)->setVisible(true);
            layout->addWidget(listWidgets.at(i), crow, ccol);
            crow++;
            if(crow>(row-1)) { crow=0; ccol++; }
        }
        else
        {
            listWidgets.at(i)->setVisible(false);
        }
    }
}

void module_select_display::emitSelChanged(int)
{
    emit selectionChanged();
}
