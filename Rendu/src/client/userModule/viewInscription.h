/*
     * Created by Mélanie Huck
     * Edited by Bastien Rouiller
     *
     * Verify user input's in the forms and inform ControllerUser
     * when user's information can be processed.
*/

#ifndef VIEWINSCRIPTION_H
#define VIEWINSCRIPTION_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include "../../common/models/modelChator.h"
#include "../../common/leChator/chatorConstants.h"

namespace Ui {
class viewInscription;
}

class ViewInscription : public QMainWindow
{
    Q_OBJECT

public:
    //----------------------------------------------------------------------------------
    // Goal      : Constructor for the view used for inscription
    // Param     : /
    //----------------------------------------------------------------------------------
    ViewInscription(QWidget *parent = 0);

    //----------------------------------------------------------------------------------
    // Goal      : Constructor for the view used for inscription
    // Param     : currentUser - Contain information about the current user
    //----------------------------------------------------------------------------------
    ViewInscription(QWidget *parent, ModelUser* _currentUser);

    ~ViewInscription();

    //----------------------------------------------------------------------------------
    // Goal      : Occurs when the entered username already exists.
    // Param     : /
    //----------------------------------------------------------------------------------
    void usernameAlreadyExistd();

    //----------------------------------------------------------------------------------
    // Goal      : Allow the controller to get the user's inputs in the view
    // Param     : /
    //----------------------------------------------------------------------------------
    QString getFirstName() const;
    QString getLastName() const;
    QString getUserName() const;
    QString getPassword() const;
    QImage getProfileImage() const;

    //----------------------------------------------------------------------------------
    // Goal      : This function is used to set the currentUser and display it on the
    //             edition view.
    // Param     : currentUser - Current user connected
    //----------------------------------------------------------------------------------
    void setCurrentUser(ModelUser* _currentUser);

private slots:
    //----------------------------------------------------------------------------------
    // Goal      : When clicking on the [...] button next to the picture, open a window
    //             to allow the user selecting a profile picture from his computer.
    // Param     : /
    //----------------------------------------------------------------------------------
    void on_btn_path_clicked();

    //----------------------------------------------------------------------------------
    // Goal      : If the user is creating an account, verify the fields and ask the
    //             controller to take further action such as interacting with cryptor
    //             and the server in order to create securely.
    // Param     : /
    //----------------------------------------------------------------------------------
    void on_btn_inscription_clicked();

    void on_btn_cancel_clicked();

    void on_btn_question_clicked();

private:
    Ui::viewInscription *_ui;
    void closeEvent(QCloseEvent*);

    //----------------------------------------------------------------------------------
    // Goal      : Make several field's verification such as password complexity and
    //             required fields. Return false if one or many fields are incorrect.
    // Param     : /
    //----------------------------------------------------------------------------------
    bool verifyFields();

    //----------------------------------------------------------------------------------
    // Goal      : Validate the profile image. Return false if the image does not have
    //             an appropriate format
    // Param     : /
    //----------------------------------------------------------------------------------
    bool verifyProfileImage() const;

    QString _passwordRequirement;
    bool _edition;
    ModelUser* _currentUser;

signals:
    //----------------------------------------------------------------------------------
    // Goal      : These two signals are used to ask the controller to retrieves the values
    //             from the forms and takes the lead.
    // Param     : /
    //----------------------------------------------------------------------------------
    void requestGetNewUser() const;
    void requestEditUser() const;

    //----------------------------------------------------------------------------------
    // Goal      : The view sent this signal when user cancel inscription. Enable
    //             connection view.
    // Param     : /
    //----------------------------------------------------------------------------------
    void requestCancelInscription() const;
};

#endif // VIEWINSCRIPTION_H
