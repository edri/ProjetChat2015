/*
     * Created by Mélanie Huck
     * Edited by Bastien Rouiller
     *
     * Inform controllerUser when the user finish completing
     * the connection forms or want to create a new account.
     *
*/

#ifndef VIEWUSER_H
#define VIEWUSER_H

#include <QMainWindow>
#include "viewInscription.h"

namespace Ui {
class ViewUser;
}

class ViewUser : public QMainWindow
{
    Q_OBJECT

public:
    explicit ViewUser(QWidget *parent = 0);
    ~ViewUser();

    // Getters
    QString getUsername() const;
    QString getPassword() const;
    QString getPort() const;
    QString getIpAddress() const;
    ViewInscription* getViewInscription() const;

private slots:

    //----------------------------------------------------------------------------------
    // Goal      : If the fields are validated ask the controller to retrieves the
    //             values from the forms and log to the server.
    //----------------------------------------------------------------------------------
    void on_btn_connexion_clicked();

    //----------------------------------------------------------------------------------
    // Goal      : If the fields are validated ask the controller to retrieves the
    //             values from the forms and connect to the server in order to create a
    //             new account.
    //----------------------------------------------------------------------------------
    void on_btn_inscription_clicked();

private:
    Ui::ViewUser* ui;
    //viewInscription is linked to the viewUser
    ViewInscription* _viewInscription;

signals:

    //----------------------------------------------------------------------------------
    // Goal      : Signal to the controller to retrieve the field values.
    //----------------------------------------------------------------------------------
    void requestGetIds(bool fromBtnConnection) const;
};

#endif // VIEWUSER_H
