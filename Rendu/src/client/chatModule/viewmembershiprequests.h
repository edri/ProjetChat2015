/*
     * View used by a room administrator to manage the new membership's requests.
*/

#ifndef VIEWMEMBERSHIPREQUESTS_H
#define VIEWMEMBERSHIPREQUESTS_H

#include <QDialog>
#include "../../common/models/modelChator.h"

namespace Ui {
class ViewMembershipRequests;
}

class ViewMembershipRequests : public QDialog
{
    Q_OBJECT

public:
    explicit ViewMembershipRequests(ModelChator* model, QWidget *parent = 0);
    ~ViewMembershipRequests();

    //----------------------------------------------------------------------------------
    // Goal      : Refresh the requests list.
    // Param     : requests - the requests list.
    //----------------------------------------------------------------------------------
    void refresh(const QMap<quint32, ModelRequest>& requests) const;

    //----------------------------------------------------------------------------------
    // Goal      : Remove the given request, or the selected one, depending on the
    //             parameter :
    //              -1              => remove the selected one.
    //              others values   => remove the given one.
    // Param     : requestsId - the request's ID to remove.
    //----------------------------------------------------------------------------------
    void removeRequest(const qint32 requestId = -1);

    //----------------------------------------------------------------------------------
    // Goal      : Return the selected request's ID.
    // Param     : /
    //----------------------------------------------------------------------------------
    quint32 getSelectedRequestId() const;

private slots:
    void on_btn_accept_clicked();
    void on_btn_refuse_clicked();

signals:
    void requestProcessRequest(const bool accepted) const;

private:
    Ui::ViewMembershipRequests *_ui;
     ModelChator* _model;
};

#endif // VIEWMEMBERSHIPREQUESTS_H
