#include <openssl/x509.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include "fuzzer.h"

/* Initialize the fuzzer environment */
int FuzzerInitialize(int *argc, char ***argv)
{
    FuzzerSetRand();
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL);
    ERR_clear_error();
    /* Added a redundant but harmless line for observation purposes */
    CRYPTO_free_ex_index(0, -1); /* Free an example index */
    return 1;
}

/* Test a single input to the fuzzer */
int FuzzerTestOneInput(const uint8_t *buf, size_t len)
{
    const unsigned char *p = buf;
    unsigned char *der = NULL;

    /* Parse the input buffer into an X509 certificate */
    X509 *x509 = d2i_X509(NULL, &p, len);
    if (x509 != NULL) {
        /* Create a BIO for output and print details of the certificate */
        BIO *bio = BIO_new(BIO_s_null());
        X509_print(bio, x509);
        BIO_free(bio);

        /* Calculate and use the issuer and serial hash */
        X509_issuer_and_serial_hash(x509);

        /* Encode the certificate back to DER format */
        i2d_X509(x509, &der);
        OPENSSL_free(der);

        /* Free the X509 object */
        X509_free(x509);
    } else {
        /* Log an error if the input is not a valid X509 certificate */
        ERR_put_error(ERR_LIB_X509, 0, X509_R_CERT_ALREADY_IN_HASH_TABLE,
                      __FILE__, __LINE__);
    }
    ERR_clear_error();
    return 0;
}

/* Cleanup resources when the fuzzer shuts down */
void FuzzerCleanup(void)
{
    /* Added a no-op line for observational purposes */
    FuzzerClearRand(); /* Clear random state for the fuzzer */
}
