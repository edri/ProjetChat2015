#include "cryptor.h"
#include <cstring>
#include <cstdio>
#include <iostream>

int main()
{
    Cryptor* cryptor = new Cryptor();
    
    Salt salt = cryptor->generateSalt();
    
    cout << "Generated salt : ";
    for (unsigned i = 0; i < SALT_LENGTH/8; ++i)
    {
        printf("%02X", salt.at(i));
    }
    cout << endl;
    
    string pwd;
    // cout << "Impout youre passeworde : "; cin >> pwd;
    pwd = "bonjour";
    
    Hash hash = cryptor->generateHash(pwd, salt);
    cout << "Generated hash : ";
    for (unsigned i = 0; i < HASH_LENGTH/8; ++i)
    {
        printf("%02X", hash.at(i));
    }
    cout << endl;
    
    AESKey aesKey = cryptor->generateAESKey();
    cout << "Generated AES key : ";
    for (unsigned i = 0; i < AES_KEY_LENGTH/8; ++i)
    {
        printf("%02X", aesKey.key.at(i));
    }
    cout << endl;
    
    cout << "Generated initialization vector : ";
    for (unsigned i = 0; i < AES_BLOCK_SIZE/8; ++i)
    {
        printf("%02X", aesKey.initializationVector.at(i));
    }
    cout << endl << endl;
    
    /*RSAPair rsaPair = cryptor->generateRSAPair();
    cout << "Generated RSA private key : " << endl << rsaPair.privateKey.data() << endl;
    cout << "Generated RSA public key : " << endl << rsaPair.publicKey.data() << endl << endl;*/
    
    string msg = "Je suis un premier message.";
    CypherText cypherText = cryptor->cypherAES(msg, aesKey);
    
    cout << "Message : " << msg << endl;
    cout << "Message chiffré : ";
    for (unsigned i = 0; i < cypherText.size(); ++i)
    {
        printf("%02X", cypherText.at(i));
    }
    cout << endl;
    
    string decypher = cryptor->decypherAES(cypherText, aesKey);
    cout << "Message déchiffré : " << decypher << endl << endl;
    
    msg = "Français : Les fougères mangent les petits garçons. Chinois : 蕨類植物吃小男孩。";
    cypherText = cryptor->cypherAES(msg, aesKey);
    
    cout << "Message : " << msg << endl;
    cout << "Message chiffré : ";
    for (unsigned i = 0; i < cypherText.size(); ++i)
    {
        printf("%02X", cypherText.at(i));
    }
    cout << endl;
    
    decypher = cryptor->decypherAES(cypherText, aesKey);
    cout << "Message déchiffré : " << decypher << endl << endl;
    
    
    msg = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
    cypherText = cryptor->cypherAES(msg, aesKey);
    
    cout << "Message : " << msg << endl;
    cout << "Message chiffré : ";
    for (unsigned i = 0; i < cypherText.size(); ++i)
    {
        printf("%02X", cypherText.at(i));
    }
    cout << endl;
    
    decypher = cryptor->decypherAES(cypherText, aesKey);
    cout << "Message déchiffré : " << decypher << endl << endl;
    
    /*cout << "Simulation création utilisateur" << endl;
    
    string password = "mot de passe";
    Salt saltPassword = cryptor->generateSalt();
    Hash hashPassword = cryptor->generateHash(password, saltPassword);
    RSAPair userRSAKeys = cryptor->generateRSAPair();
    Salt saltKey = cryptor->generateSalt();
    Hash masterKey = cryptor->generateHash(password, saltKey);*/
    
    delete cryptor;
    return 0;
} 
