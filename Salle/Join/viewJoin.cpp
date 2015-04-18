#include <QVBoxLayout>
#include <QStandardItem>
#include <QFont>
#include "viewJoin.h"

ViewJoin::ViewJoin()
{    
    // Setting attributes
    setAttribute(Qt::WA_DeleteOnClose, true);
    
    // Initializazion of core elements
    layouts = new QList<QLayout*>();
    
    // Initialization of the GUI elements.
    label_title = new QLabel("Rejoindre une salle");
    label_title->setFont(QFont(this->font().family(), 
                         this->font().pointSize() * 2, QFont::DemiBold));
    label_title->setAlignment(Qt::AlignCenter);
    
    label_name = new QLabel("Nom de la salle");
    label_rooms = new QLabel("Résultat(s)");
    
    ledit_name = new QLineEdit("");
    
    btn_join = new QPushButton("Rejoindre");
    btn_cancel = new QPushButton("Annuler");
    
    list_rooms = new QListView();
    
    model_rooms = new QStandardItemModel();
    list_rooms->setModel(model_rooms);
    
    // Main Layout
    QVBoxLayout* vbl_main = new QVBoxLayout();
    layouts->append(vbl_main);
    vbl_main->addWidget(label_title);
    vbl_main->addSpacing(3);
    vbl_main->addWidget(label_name);
    vbl_main->addWidget(ledit_name);
    vbl_main->addWidget(label_rooms);
    vbl_main->addWidget(list_rooms);
    vbl_main->addWidget(btn_join);
    vbl_main->addWidget(btn_cancel);
    
    setLayout(vbl_main);
}

ViewJoin::~ViewJoin()
{
    delete label_title;
    delete label_name;
    delete label_rooms;
    delete ledit_name;
    delete btn_join;
    delete btn_cancel;
    delete list_rooms;
    delete model_rooms;
    
    // Delete layouts.
    for (QLayout* l : *layouts)
    {
        delete l;
    }
    delete layouts;
}
