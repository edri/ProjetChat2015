#include <QApplication>
#include "controllerRoom.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ModelChator* model = new ModelChator();
    // 5 utilisateurs de test.
    QImage image("img/54226b6cd2c4b.jpg");
    ModelUser user1 (1, "edri", "Miguel", "Santamaria", true, QDateTime::currentDateTime(), image);
    ModelUser user2 (2, "beedle", "Bastien", "Rouiller", false, QDateTime::currentDateTime(), image);
    ModelUser user3 (3, "benoistwolleb", "Benoist", "Wolleb", false, QDateTime::currentDateTime(), image);
    ModelUser user4 (4, "melhk", "Mélanie", "Huck", false, QDateTime::currentDateTime(), image);
    ModelUser user5 (5, "jurporan", "Jan", "Purro", false, QDateTime::currentDateTime(), image);
    model->addUser(user1);
    model->addUser(user2);
    model->addUser(user3);
    model->addUser(user4);
    model->addUser(user5);
    // L'utilisateur connecté.
    ModelUser currentUser = user1;
    
    // Création d'un tableau de messages vides
    QMap<quint32, ModelMessage> messages;
    // Quatre salles de test.
    // ====== 1 ======
    // Liste des administrateurs de la salle.
    QSet<quint32> adminsR1;
    adminsR1.insert(user1.getIdUser());
    // Liste des utilisateurs de la salle.
    QSet<quint32>  usersR1;
    usersR1.insert(user1.getIdUser());
    usersR1.insert(user3.getIdUser());
    usersR1.insert(user5.getIdUser());
    // Création de la salle de test.
    image = QImage("aboutShoe.png");
    ModelRoom room1 (1, "Room1", 100, false, true, image, adminsR1, usersR1, messages);
    model->addRoom(room1);
    // ====== 2 ======
    // Liste des administrateurs de la salle.
    QSet<quint32>  adminsR2;
    adminsR2.insert(user1.getIdUser());
    adminsR2.insert(user2.getIdUser());
    // Liste des utilisateurs de la salle.
    QSet<quint32>  usersR2;
    usersR2.insert(user1.getIdUser());
    usersR2.insert(user2.getIdUser());
    usersR2.insert(user3.getIdUser());
    usersR2.insert(user4.getIdUser());
    usersR2.insert(user5.getIdUser());
    // Création de la salle de test.
    image = QImage("edit.png");
    ModelRoom room2 (2, "Room2", 10, true, false, image, adminsR2, usersR2, messages);
    model->addRoom(room2);
    // ====== 3 ======
    // Liste des administrateurs de la salle.
    QSet<quint32>  adminsR3;
    adminsR1.insert(user2.getIdUser());
    // Liste des utilisateurs de la salle.
    QSet<quint32>  usersR3;
    usersR1.insert(user2.getIdUser());
    usersR1.insert(user3.getIdUser());
    // Création de la salle de test.
    image = QImage("delete.png");
    ModelRoom room3 (3, "Room3", 150, false, true, image, adminsR3, usersR3, messages);
    model->addRoom(room3);
    // ====== 4 ======
    // Liste des administrateurs de la salle.
    QSet<quint32>  adminsR4;
    adminsR4.insert(user3.getIdUser());
    // Liste des utilisateurs de la salle.
    QSet<quint32>  usersR4;
    usersR4.insert(user1.getIdUser());
    usersR4.insert(user3.getIdUser());
    usersR4.insert(user5.getIdUser());
    // Création de la salle de test.
    ModelRoom room4 (4, "Room4", 100, false, true, image, adminsR4, usersR4, messages);
    model->addRoom(room4);
    
    ControllerRoom w(model, &currentUser);
    w.showRoom();//room2.getIdRoom());
    
    
    
    return a.exec();
}
