#include <QApplication>
#include "../ModeleChator/modelChator.h"
#include "controllerChat.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ModelChator* model = new ModelChator();
    // Cinq utilisateurs de test.
    QSet<quint32> roomsU1;
    roomsU1.insert(1);
    roomsU1.insert(2);
    roomsU1.insert(4);
    ModelUser user1(1, "edri", "Miguel", "Santamaria", true, QDateTime::currentDateTime(), QImage("img/54226b6d8bfa4.jpg"), roomsU1);

    QSet<quint32> roomsU2;
    roomsU2.insert(2);
    roomsU2.insert(3);
    ModelUser user2(2, "beedle", "Bastien", "Rouiller", false, QDateTime::currentDateTime(), QImage("img/5470a45364910.png"), roomsU2);

    QSet<quint32> roomsU3;
    roomsU3.insert(1);
    roomsU3.insert(2);
    roomsU3.insert(3);
    roomsU3.insert(4);
    ModelUser user3(3, "benoistwolleb", "Benoist", "Wolleb", false, QDateTime::currentDateTime(), QImage("img/54226b6cd2c4b.jpg"), roomsU3);

    QSet<quint32> roomsU4;
    roomsU4.insert(2);
    ModelUser user4(4, "melhk", "Mélanie", "Huck", false, QDateTime::currentDateTime(), QImage("img/541c2d4081e22.jpg"), roomsU4);

    QSet<quint32> roomsU5;
    roomsU5.insert(1);
    roomsU5.insert(2);
    roomsU5.insert(4);
    ModelUser user5(5, "jurporan", "Jan", "Purro", false, QDateTime::currentDateTime(), QImage("img/54226b6e4cffe.jpg"), roomsU5);

    model->addUser(user1);
    model->addUser(user2);
    model->addUser(user3);
    model->addUser(user4);
    model->addUser(user5);
    // L'utilisateur connecté.
    ModelUser* currentUser = &user1;

    // Dix messages de test.
    ModelMessage message1(1, 2, 1, QDateTime::fromString("04.04.2015 20:17", "dd.MM.yyyy HH:mm"), "Salut !");
    ModelMessage message2(2, 2, 2, QDateTime::fromString("04.04.2015 20:18", "dd.MM.yyyy HH:mm"), "hello");
    ModelMessage message3(3, 2, 3, QDateTime::fromString("04.04.2015 20:20", "dd.MM.yyyy HH:mm"), "Yo");
    ModelMessage message4(4, 2, 1, QDateTime::fromString("04.04.2015 20:20", "dd.MM.yyyy HH:mm"), "Ca va ?");
    ModelMessage message5(5, 2, 2, QDateTime::fromString("04.04.2015 20:22", "dd.MM.yyyy HH:mm"), "bien et toi?");
    ModelMessage message6(6, 2, 1, QDateTime::fromString("04.04.2015 20:25", "dd.MM.yyyy HH:mm"), "Pépère ouais ^^");
    ModelMessage message7(7, 2, 4, QDateTime::fromString("04.04.2015 20:32", "dd.MM.yyyy HH:mm"), "Hello");
    ModelMessage message8(8, 2, 3, QDateTime::fromString("04.04.2015 23:56", "dd.MM.yyyy HH:mm"), "J'ai trouvé un site trop bien");
    ModelMessage message9(9, 2, 1, QDateTime::fromString("05.04.2015 00:02", "dd.MM.yyyy HH:mm"), "Pas encore au lit ? :p");
    ModelMessage message10(10, 1, 1, QDateTime::fromString("18.04.2015 15:10", "dd.MM.yyyy HH:mm"), "Yop");

    // Quatre salles de test.
    // ====== 1 ======
    // Liste des administrateurs de la salle.
    QSet<quint32> adminsR1;
    adminsR1.insert(user1.getIdUser());
    // Liste des utilisateurs de la salle.
    QSet<quint32> usersR1;
    usersR1.insert(user1.getIdUser());
    usersR1.insert(user3.getIdUser());
    usersR1.insert(user5.getIdUser());
    // Liste des messages de la salle.
    QMap<quint32, ModelMessage> messagesR1;
    messagesR1.insert(message10.getIdMessage(), message10);
    // Création de la salle de test.
    ModelRoom room1(1, "Room1", 100, false, true, QImage("img/aboutShoe.png"), adminsR1, usersR1, messagesR1);
    // ====== 2 ======
    // Liste des administrateurs de la salle.
    QSet<quint32> adminsR2;
    adminsR2.insert(user1.getIdUser());
    adminsR2.insert(user2.getIdUser());
    // Liste des utilisateurs de la salle.
    QSet<quint32> usersR2;
    usersR2.insert(user1.getIdUser());
    usersR2.insert(user2.getIdUser());
    usersR2.insert(user3.getIdUser());
    usersR2.insert(user4.getIdUser());
    usersR2.insert(user5.getIdUser());
    // Liste des messages de la salle.
    QMap<quint32, ModelMessage> messagesR2;
    messagesR2.insert(message1.getIdMessage(), message1);
    messagesR2.insert(message2.getIdMessage(), message2);
    messagesR2.insert(message3.getIdMessage(), message3);
    messagesR2.insert(message4.getIdMessage(), message4);
    messagesR2.insert(message5.getIdMessage(), message5);
    messagesR2.insert(message6.getIdMessage(), message6);
    messagesR2.insert(message7.getIdMessage(), message7);
    messagesR2.insert(message8.getIdMessage(), message8);
    messagesR2.insert(message9.getIdMessage(), message9);
    // Création de la salle de test.
    ModelRoom room2(2, "Room2", 10, true, false, QImage("img/edit.png"), adminsR2, usersR2, messagesR2);
    // ====== 3 ======
    // Liste des administrateurs de la salle.
    QSet<quint32> adminsR3;
    adminsR1.insert(user2.getIdUser());
    // Liste des utilisateurs de la salle.
    QSet<quint32> usersR3;
    usersR1.insert(user2.getIdUser());
    usersR1.insert(user3.getIdUser());
    // Liste des messages de la salle.
    QMap<quint32, ModelMessage> messagesR3;
    // Création de la salle de test.
    ModelRoom room3(3, "Room3", 150, false, true, QImage("img/delete.png"), adminsR3, usersR3, messagesR3);
    // ====== 4 ======
    // Liste des administrateurs de la salle.
    QSet<quint32> adminsR4;
    adminsR4.insert(user3.getIdUser());
    // Liste des utilisateurs de la salle.
    QSet<quint32> usersR4;
    usersR4.insert(user1.getIdUser());
    usersR4.insert(user3.getIdUser());
    usersR4.insert(user5.getIdUser());
    // Liste des messages de la salle.
    QMap<quint32, ModelMessage> messagesR4;
    // Création de la salle de test.
    ModelRoom room4(4, "Room4", 100, false, true, QImage("img/delete.png"), adminsR4, usersR4, messagesR4);

    model->addRoom(room2);
    model->addRoom(room1);
    model->addRoom(room4);
    model->addRoom(room3);

    ClientControllerInput* cci = new ClientControllerInput();
    Interpretor* i = new Interpretor(*cci);
    ClientConnector* cc = new ClientConnector();
    ControllerOutput* co = new ControllerOutput(*cc, *i);
    cc->connectToServer("localhost:1234");
    co->login("licorne", "java");

    ControllerChat* c = new ControllerChat(model, currentUser, cci, i, cc, co);
    c->showView();

    return a.exec();
}
