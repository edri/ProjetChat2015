#include <QApplication>
#include "../ModeleChator/modelChator.h"
#include "controllerChat.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ModelChator* model = new ModelChator();
    // 5 utilisateurs de test.
    ModelUser* user1 = new ModelUser(1, "edri", "Miguel", "Santamaria", true, QDateTime::currentDateTime(), "541c2d4081e22.jpg");
    ModelUser* user2 = new ModelUser(2, "beedle", "Bastien", "Rouiller", false, QDateTime::currentDateTime(), "5470a45364910.jpg");
    ModelUser* user3 = new ModelUser(3, "benoistwolleb", "Benoist", "Wolleb", false, QDateTime::currentDateTime(), "54226b6cd2c4b.jpg");
    ModelUser* user4 = new ModelUser(4, "melhk", "Mélanie", "Huck", false, QDateTime::currentDateTime(), "54226b6d8bfa4.jpg");
    ModelUser* user5 = new ModelUser(5, "jurporan", "Jan", "Purro", false, QDateTime::currentDateTime(), "54226b6e4cffe.jpg");
    model->addUser(user1);
    model->addUser(user2);
    model->addUser(user3);
    model->addUser(user4);
    model->addUser(user5);
    // L'utilisateur connecté.
    ModelUser* currentUser = user1;

    // Quatre salles de test.
    // ====== 1 ======
    // Liste des administrateurs de la salle.
    QMap<quint32, ModelUser*> adminsR1;
    adminsR1.insert(user1->getIdUser(), user1);
    // Liste des utilisateurs de la salle.
    QMap<quint32, ModelUser*> usersR1;
    usersR1.insert(user1->getIdUser(), user1);
    usersR1.insert(user3->getIdUser(), user3);
    usersR1.insert(user5->getIdUser(), user5);
    // Création de la salle de test.
    ModelRoom* room1 = new ModelRoom(1, "Room1", 100, false, true, "aboutShoe.png", adminsR1, usersR1);
    model->addRoom(room1);
    // ====== 4 ======
    // Liste des administrateurs de la salle.
    QMap<quint32, ModelUser*> adminsR4;
    adminsR4.insert(user3->getIdUser(), user3);
    // Liste des utilisateurs de la salle.
    QMap<quint32, ModelUser*> usersR4;
    usersR4.insert(user1->getIdUser(), user1);
    usersR4.insert(user3->getIdUser(), user3);
    usersR4.insert(user5->getIdUser(), user5);
    // Création de la salle de test.
    ModelRoom* room4 = new ModelRoom(4, "Room4", 100, false, true, "delete.png", adminsR4, usersR4);
    model->addRoom(room4);
    // ====== 2 ======
    // Liste des administrateurs de la salle.
    QMap<quint32, ModelUser*> adminsR2;
    adminsR2.insert(user1->getIdUser(), user1);
    adminsR2.insert(user2->getIdUser(), user2);
    // Liste des utilisateurs de la salle.
    QMap<quint32, ModelUser*> usersR2;
    usersR2.insert(user1->getIdUser(), user1);
    usersR2.insert(user2->getIdUser(), user2);
    usersR2.insert(user3->getIdUser(), user3);
    usersR2.insert(user4->getIdUser(), user4);
    usersR2.insert(user5->getIdUser(), user5);
    // Création de la salle de test.
    ModelRoom* room2 = new ModelRoom(2, "Room2", 10, true, false, "edit.png", adminsR2, usersR2);
    model->addRoom(room2);
    // ====== 3 ======
    // Liste des administrateurs de la salle.
    QMap<quint32, ModelUser*> adminsR3;
    adminsR1.insert(user2->getIdUser(), user2);
    // Liste des utilisateurs de la salle.
    QMap<quint32, ModelUser*> usersR3;
    usersR1.insert(user2->getIdUser(), user2);
    usersR1.insert(user3->getIdUser(), user3);
    // Création de la salle de test.
    ModelRoom* room3 = new ModelRoom(3, "Room3", 150, false, true, "delete.png", adminsR3, usersR3);
    model->addRoom(room3);

    ControllerChat* c = new ControllerChat(model, currentUser);
    c->showView();

    return a.exec();
}
