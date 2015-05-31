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

void ViewMembershipRequests::refresh(const QMap<quint32, ModelRequest>& requests) const
{
    _ui->tre_requests->clear();

    for (ModelRequest request : requests)
    {
        QTreeWidgetItem* requestItem = new QTreeWidgetItem(_ui->tre_requests);
        requestItem->setText(0, "#" + QString::number(request.getId()));
        requestItem->setText(1, request.getUser().getUserName());
        requestItem->setText(2, request.getRoom().getName());
        requestItem->setData(0, Qt::UserRole, request.getId());
        requestItem->setData(1, Qt::UserRole, request.getUser().getIdUser());
        requestItem->setData(2, Qt::UserRole, request.getRoom().getIdRoom());
    }

    if (_ui->tre_requests->topLevelItemCount() > 0)
    {
        _ui->tre_requests->topLevelItem(0)->setSelected(true);
        _ui->btn_accept->setDisabled(false);
        _ui->btn_refuse->setDisabled(false);
    }
    else
    {
        _ui->btn_accept->setDisabled(true);
        _ui->btn_refuse->setDisabled(true);
    }
}

void ViewMembershipRequests::removeRequest(const qint32 requestId)
{
    // If no ID is given, remove the selected request.
    if (requestId == -1)
    {
        _ui->tre_requests->takeTopLevelItem(_ui->tre_requests->currentIndex().row());
    }
    // Else remove the given ID one.
    else
    {
        quint32 nbRequests = _ui->tre_requests->topLevelItemCount();

        for (quint32 i = 0; i < nbRequests; ++i)
        {
            if (_ui->tre_requests->topLevelItem(i)->data(0, Qt::UserRole).toInt() == requestId)
            {
                _ui->tre_requests->takeTopLevelItem(i);
                break;
            }
        }
    }

    setDisabled(false);

    if (!_ui->tre_requests->topLevelItemCount())
    {
        _ui->btn_accept->setDisabled(true);
        _ui->btn_refuse->setDisabled(true);
    }
}

quint32 ViewMembershipRequests::getSelectedRequestId() const
{
    return _ui->tre_requests->selectedItems().at(0)->data(0, Qt::UserRole).toInt();
}

void ViewMembershipRequests::on_btn_accept_clicked()
{
    setDisabled(true);
    emit requestProcessRequest(true);
}

void ViewMembershipRequests::on_btn_refuse_clicked()
{
    setDisabled(true);
    emit requestProcessRequest(false);
}
