#include <QApplication>
#include "../ModeleChator/modelChator.h"
#include "controllerChat.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ModelChator* model = new ModelChator();
    // Cinq utilisateurs de test.
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

    // Dix messages de test.
    ModelMessage* message1 = new ModelMessage(1, 2, 1, QDateTime::fromString("04.04.2015 20:17", "dd.MM.yyyy HH:mm"), "Salut !");
    ModelMessage* message2 = new ModelMessage(2, 2, 2, QDateTime::fromString("04.04.2015 20:18", "dd.MM.yyyy HH:mm"), "hello");
    ModelMessage* message3 = new ModelMessage(3, 2, 3, QDateTime::fromString("04.04.2015 20:20", "dd.MM.yyyy HH:mm"), "Yo");
    ModelMessage* message4 = new ModelMessage(4, 2, 1, QDateTime::fromString("04.04.2015 20:20", "dd.MM.yyyy HH:mm"), "Ca va ?");
    ModelMessage* message5 = new ModelMessage(5, 2, 2, QDateTime::fromString("04.04.2015 20:22", "dd.MM.yyyy HH:mm"), "bien et toi?");
    ModelMessage* message6 = new ModelMessage(6, 2, 1, QDateTime::fromString("04.04.2015 20:25", "dd.MM.yyyy HH:mm"), "Pépère ouais ^^");
    ModelMessage* message7 = new ModelMessage(7, 2, 4, QDateTime::fromString("04.04.2015 20:32", "dd.MM.yyyy HH:mm"), "Hello");
    ModelMessage* message8 = new ModelMessage(8, 2, 3, QDateTime::fromString("04.04.2015 23:56", "dd.MM.yyyy HH:mm"), "J'ai trouvé un site trop bien");
    ModelMessage* message9 = new ModelMessage(9, 2, 1, QDateTime::fromString("05.04.2015 00:02", "dd.MM.yyyy HH:mm"), "Pas encore au lit ? :p");
    ModelMessage* message10 = new ModelMessage(10, 1, 1, QDateTime::fromString("18.04.2015 15:10", "dd.MM.yyyy HH:mm"), "Yop");

    room1->addMessage(message10);
    room2->addMessage(message1);
    room2->addMessage(message2);
    room2->addMessage(message3);
    room2->addMessage(message4);
    room2->addMessage(message5);
    room2->addMessage(message6);
    room2->addMessage(message7);
    room2->addMessage(message8);
    room2->addMessage(message9);
    model->addRoom(room2);
    model->addRoom(room1);
    model->addRoom(room4);
    model->addRoom(room3);

    ControllerChat* c = new ControllerChat(model, currentUser);
    c->showView();

    return a.exec();
}
