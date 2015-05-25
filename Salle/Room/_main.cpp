#include <QApplication>
#include "controllerRoom.h"
#include "../../Serveur/Interpretor/interpretor.h"
#include "../../Serveur/controllerInput/clientControllerInput.h"
#include "../../Serveur/ServerConnector/connector.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    ClientControllerInput cci;
    Interpretor i(cci);
    ClientConnector c;
    ControllerOutput co(c, i);
    ModelChator* model = new ModelChator();
    
    
    // 5 utilisateurs de test.
    QImage image("img/54226b6cd2c4b.jpg");
    
    // Init user's rooms sets.
    QSet<quint32> roomIdUser1;
    QSet<quint32> roomIdUser2;
    QSet<quint32> roomIdUser3;
    QSet<quint32> roomIdUser4;
    QSet<quint32> roomIdUser5;
    
    roomIdUser1.insert(1);
    roomIdUser1.insert(2);
    roomIdUser1.insert(4);
    roomIdUser2.insert(2);
    roomIdUser2.insert(3);
    roomIdUser3.insert(1);
    roomIdUser3.insert(2);
    roomIdUser3.insert(3);
    roomIdUser3.insert(4);
    roomIdUser4.insert(2);
    roomIdUser5.insert(1);
    roomIdUser5.insert(2);
    roomIdUser5.insert(4);
    
    ModelUser user1 (1, "edri", "Miguel", "Santamaria", true, QDateTime::currentDateTime(), image,roomIdUser1);
    ModelUser user2 (2, "beedle", "Bastien", "Rouiller", false, QDateTime::currentDateTime(), image,roomIdUser2);
    ModelUser user3 (3, "benoistwolleb", "Benoist", "Wolleb", false, QDateTime::currentDateTime(), image,roomIdUser3);
    ModelUser user4 (4, "melhk", "Mélanie", "Huck", false, QDateTime::currentDateTime(), image,roomIdUser4);
    ModelUser user5 (5, "jurporan", "Jan", "Purro", false, QDateTime::currentDateTime(), image,roomIdUser5);
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
    
    ControllerRoom cr(model, &currentUser, &co);
    cci.controllerRoom(&cr);
    cr.showRoom();//room2.getIdRoom());
    
    
    
    return a.exec();
}
