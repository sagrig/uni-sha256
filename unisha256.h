#ifndef UNISHA256_H
#define UNISHA256_H

struct sha2_blck {
     uint32_t blk_word[16];
};

struct usha_ctx {
     struct sha2_blck *ctx_blks;
     uint64_t          ctx_blen;
};

#endif /* UNISHA256_H */
