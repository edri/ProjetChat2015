#include "cryptor.h"
#include <iostream>
#include <openssl/err.h>

Cryptor::Cryptor()
{
    // Initialize the PRNG.
    RAND_poll();
    // Load the human readable error strings for libcrypto
    ERR_load_crypto_strings();
    // Link all algortithms (maybe unnecessary)
    OpenSSL_add_all_algorithms();
}

Cryptor::~Cryptor()
{
    CRYPTO_cleanup_all_ex_data();
    EVP_cleanup();
    ERR_free_strings();
}

void Cryptor::handleRANDError(const int RANDResult)
{
    unsigned long errorCode;
    
    switch (RANDResult)
    {
        case 1 : 
            break;
        case -1 :
            cerr << "RAND_bytes not supported by current RAND function." << endl;
            errorCode = ERR_get_error();
            cerr << ERR_error_string(errorCode, NULL) << endl;
        case 0 :    
            cerr << "RAND_bytes couldn't generate random numbers" << endl;
            errorCode = ERR_get_error();
            cerr << ERR_error_string(errorCode, NULL) << endl;
        default :
            cerr << "RAND_bytes returned an unexpected value" << endl;
    }
    
}

AESKey Cryptor::generateAESKey(const unsigned keyLength)
{
    int randomResult;
    AESKey newKey;
    newKey.key.reserve(keyLength);
    newKey.initializationVector.reserve(keyLength);
    
    // Generate random AES Key. Returns 1 if successful 0 otherwise, -1 if it's not supported
    randomResult = RAND_bytes(newKey.key.data(), keyLength);
    
    if (randomResult != 1)
    {
        handleRANDError(randomResult);
        newKey.key.clear();
        newKey.initializationVector.clear();
        return newKey;
    }
    
    randomResult = RAND_bytes(newKey.initializationVector.data(), keyLength);
    
    if (randomResult != 1)
    {
        handleRANDError(randomResult);
        newKey.key.clear();
        newKey.initializationVector.clear();
        return newKey;
    }
    
    newKey.key.shrink_to_fit();
    newKey.initializationVector.shrink_to_fit();
    return newKey;
}

RSAPair Cryptor::generateRSAPair(const unsigned keyLength)
{
    RSA* keypair = RSA_new();
    RSAPair keys;
    BIGNUM* publicExponent = BN_new();
    BN_set_word(publicExponent, PUBLIC_EXPONENT);
    
    // RSA keypair generation. Returns 0 if unsuccessful, 1 otherwise (we suppose, the docummentation isn't exactly clear on that).
    if (!RSA_generate_key_ex(keypair, keyLength, publicExponent, 0))
    {
        cerr << "RSA key generation was unsuccessful." << endl;
        errorCode = ERR_get_error();
        cerr << ERR_error_string(errorCode, NULL) << endl;
        RSA_free(keypair);
        return keys;
    }
    
    BIO* privateKeyBuffer = BIO_new(BIO_s_mem());
    BIO* publicKeyBuffer = BIO_new(BIO_s_mem());
    
    PEM_write_bio_RSAPrivateKey(privateKey, keypair, NULL, NULL, 0, NULL, NULL);
    PEM_write_bio_RSAPublicKey(publicKey, keypair);
    
    int privateKeyLength = BIO_pending(privateKeyBuffer);
    int publicKeyLength = BIO_pending(publicKeyBuffer);
    
    keys.privateKey.reserve(privateKeyLength + 1);
    keys.publicKey.reserve(publicKeyLength + 1);
    
    BIO_read(privateKeyBuffer, keys.privateKey.data(), privateKeyLength);
    BIO_read(publicKeyBuffer, keys.publicKey.data(), publicKeyLength);
    
    keys.privateKey[privateKeyLength] = '\0';
    keys.publicKey[publicKeyLength] = '\0';
    
    BIO_free(privateKeyBuffer);
    BIO_free(publicKeyBuffer);
    RSA_free(keypair);
    
    return keys;
}

Salt Cryptor::generateSalt(const unsigned saltLength)
{
    int randomResult;
    Salt salt;
    salt.reserve(saltLength);
    
    randomResult = RAND_bytes(salt.data(), saltLength);
    
    if (randomResult != 1)
    {
        handleRANDError(randomResult);
        salt.clear()
        return salt;
    }
    
    salt.shrink_to_fit();
    return salt;
}

string Cryptor::generateHash(const string& password, Salt& salt)
{
    unsigned char hash[HASH_LENGTH/8];
    if (!PKCS5_PBKDF2_HMAC(password.c_str(), (int) password.size(), salt.data(), salt.size(), NUMBER_OF_HASH_ROUNDS, EVP_sha512(), HASH_LENGTH/8, hash))
    {
        return -1;
    }
    
    
}
