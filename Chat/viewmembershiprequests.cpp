#include "viewmembershiprequests.h"
#include "ui_viewmembershiprequests.h"

ViewMembershipRequests::ViewMembershipRequests(ModelChator* model, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewMembershipRequests),
    _model(model)
{
    ui->setupUi(this);
}

ViewMembershipRequests::~ViewMembershipRequests()
{
    delete ui;
}
