/*
     * Contains functions required for security aspect of the chat
     * such as encryption, generation of keys as well as salts, and hash functions.
*/

#ifndef CRYPTOR_H
#define CRYPTOR_H

#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/bn.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <vector>
#include <string>
#include <cstring>

using namespace std;

// All lengths are in bits.
const unsigned AES_KEY_LENGTH = 256; 
const unsigned AES_BLOCK_SIZE = 128;
const unsigned SALT_LENGTH = 64;
const int RSA_KEY_LENGTH = 4096;
const int RSA_BLOCK_LENGTH = 512;
const int HASH_LENGTH = 512;


// Public exponent used for the RSA keys generation.
const unsigned long PUBLIC_EXPONENT = 65537;

// Number of rounds to generate a hash.
const unsigned NUMBER_OF_HASH_ROUNDS = 10000;

// Typedefs
typedef vector<unsigned char> Salt;
typedef vector<unsigned char> Hash;
typedef vector<unsigned char> CypherText;

// Abstraction for the AES keys. contains both the key and the initialization 
// vector, needed to initialize encryption.
struct AESKey
{
    vector<unsigned char> key;
    vector<unsigned char> initializationVector;
};

// Abstraction for the RSA keys. Contains both private and public keys.
struct RSAPair
{
    vector<char> privateKey;
    vector<char> publicKey;
};

class Cryptor
{
    public:
    /* Constructor.
     * Will initialize the openssl environnement for the session.
     */
    Cryptor();
    
    /* Destructor.
     * Will clean up the openssl environnement at the end of the session.
     */
    ~Cryptor();
    
    /* Randomly generate an AES key. The PRNs are cryptographically secure, if 
     * the openssl documentation is to be trusted.
     * keyLength : the desired size of the aes key, in bytes (octets).*/
    AESKey generateAESKey(const unsigned keyLength = AES_KEY_LENGTH/8);
    
    /* Randomly generate a pair of private/public RSA keys. The PRNs are 
     * cryptographically secure, if the openssl docuementation is to be trusted.
     * keyLength : the desired size of the RSA key, in bits.*/
    RSAPair generateRSAPair(const unsigned keyLength = RSA_KEY_LENGTH);
    
    /* Generate a random salt. The PRNs are cryptographically secure, if 
     * the openssl documentation is to be trusted.
     * saltLength : the desired size for the salt, in bytes (octets).
     */ 
    Salt generateSalt(const unsigned saltLength = SALT_LENGTH/8);
    
    /* Generate the hash for the given password and salt.
     * password : the password to be hashed.
     * salt : the added salt to avoid tables attack.
     * numberOfRounds : the number of rounds to generate the hash. Using a high
     * number of round avoid brute force attack.*/
    Hash generateHash(const string& password, const Salt& salt,
         const unsigned numberOfRounds = NUMBER_OF_HASH_ROUNDS);
    
    /* Generate an AES key from a Hash. 
     * hash : the hash from which the AESKey will be created.*/
    AESKey generateAESKeyFromHash(const Hash& hash);
    
    /* Returns the result of the encryption of the message with the given AES
     * key.
     * message : the message to encrypt.
     * encryptionKey : the key used for encrpytion.*/
    CypherText encryptWithAES(const string& message, const AESKey& encryptionKey);
    
    /* Returns the clear message decrypted with the given AES key.
     * cypherMessage : the message to decrypt.
     * encryptionKey : the key used for decrpytion (must the same as the one used
     * for encryptioon).*/
    string decryptWithAES(const CypherText& cypherMessage, const AESKey& encryptionKey);
    
    /* Encrypt the private key of the given RSAPair with the given AES key. Returns
     * 0 on success and -1 otherwise.
     * key : the key pair whose private key will be encrypted.
     * encryptionKey : the key used for encrpytion */
    int encryptWithAES(RSAPair& key, const AESKey& encryptionKey);
    
    /* Decrypte the private key of the given RSAPair with the given AES key. Returns
     * 0 on success and -1 otherwise.
     * key : the key pair whose private key will be decrypted.
     * encryptionKey : the key used for decrpytion (must the same as the one used
     * for encryptioon).*/
    int decryptWithAES(RSAPair& key, const AESKey& encryptionKey);
    
    /* Encrypt an AES key with the given RSAPair's public key. Returns
     * 0 on success and -1 otherwise.
     * key : the key that will be encrypted.
     * encryptionKey : the pair of key whose public key will be used for 
     * encryption. */    
    int encryptWithRSA(AESKey& key, const RSAPair& encryptionKey);
    
    /* Decrypt an AES key with the given RSAPair's private key. Returns
     * 0 on success and -1 otherwise.
     * key : the key that will be decrypted.
     * encryptionKey : the pair of key whose private key will be used for 
     * decryption. */    
    int decryptWithRSA(AESKey& key, const RSAPair& encryptionKey);
    
    private :
    /* Function that will handle errors occuring when generating random numbers
     * with the RAND function of the openSSL library.
     * RANDResult : the return value of the RAND function.
     * decryption. */ 
    void handleRANDError(const int RANDResult);
};

#endif //CRYPTOR_H
