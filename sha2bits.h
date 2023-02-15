#include <stdint.h>

#include "unisha256.h"

static inline uint32_t bits_rotr(const uint32_t inpt, const uint8_t rotv)
{
     return (inpt >> rotv)|(inpt << (32 - rotv));
}

static inline uint32_t sigm_fun0(const uint32_t inpt)
{
     return bits_rotr(inpt, 7) ^ bits_rotr(inpt, 18) ^ (inpt >> 3);
}

static inline uint32_t sigm_fun1(const uint32_t inpt)
{
     return bits_rotr(inpt, 17) ^ bits_rotr(inpt, 19) ^ (inpt >> 10);
}

static inline uint32_t summ_fun0(const uint32_t inpt)
{
     return bits_rotr(inpt, 2) ^ bits_rotr(inpt, 13) ^ bits_rotr(inpt, 22);
}

static inline uint32_t summ_fun1(const uint32_t inpt)
{
     return bits_rotr(inpt, 6) ^ bits_rotr(inpt, 11) ^ bits_rotr(inpt, 25);
}

static inline uint32_t choi_func(const struct usha_ctx *ctx)
{
     return (ctx->ctx_hash[4] & ctx->ctx_hash[5]) ^
	   (~ctx->ctx_hash[4] & ctx->ctx_hash[6]);
}

static inline uint32_t majr_func(const struct usha_ctx *ctx)
{
     return (ctx->ctx_hash[0] & ctx->ctx_hash[1]) ^
	  (ctx->ctx_hash[0] & ctx->ctx_hash[2]) ^
	  (ctx->ctx_hash[1] & ctx->ctx_hash[2]);
}
