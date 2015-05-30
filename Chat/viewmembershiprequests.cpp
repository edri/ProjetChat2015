#include "viewmembershiprequests.h"
#include "ui_viewmembershiprequests.h"

ViewMembershipRequests::ViewMembershipRequests(ModelChator* model, QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::ViewMembershipRequests),
    _model(model)
{
    _ui->setupUi(this);
}

ViewMembershipRequests::~ViewMembershipRequests()
{
    delete _ui;
}

void ViewMembershipRequests::refresh(const QList<ModelRequest>& requests) const
{
    _ui->tre_requests->clear();

    for (ModelRequest request : requests)
    {
        QTreeWidgetItem* requestItem = new QTreeWidgetItem(_ui->tre_requests);
        requestItem->setText(0, request.getUser().getUserName());
        requestItem->setText(1, request.getRoom().getName());
        requestItem->setData(0, Qt::UserRole, request.getUser().getIdUser());
        requestItem->setData(1, Qt::UserRole, request.getRoom().getIdRoom());
    }
}

void ViewMembershipRequests::on_btn_accept_clicked()
{

}

void ViewMembershipRequests::on_btn_refuse_clicked()
{

}
