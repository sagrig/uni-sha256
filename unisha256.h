#ifndef UNISHA256_H
#define UNISHA256_H

#define WSCHEDLNUM 64U

struct sha2_chnk {
     uint32_t chnk_word[WSCHEDLNUM];
};

struct usha_ctx {
     struct sha2_chnk *ctx_chnk;
     uint64_t          ctx_clen;
     bool              ctx_fret;
};

#endif /* UNISHA256_H */
