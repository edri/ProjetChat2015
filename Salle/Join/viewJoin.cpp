#include <QVBoxLayout>
#include <QStandardItem>
#include <QFont>
#include "viewJoin.h"

ViewJoin::ViewJoin()
{    
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
    
    QVBoxLayout* vblay_main = new QVBoxLayout();
    vblay_main->addWidget(label_title);
    vblay_main->addSpacing(3);
    vblay_main->addWidget(label_name);
    vblay_main->addWidget(ledit_name);
    vblay_main->addWidget(label_rooms);
    vblay_main->addWidget(list_rooms);
    vblay_main->addWidget(btn_join);
    vblay_main->addWidget(btn_cancel);
    
    setLayout(vblay_main);
}

ViewJoin::~ViewJoin()
{
    
}
