#include <sodium.h>
#include <emscripten.h>

EMSCRIPTEN_KEEPALIVE
int vrf_keypair(unsigned char *pk, unsigned char *sk) {
    return crypto_vrf_keypair(pk, sk);
}

EMSCRIPTEN_KEEPALIVE
int vrf_prove(unsigned char *proof, const unsigned char *sk, const unsigned char *m, unsigned long long mlen) {
    return crypto_vrf_prove(proof, sk, m, mlen);
}

EMSCRIPTEN_KEEPALIVE
int vrf_verify(unsigned char *output, const unsigned char *pk, const unsigned char *proof, const unsigned char *m, unsigned long long mlen) {
    return crypto_vrf_verify(output, pk, proof, m, mlen);
}

EMSCRIPTEN_KEEPALIVE
int vrf_proof_to_hash(unsigned char *hash, const unsigned char *proof) {
    return crypto_vrf_proof_to_hash(hash, proof);
}

EMSCRIPTEN_KEEPALIVE
size_t vrf_publickeybytes() {
    return crypto_vrf_PUBLICKEYBYTES;
}

EMSCRIPTEN_KEEPALIVE
size_t vrf_secretkeybytes() {
    return crypto_vrf_SECRETKEYBYTES;
}

EMSCRIPTEN_KEEPALIVE
size_t vrf_proofbytes() {
    return crypto_vrf_PROOFBYTES;
}

EMSCRIPTEN_KEEPALIVE
size_t vrf_outputbytes() {
    return crypto_vrf_OUTPUTBYTES;
}
