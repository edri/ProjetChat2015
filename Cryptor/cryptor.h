
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
// vector.
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
    
    /* Generate the hash for the given password and salt. The 
     * password : the password to be hashed.
     * salt : the added salt to avoid tables attack.
     * numberOfRounds : the number of rounds to generate the hash. Using a high
     * number of round avoid brute force attack.*/
    Hash generateHash(const string& password, Salt& salt,
         const unsigned numberOfRounds = NUMBER_OF_HASH_ROUNDS);
         
    AESKey generateAESKeyFromHash(const Hash& hash);
    
    CypherText encryptWithAES(const string& message, const AESKey& encryptionKey);
    string decryptWithAES(const CypherText& cypherMessage, const AESKey& encryptionKey);
    int encryptWithAES(RSAPair& key, const AESKey& encryptionKey);
    int decryptWithAES(RSAPair& key, const AESKey& encryptionKey);    
    int encryptWithRSA(AESKey& key, const RSAPair& encryptionKey);
    int decryptWithRSA(AESKey& key, const RSAPair& encryptionKey);
    
    private :
    void handleRANDError(const int RANDResult);
};

#endif //CRYPTOR_H
