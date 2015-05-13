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
    
    RSAPair rsaPair = cryptor->generateRSAPair();
    cout << "Generated RSA private key : " << endl << rsaPair.privateKey.data() << endl;
    cout << "Generated RSA public key : " << endl << rsaPair.publicKey.data() << endl << endl;
    
    cout << "Simulation création utilisateur" << endl;
    
    string password = "mot de passe";
    Salt saltPassword = cryptor->generateSalt();
    Hash hashPassword = cryptor->generateHash(password, saltPassword);
    RSAPair userRSAKeys = cryptor->generateRSAPair();
    Salt saltKey = cryptor->generateSalt();
    Hash masterKey = cryptor->generateHash(password, saltKey);
    
    delete cryptor;
    return 0;
} 
