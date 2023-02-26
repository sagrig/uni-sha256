#ifndef UNISHA256_H
#define UNISHA256_H

#define HASHVALNUM 8  /* number of hash values */
#define MAXARGSNUM 2  /* number of expected arguments (including prog name) */
#define WSCHEDLBUF 64 /* number of bytes of first 16 schedulers per chunk */
#define WSCHEDLNUM 64 /* number of schedulers */

#ifndef CHAR_BIT
#define CHAR_BIT 8
#endif  /* CHAR_BIT */

struct sha2_chnk {
     uint32_t chnk_word[WSCHEDLNUM];
};


struct usha_ctx {
     uint32_t          ctx_hash[HASHVALNUM];
     FILE             *ctx_file;
     char             *ctx_buff;
     struct sha2_chnk *ctx_chnk;
     size_t            ctx_bsiz;
     uint64_t          ctx_clen;
     uint64_t          ctx_coff;
     int32_t           ctx_eror;
     bool              ctx_fret;
     char              ctx_padd[3];
};


static const uint32_t kval[] = {
     0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
     0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
     0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
     0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
     0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
     0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
     0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
     0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
     0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
     0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
     0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
     0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
     0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
     0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
     0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
     0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

#endif /* UNISHA256_H */
