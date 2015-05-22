#include "cryptor.h"

int main()
{
    Cryptor* cryptor = new Cryptor();
    
    
    
    
    // Inscription
    
    // Génération du sel et du hash du mot de passe.
    
    // Depuis une QString utilister QString::toStdString()
    string password = "password";
    Salt passwordSalt = cryptor->generateSalt();
    
    if(passwordSalt.empty())
    {
        // L'opératon a échoué
    }
    
    Hash hashPassword = cryptor->generateHash(password, passwordSalt);
    
    if(hashPassword.empty())
    {
        // L'opératon a échoué
    }
    
    // Génération de la pair de clée (privée et publique).
    RSAPair keyPair = cryptor->generateRSAPair();
    
    if(keyPair.privateKey.empty())
    {
        // L'opération a échoué
    }
    
    // Génération du sel et du hash pour chiffré la clé privée.
    Salt keySalt = cryptor->generateSalt();
    Hash keyHash = cryptor->generateHash(password, keySalt);
    // Je fais pas à nouveau les vérifications...
    
    // Chiffrement de la clé privée.
    if(cryptor->encryptWithAES(keyPair, cryptor->generateAESKeyFromHash(keyHash)) != 0)
    {
        // L'opération a échoué
    }
    
    // Transmettre passwordSalt, hashPassword, keySalt et keyPair
    
    
    
    
    
    // Connexion
    
    // Récupération de "passwordSalt" (type Salt, password est de type string)
    
    Hash hashPassword = cryptor->generateHash(password, passwordSalt);
    
    // Envoi du hash au serveur pour identification.
    
    // Récepetion des données utilisateurs ModelUser, "keySalt" et pair de clés (RSAPair keyPair).
    // Je sais plus qui s'occupe de ça, mais il doit connaître le mot de passe pour pouvoir déchiffrer la clé.
    
    // On déchiffre la clé privée de l'utilisateur, pour ça il faut générer le hash qui sert de clé.
    
     Hash keyHash = cryptor->generateHash(password, keySalt);
     
    // Déchiffrement de la clé privée.
    cryptor->decryptWithAES(keyPair, cryptor->generateAESKeyFromHash(keyHash);
    
    // Ajouter la pair de clé dans ModelChator.
    
    
    
    
    
    // Récepetion d'un ModelRoom.
    // Il doit y avoir un champ "secretKey" dans ModelRoom.
    
    // Il faut déchiffrer la clé de la salle.
    cryptor->decryptWithRSA(room.secretKey(), modelChator.rsaKeyPair());
    
    
    
    
    //Envoi d'un message dans une salle privée.
    
    CypherText messageChiffré(cryptor->encryptWithAES(contenuChampText.toStdString(), room.secretKey()));
    QByteArray contenuMessage((char*)messageChiffré.data(), messageChiffré.size());
    
    // Contruire le modelMessage en lui passant le QByteArray.
    
    
    
    // Réception d'un message dans une salle privée
    CypherText messageChiffré(message.content().size());
    memcpy(messageChiffré.data(), message.content().data(), message.content().size());
    Qstring messageEnClair(cryptor->decryptWithAES(messageChiffré, room.secretKey()));
    message.setContent(messageEnClair.toUtf8());
    
    
    
    return 0;
    
}
