#include <QtWidgets>

#include "room.h"
#include "roomController.h"

Room::Room(RoomController* rc)
{
    
    roomController = rc;
    
    lbl_title = new QLabel(tr("Nouvelle Salle"));
    lbl_title->setFont(QFont(this->font().family(), 
                         this->font().pointSize() * 2, QFont::DemiBold));
    lbl_title->setAlignment(Qt::AlignCenter);
    
    lbl_name = new QLabel(tr("* Nom de la salle"));
    lbl_number = new QLabel(tr("Nombre maximal de messages stockés"));
    lbl_logo = new QLabel(tr("Logo"));
    lbl_membre = new QLabel(tr("Membres"));
    
    ldt_name = new QLineEdit("");
    ldt_logo = new QLineEdit("");
    ldt_membre = new QLineEdit("");
    ldt_membre->setPlaceholderText(tr("Entrez le nom du nouveau membre"));
    
    sbx_number = new QSpinBox();
    sbx_number->setRange(0, 10000);
    sbx_number->setSingleStep(50);
    sbx_number->setValue(50);
    
    chk_private = new QCheckBox(tr("Salle Privée"));
    
    rbt_visible = new QRadioButton(tr("Visible par tous"));
    rbt_onInvitation = new QRadioButton(tr("Rejoignable sur invitation"));
    
    rbt_visible->setEnabled(false);
    rbt_onInvitation->setEnabled(false);
    
    rbt_visible->setVisible(true);
    rbt_onInvitation->setVisible(true);
    
    bgp_visibility = new QButtonGroup();
    bgp_visibility->addButton(rbt_visible);
    bgp_visibility->addButton(rbt_onInvitation);
    bgp_visibility->setExclusive(true);
    
    btn_browse = new QPushButton(tr("Parcourir..."));
    btn_add = new QPushButton(tr("Ajouter"));
    btn_cancel = new QPushButton(tr("Annuler"));
    btn_create = new QPushButton(tr("Créer"));
    
    lst_members = new QListView();
    
    sim_members = new QStandardItemModel();
    lst_members->setModel(sim_members);
    
    QHBoxLayout* hblay_browse = new QHBoxLayout();
    hblay_browse->addWidget(ldt_logo);
    hblay_browse->addWidget(btn_browse);
    
    QHBoxLayout* hblay_membre = new QHBoxLayout();
    hblay_membre->addWidget(ldt_membre);
    hblay_membre->addWidget(btn_add);
    
    QHBoxLayout* hblay_down = new QHBoxLayout();
    hblay_down->addWidget(btn_cancel);
    hblay_down->addWidget(btn_create);
    
    QVBoxLayout* vblay_visibilityButtons = new QVBoxLayout();
    vblay_visibilityButtons->addWidget(rbt_visible);
    vblay_visibilityButtons->addWidget(rbt_onInvitation);
    
    QHBoxLayout* hblay_visibilityOptions = new QHBoxLayout();
    hblay_visibilityOptions->addWidget(chk_private);
    hblay_visibilityOptions->addLayout(vblay_visibilityButtons);
    
    QVBoxLayout* vblay_main = new QVBoxLayout();
    vblay_main->addWidget(lbl_title);
    vblay_main->addSpacing(3);
    vblay_main->addWidget(lbl_name);
    vblay_main->addWidget(ldt_name);
    vblay_main->addWidget(lbl_number);
    vblay_main->addWidget(sbx_number);
    vblay_main->addWidget(lbl_logo);
    vblay_main->addLayout(hblay_browse);
    vblay_main->addWidget(lbl_membre);
    vblay_main->addLayout(hblay_membre);
    vblay_main->addWidget(lst_members);
    vblay_main->addLayout(hblay_visibilityOptions);
    vblay_main->addLayout(hblay_down);
    
    setLayout(vblay_main);
    
    loadMembers();
    
    connect(chk_private, SIGNAL(stateChanged(int )), this, SLOT(toggleVisibility()));
    connect(btn_add, SIGNAL(clicked()), this, SLOT(addMember()));
}

Room::~Room()
{
    
}


void Room::toggleVisibility()
{
    rbt_visible->setEnabled(chk_private->isChecked());
    rbt_onInvitation->setEnabled(chk_private->isChecked());
    if (!bgp_visibility->checkedButton() && chk_private->isChecked())
    {
        rbt_visible->setChecked(true);
    }
}

void Room::loadMembers()
{
    sim_members->appendRow(new QStandardItem("Franz"));
    sim_members->appendRow(new QStandardItem("Garry"));
    sim_members->appendRow(new QStandardItem("Georges"));
}

void Room::addMember()
{
    if (!ldt_membre->text().isEmpty())
    {
        QStandardItem* item = new QStandardItem(ldt_membre->text());
        item->setEditable(false);
        sim_members->appendRow(item);
    }
}
