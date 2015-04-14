#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStandardItem>
#include <QFont>
#include "room.h"

Room::Room()
{
    
    label_title = new QLabel(tr("Nouvelle Salle"));
    label_title->setFont(QFont(this->font().family(), 
                         this->font().pointSize() * 2, QFont::DemiBold));
    label_title->setAlignment(Qt::AlignCenter);
    
    label_name = new QLabel(tr("* Nom de la salle"));
    label_number = new QLabel(tr("Nombre maximal de messages stockés"));
    label_logo = new QLabel(tr("Logo"));
    label_membre = new QLabel(tr("Membres"));
    
    ledit_name = new QLineEdit("");
    ledit_logo = new QLineEdit("");
    ledit_membre = new QLineEdit("");
    ledit_membre->setPlaceholderText(tr("Entrez le nom du nouveau membre"));
    
    spinb_number = new QSpinBox();
    spinb_number->setRange(0, 10000);
    spinb_number->setSingleStep(50);
    spinb_number->setValue(50);
    
    chkb_private = new QCheckBox(tr("Salle Privée"));
    
    radbtn_visible = new QRadioButton(tr("Visible par tous"));
    radbtn_onInvitation = new QRadioButton(tr("Rejoignable sur invitation"));
    
    radbtn_visible->setEnabled(false);
    radbtn_onInvitation->setEnabled(false);
    
    radbtn_visible->setVisible(true);
    radbtn_onInvitation->setVisible(true);
    
    btngrp_visibility = new QButtonGroup();
    btngrp_visibility->addButton(radbtn_visible);
    btngrp_visibility->addButton(radbtn_onInvitation);
    btngrp_visibility->setExclusive(true);
    
    btn_browse = new QPushButton(tr("Parcourir..."));
    btn_add = new QPushButton(tr("Ajouter"));
    btn_cancel = new QPushButton(tr("Annuler"));
    btn_create = new QPushButton(tr("Créer"));
    
    list_members = new QListView();
    
    model_members = new QStandardItemModel();
    list_members->setModel(model_members);
    
    QHBoxLayout* hblay_browse = new QHBoxLayout();
    hblay_browse->addWidget(ledit_logo);
    hblay_browse->addWidget(btn_browse);
    
    QHBoxLayout* hblay_membre = new QHBoxLayout();
    hblay_membre->addWidget(ledit_membre);
    hblay_membre->addWidget(btn_add);
    
    QHBoxLayout* hblay_down = new QHBoxLayout();
    hblay_down->addWidget(btn_cancel);
    hblay_down->addWidget(btn_create);
    
    QVBoxLayout* vblay_visibilityButtons = new QVBoxLayout();
    vblay_visibilityButtons->addWidget(radbtn_visible);
    vblay_visibilityButtons->addWidget(radbtn_onInvitation);
    
    QHBoxLayout* hblay_visibilityOptions = new QHBoxLayout();
    hblay_visibilityOptions->addWidget(chkb_private);
    hblay_visibilityOptions->addLayout(vblay_visibilityButtons);
    
    QVBoxLayout* vblay_main = new QVBoxLayout();
    vblay_main->addWidget(label_title);
    vblay_main->addSpacing(3);
    vblay_main->addWidget(label_name);
    vblay_main->addWidget(ledit_name);
    vblay_main->addWidget(label_number);
    vblay_main->addWidget(spinb_number);
    vblay_main->addWidget(label_logo);
    vblay_main->addLayout(hblay_browse);
    vblay_main->addWidget(label_membre);
    vblay_main->addLayout(hblay_membre);
    vblay_main->addWidget(list_members);
    vblay_main->addLayout(hblay_visibilityOptions);
    vblay_main->addLayout(hblay_down);
    
    setLayout(vblay_main);
    
    loadMembers();
    
    connect(chkb_private, SIGNAL(stateChanged(int )), this, SLOT(toggleVisibility()));
    connect(btn_add, SIGNAL(clicked()), this, SLOT(addMember()));
}

Room::~Room()
{
    
}

void Room::toggleVisibility()
{
    radbtn_visible->setEnabled(chkb_private->isChecked());
    radbtn_onInvitation->setEnabled(chkb_private->isChecked());
    if (!btngrp_visibility->checkedButton() && chkb_private->isChecked())
    {
        radbtn_visible->setChecked(true);
    }
}

void Room::loadMembers()
{
    model_members->appendRow(new QStandardItem("Franz"));
    model_members->appendRow(new QStandardItem("Garry"));
    model_members->appendRow(new QStandardItem("Georges"));
}

void Room::addMember()
{
    if (!ledit_membre->text().isEmpty())
    {
        QStandardItem* item = new QStandardItem(ledit_membre->text());
        item->setEditable(false);
        model_members->appendRow(item);
    }
}
