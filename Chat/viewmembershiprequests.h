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

private:
    Ui::ViewMembershipRequests *ui;
     ModelChator* _model;
};

#endif // VIEWMEMBERSHIPREQUESTS_H
