
#ifndef CRYPTOR_H
#define CRYPTOR_H

#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/bn.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <vector>
#include <string>

// All lengths are in bits.
const unsigned AES_KEY_LENGTH = 256; 
const unsigned SALT_LENGTH = 64;
const unsigned RSA_KEY_LENGTH = 4096;
const int HASH_LENGTH = 512;

const unsigned long PUBLIC_EXPONENT = 65537;

const unsigned NUMBER_OF_HASH_ROUNDS = 10000;

typedef vector<unsigned char> Salt;

struct AESKey
{
    vector<unsigned char> key;
    vector<unsigned char> initializationVector;
    
};

struct RSAPair
{
    vector<char> privateKey;
    vector<char> publicKey;
};

class Cryptor
{
    public:
    Cryptor();
    ~Cryptor();
    
    AESKey generateAESKey(const unsigned keyLength = AES_KEY_LENGTH/8);
    RSAPair generateRSAPair(const unsigned keyLength = RSA_KEY_LENGTH);
    
    Salt generateSalt(const unsigned saltLength = SALT_LENGTH/8);
    QString generateHash(const QString& password, Salt& salt);
    
    int cypherAES(QString& message, const AESKey key);
    int decypherAES(QString& message, const AESKey key);
    int cypherAES(RSAPair& message, const AESKey key);
    int decypherAES(RSAPair& message, const AESKey key);    
    int cypherRSA(AESKey& key, const RSAPair& key);
    int decypherRSA(AESKey& key, const RSAPair& key);
    
    private :
    void handleRANDError(const int RANDResult);
};

#endif //CRYPTOR_H
