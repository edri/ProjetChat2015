#ifndef VIEWMEMBERSHIPREQUESTS_H
#define VIEWMEMBERSHIPREQUESTS_H

#include <QDialog>
#include "../ModeleChator/modelChator.h"

namespace Ui {
class ViewMembershipRequests;
}

class ViewMembershipRequests : public QDialog
{
    Q_OBJECT

public:
    explicit ViewMembershipRequests(ModelChator* model, QWidget *parent = 0);
    ~ViewMembershipRequests();

    void refresh(const QMap<quint32, ModelRequest>& requests) const;
    void removeRequest(const qint32 requestId = -1);

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
