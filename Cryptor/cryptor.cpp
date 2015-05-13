#include "cryptor.h"
#include <iostream>
#include <openssl/err.h>

Cryptor::Cryptor()
{
    // Initialize the PRNG.
    RAND_poll();
    // Load the human readable error strings for libcrypto
    ERR_load_crypto_strings();
    // Link all algortithms
    OpenSSL_add_all_algorithms();
}

Cryptor::~Cryptor()
{
    // Cleanup the environnement
    CRYPTO_cleanup_all_ex_data();
    EVP_cleanup();
    ERR_free_strings();
}

void Cryptor::handleRANDError(const int RANDResult)
{
    /* Depending on the return value, write an apporpriate error message to
     * cerr. Also write the library error message if it's applicable.*/
    switch (RANDResult)
    {
        // Nothing was wrong (normally this function shouldn't be called in this case).
        case 1 : 
            break;
        // Returned by RAND_bytes if the operation isn't supported by the RAND function (from the openssl "documentation")
        case -1 :
            cerr << "RAND_bytes not supported by current RAND function." << endl;
            cerr << ERR_error_string(ERR_get_error(), NULL) << endl;
            break;
        // Returned by RAND_bytes if the operation didn't succeed.
        case 0 :    
            cerr << "RAND_bytes couldn't generate random numbers" << endl;
            cerr << ERR_error_string(ERR_get_error(), NULL) << endl;
            break;
        default :
            cerr << "RAND_bytes returned an unexpected value" << endl;
            break;
    }
    
}

AESKey Cryptor::generateAESKey(const unsigned keyLength)
{
    int randomResult;
    
    AESKey newKey;
    newKey.key.resize(keyLength);
    newKey.initializationVector.resize(AES_BLOCK_SIZE/8);
    
    /* Generate the AES key. If the operation wasn't succesful (return value 
     * != 1) calls a function to handle the error and returns an empty key.*/
    randomResult = RAND_bytes(newKey.key.data(), keyLength);
    if (randomResult != 1)
    {
        handleRANDError(randomResult);
        newKey.key.clear();
        newKey.initializationVector.clear();
        return newKey;
    }
    
    /* Generate the initialization vector. If the operation wasn't succesful 
     * (return value != 1) calls a function to handle the error and returns an 
     * empty key.*/
    randomResult = RAND_bytes(newKey.initializationVector.data(), 
                              AES_BLOCK_SIZE/8);
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
    
    /* RSA keypair generation. Returns 0 if unsuccessful, 1 otherwise (as much 
     * ass we can guess from the documentation). If the operation was a failure,
     * write an error message and returns an empty pair of keys.*/
    if (!RSA_generate_key_ex(keypair, keyLength, publicExponent, 0))
    {
        cerr << "RSA key generation was unsuccessful." << endl;
        cerr << ERR_error_string(ERR_get_error(), NULL) << endl;
        RSA_free(keypair);
        return keys;
    }
    
    /* Both keys are write into a buffer and then into a char string. Once the
     * operation is completed returns the pair.*/
    
    // Create buffers.
    BIO* privateKeyBuffer = BIO_new(BIO_s_mem());
    BIO* publicKeyBuffer = BIO_new(BIO_s_mem());
    
    // Write keys into the buffers.
    PEM_write_bio_RSAPrivateKey(privateKeyBuffer, keypair, NULL, NULL, 0, NULL, NULL);
    PEM_write_bio_RSAPublicKey(publicKeyBuffer, keypair);
    
    // Retrieve keys length.
    int privateKeyLength = BIO_pending(privateKeyBuffer);
    int publicKeyLength = BIO_pending(publicKeyBuffer);
    
    // Resize the vectors.
    keys.privateKey.resize(privateKeyLength + 1);
    keys.publicKey.resize(publicKeyLength + 1);
    
    // Write the content of the buffers into the char arrays.
    BIO_read(privateKeyBuffer, keys.privateKey.data(), privateKeyLength);
    BIO_read(publicKeyBuffer, keys.publicKey.data(), publicKeyLength);
    
    // Add ending character to the strings.
    keys.privateKey[privateKeyLength] = '\0';
    keys.publicKey[publicKeyLength] = '\0';
    
    // Cleanup
    BIO_free(privateKeyBuffer);
    BIO_free(publicKeyBuffer);
    RSA_free(keypair);
    
    return keys;
}

Salt Cryptor::generateSalt(const unsigned saltLength)
{
    int randomResult;
    Salt salt;
    salt.resize(saltLength);
    
    /* Generate the random salt. If the operation wasn't succesful 
     * (return value != 1) calls a function to handle the error and returns an 
     * empty salt.*/
    randomResult = RAND_bytes(salt.data(), saltLength);
    
    if (randomResult != 1)
    {
        handleRANDError(randomResult);
        salt.clear();
        return salt;
    }
    
    salt.shrink_to_fit();
    
    return salt;
}

Hash Cryptor::generateHash(const string& password, Salt& salt,
                           const unsigned numberOfRounds)
{
    Hash hash;
    hash.resize(HASH_LENGTH/8);
    
    /* Generate the derivation key (hash), from the password and the salt using
     * the sha512 digest method. The operation is repeated NUMBER_OF_HASH_ROUND
     * times.
     * If the operation isn't succesful (i.e. the return value is 0), write an
     * error message to cerr and returns an empty hash.*/
    if (!PKCS5_PBKDF2_HMAC(password.c_str(), (int) password.size(), salt.data(),
        (int) salt.size(), numberOfRounds, EVP_sha512(), 
        HASH_LENGTH/8, hash.data()))
    {
        cerr << "Hash generation was unsuccessful." << endl;
        cerr << ERR_error_string(ERR_get_error(), NULL) << endl;
        hash.clear();
    }
    
    hash.shrink_to_fit();
    return hash;
}

int Cryptor::cypherAES(string& message, const AESKey& encryptionKey)
{return 1;}
int Cryptor::decypherAES(string& message, const AESKey& encryptionKey)
{return 1;}
int Cryptor::cypherAES(RSAPair& key, const AESKey& encryptionKey)
{return 1;}
int Cryptor::decypherAES(RSAPair& key, const AESKey& encryptionKey)
{return 1;}
int Cryptor::cypherRSA(AESKey& key, const RSAPair& encryptionKey)
{return 1;}
int Cryptor::decypherRSA(AESKey& key, const RSAPair& encryptionKey)
{return 1;}
