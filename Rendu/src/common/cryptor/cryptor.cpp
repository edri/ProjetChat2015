/*
 * Implements cryptor.h
 * The openSSL library is vastly used in the implementation of all those
 * functions. It's advised to check the openSSL documentation for more 
 * informations about their functions.
*/

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

AESKey Cryptor::generateAESKeyFromHash(const Hash& hash)
{
    AESKey key;
    // Copy the wanted number of bytes from the hash into the key.
    key.key.resize(AES_KEY_LENGTH/8);
    memcpy(key.key.data(), hash.data(), key.key.size());
    // We initialize the IV with 0s.
    key.initializationVector.resize(AES_BLOCK_SIZE/8, 0);
    return key;
}

AESKey Cryptor::generateAESKey(const unsigned keyLength)
{
    int randomResult;
    
    // Initialize the vectors lengths.
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

Hash Cryptor::generateHash(const string& password, const Salt& salt,
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

CypherText Cryptor::encryptWithAES(const string& message, const AESKey& encryptionKey)
{
    // Initialize encryption.
    EVP_CIPHER_CTX encrypt;
    EVP_CIPHER_CTX_init(&encrypt);
    EVP_EncryptInit_ex(&encrypt, EVP_aes_256_cbc(), NULL, encryptionKey.key.data(), encryptionKey.initializationVector.data());
    
    /* The max length fot cypher text for n bytes of plain text is n + AES_BLOCK_SIZE -1 bytes */
    int cypherLength = (int) message.length() + AES_BLOCK_SIZE;
    int finalEncryptLength = 0;
    unsigned char *cypher = new unsigned char[cypherLength];

    /* Allows reusing of 'encrypt' for multiple encryption cycles */
    EVP_EncryptInit_ex(&encrypt, NULL, NULL, NULL, NULL);

    /* Update the  cypher text, cypherLength is filled with the length of ciphertext generated */
    EVP_EncryptUpdate(&encrypt, cypher, &cypherLength, (unsigned char*)message.data(), (int) message.length());

    /* Update the cypher text with the final remaining bytes */
    EVP_EncryptFinal_ex(&encrypt, cypher + cypherLength, &finalEncryptLength);
    
    // Copy the cypher text into  a vector.
    CypherText cypherMessage(cypherLength + finalEncryptLength);
    memcpy(cypherMessage.data(), cypher, cypherMessage.size());
    
    // Clean up
    delete[] cypher;
    EVP_CIPHER_CTX_cleanup(&encrypt);
    
    return cypherMessage;
}
string Cryptor::decryptWithAES(const CypherText& cypherMessage, const AESKey& encryptionKey)
{
    // Initialize decryption
    EVP_CIPHER_CTX decrypt;
    EVP_CIPHER_CTX_init(&decrypt);
    EVP_DecryptInit_ex(&decrypt, EVP_aes_256_cbc(), NULL, encryptionKey.key.data(), encryptionKey.initializationVector.data());
    
    /* The plain text length  will always be equal to or lesser than the length of the cypher text*/
    int plainLength = (int) cypherMessage.size();
    int finalLength = 0;
    unsigned char *plainText = new unsigned char[plainLength];

    EVP_DecryptInit_ex(&decrypt, NULL, NULL, NULL, NULL);
    
    /* Update the  plain text, plainLength is filled with the length of the plain text generated */
    EVP_DecryptUpdate(&decrypt, plainText, &plainLength, cypherMessage.data(), (int) cypherMessage.size());
    
    /* Update the plain text with the final remaining bytes */
    EVP_DecryptFinal_ex(&decrypt, plainText + plainLength, &finalLength);
    
    // Copy the plain text into a string.
    string message((char*)plainText, plainLength + finalLength);
    
    // Clean up
    delete[] plainText;
    EVP_CIPHER_CTX_cleanup(&decrypt);
    
    return message;
}
int Cryptor::encryptWithAES(RSAPair& key, const AESKey& encryptionKey)
{
    // Initialize encryption.
    EVP_CIPHER_CTX encrypt;
    EVP_CIPHER_CTX_init(&encrypt);
    EVP_EncryptInit_ex(&encrypt, EVP_aes_256_cbc(), NULL, encryptionKey.key.data(), encryptionKey.initializationVector.data());
    
    /* The max length fot cypher text for n bytes of plain text is n + AES_BLOCK_SIZE -1 bytes */
    int cypherLength = (int) key.privateKey.size() + AES_BLOCK_SIZE;
    int finalEncryptLength = 0;
    vector<char> cypher(cypherLength);

    /* Allows reusing of 'encrypt' for multiple encryption cycles */
    EVP_EncryptInit_ex(&encrypt, NULL, NULL, NULL, NULL);

   /* Update the  plain text, plainLength is filled with the length of the plain text generated */
    EVP_EncryptUpdate(&encrypt, (unsigned char*)cypher.data(), &cypherLength, (unsigned char*)key.privateKey.data(), (int) key.privateKey.size());

    /* Update the cypher text with the final remaining bytes */
    EVP_EncryptFinal_ex(&encrypt, (unsigned char*)cypher.data() + cypherLength, &finalEncryptLength);
    
    // Resize the vector to the actual size of the encrypted key.
    cypher.resize(cypherLength + finalEncryptLength);
    
    // Copy the encrypted key in the RSAPair.
    key.privateKey = cypher;
    
    // Clean up
    EVP_CIPHER_CTX_cleanup(&encrypt);
    
    return 0;
}
int Cryptor::decryptWithAES(RSAPair& key, const AESKey& encryptionKey)
{
    // Initialize decryption
    EVP_CIPHER_CTX decrypt;
    EVP_CIPHER_CTX_init(&decrypt);
    EVP_DecryptInit_ex(&decrypt, EVP_aes_256_cbc(), NULL, encryptionKey.key.data(), encryptionKey.initializationVector.data());
    
    /* The plain text length  will always be equal to or lesser than the length of the cypher text*/
    int plainLength = (int) key.privateKey.size();
    int finalLength = 0;
    vector<char> plainKey(plainLength);
    
    // Allows the use of "decrypt" for multiple decryption cycles.
    EVP_DecryptInit_ex(&decrypt, NULL, NULL, NULL, NULL);
    
    /* Update the  plain text, plainLength is filled with the length of the plain text generated */
    EVP_DecryptUpdate(&decrypt, (unsigned char*)plainKey.data(), &plainLength, (unsigned char*)key.privateKey.data(), (int) key.privateKey.size());
    
     /* Update the plain text with the final remaining bytes */
    EVP_DecryptFinal_ex(&decrypt, (unsigned char*)plainKey.data() + plainLength, &finalLength);
    
    // Resize the vector to the actual size of the decrypted key.
    plainKey.resize(plainLength + finalLength);
    
    // Copy the decrypted key in the RSAPair.
    key.privateKey = plainKey;
    
    // Clean up
    EVP_CIPHER_CTX_cleanup(&decrypt);
    
    return 0;
}

int Cryptor::encryptWithRSA(AESKey& key, const RSAPair& encryptionKey)
{
    vector<unsigned char> encryptedKey(RSA_BLOCK_LENGTH);
    vector<unsigned char> encryptedIV(RSA_BLOCK_LENGTH);
    int encryptedSize;
    
    // We need to read the public key from a vector and write it into a RSA "object"
    // (openSSL object used in the encryption function).
    
    // Create a buffer
    BIO* publicKey = BIO_new(BIO_s_mem());
    
    // Write the key into the buffer.
    BIO_write(publicKey, encryptionKey.publicKey.data(), (int) encryptionKey.publicKey.size() - 1);
    // Create an RSA type variable from the buffer.
    RSA* keypair = PEM_read_bio_RSAPublicKey(publicKey,NULL,0,NULL);
    
    // Encrypt the AES key with the public key, if the function returns -1 or a bigger value than an
    // RSA block (we only apply the RSA key once, to encrypt a block smaller or equal to a RSA block),
    // returns -1 to signal an error.
    // The RSA_PKCS1_OAEP_PADDING is the padding recommended for all new applications by the openSSL documentation.
    encryptedSize = RSA_public_encrypt((int)key.key.size() + 1, key.key.data(), encryptedKey.data(), keypair, RSA_PKCS1_OAEP_PADDING);
    if(encryptedSize == -1 || encryptedSize > RSA_KEY_LENGTH/8)
    {
        return -1;
    }
    
    // Encrypt the AES IV with the public key in a similar fashion than the AES key just above.
    encryptedSize = RSA_public_encrypt((int)key.initializationVector.size() + 1, key.initializationVector.data(), encryptedIV.data(), keypair, RSA_PKCS1_OAEP_PADDING);
    if(encryptedSize == -1 || encryptedSize > RSA_KEY_LENGTH/8)
    {
        return -1;
    }
    
    // Copy the encrypted key and IV int AESKey struct.
    key.key = encryptedKey;
    key.initializationVector = encryptedIV;
    key.key.shrink_to_fit();
    key.initializationVector.shrink_to_fit();
    
    // Free the allocated openSSL "objects"
    BIO_free(publicKey);
    RSA_free(keypair);
    return 0;
}
int Cryptor::decryptWithRSA(AESKey& key, const RSAPair& encryptionKey)
{
    vector<unsigned char> decryptedKey(RSA_BLOCK_LENGTH);
    vector<unsigned char> decryptedIV(RSA_BLOCK_LENGTH);
    int decryptedSize;
    
    // We need to read the private key from a vector and write it into a RSA "object"
    // (openSSL object used in the decryption function).
    
    // Create a buffer.
    BIO* privateKey = BIO_new(BIO_s_mem());
    
    // Write the key into the buffer.
    BIO_write(privateKey, encryptionKey.privateKey.data(), (int)encryptionKey.privateKey.size() - 1);
    // Create an RSA type variable from the buffer.
    RSA *keypair = PEM_read_bio_RSAPrivateKey(privateKey,NULL,0,NULL);
    
    
    // Dencrypt the AES key with the private key, if the function returns -1 ,
    // returns -1 to signal an error.
    // The RSA_PKCS1_OAEP_PADDING is the padding recommended for all new applications by the openSSL documentation.
    decryptedSize = RSA_private_decrypt((int)key.key.size(), key.key.data(), decryptedKey.data(), keypair, RSA_PKCS1_OAEP_PADDING);
    if(decryptedSize == -1)
    {
        return -1;
    }
    
    // Resize the key to its actual size, dropping the additional '\0' at the end.
    decryptedKey.resize(decryptedSize-1);
    
    // Decrypt the AES IV with the private key in a similar fashion than the AES key just above.
    decryptedSize = RSA_private_decrypt((int)key.initializationVector.size(), key.initializationVector.data(), decryptedIV.data(), keypair, RSA_PKCS1_OAEP_PADDING);
    if(decryptedSize == -1)
    {
        return -1;
    }
    
    // Resize the IV to its actual size, dropping the additional '\0' at the end.
    decryptedIV.resize(decryptedSize-1);
    
    // Copy the decrypted key and IV int AESKey struct.
    key.key = decryptedKey;
    key.initializationVector = decryptedIV;
    
    // Free the allocated openSSL "objects"
    BIO_free(privateKey);
    RSA_free(keypair);
    
    return 0;
}
