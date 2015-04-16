#include <QtWidgets>

#include "viewRoom.h"
#include "controllerRoom.h"

 ViewRoom:: ViewRoom()
{
    // Initialization of the GUI elements.
    lbl_title = new QLabel("");
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
    btn_create = new QPushButton("");
    
    lst_members = new QListView();
    
    sim_members = new QStandardItemModel();
    lst_members->setModel(sim_members);
    
    // Placine the elements inside layouts.
    
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
    
    // Main layout.
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
    
    // Connect signals with public slots.
    connect(chk_private, SIGNAL(stateChanged(int )), this, SLOT(toggleVisibility()));
}

 ViewRoom::~ ViewRoom()
{
    
}

void ViewRoom::clear()
{
    ldt_logo->clear();
    ldt_membre->clear();
    ldt_name->clear();
    sim_members->clear();
    chk_private->setChecked(false);
    rbt_onInvitation->setChecked(false);
    rbt_visible->setChecked(false);
    sbx_number->setValue(50);
}

void ViewRoom::toggleVisibility()
{
    rbt_visible->setEnabled(chk_private->isChecked());
    rbt_onInvitation->setEnabled(chk_private->isChecked());
    if (!bgp_visibility->checkedButton() && chk_private->isChecked())
    {
        rbt_visible->setChecked(true);
    }
}
