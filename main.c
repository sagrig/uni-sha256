#define _GNU_SOURCE

#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "eror_hand.h"
#include "sha2bits.h"
#include "unisha256.h"

static struct usha_ctx ushactx = {0};
static uint16_t errl = 0;

static void prnt_bits(const void *txt, const uint64_t size)
{
     unsigned char *byte = (unsigned char *)txt;
     for (uint64_t i = 0; i < size; ++i) {
	  if (0 == (i % 4))
	       printf("\n");
	  for (uint8_t j = 8; j > 0; --j) {
	       printf("%d", 1 & (byte[i] >> (j - 1)));
	  }
	  printf(" ");
     }
     printf("\n");
}

static void chnk_size(struct usha_ctx *ctx, const uint64_t size)
{
     /*
      * total number of bytes
      * we add +1 to the size because of the
      * additional 1 bit from the algorithm
      */
     ctx->ctx_clen = size;

     /*
      * following the equation:
      * L + 1 + K = 448 mod 512
      */
     uint64_t calc = ctx->ctx_clen + 8;
     calc %= 64;
     calc  = !!calc?
	  (64 - calc):
	  0;
     ctx->ctx_clen += calc;
     ctx->ctx_clen /= 56;
}

static bool chnk_init(struct usha_ctx *ctx, const char *txt, const uint64_t size)
{
     chnk_size(ctx, size);
     ctx->ctx_chnk = malloc(ctx->ctx_clen * sizeof(struct sha2_chnk));
     if (errl = __LINE__, NULL == ctx->ctx_chnk)
	  goto eror;

     for (uint32_t i = 0; i < ctx->ctx_clen; ++i) {
	  memcpy((struct sha2_chnk *)ctx->ctx_chnk + i,
		 txt,
		 (i != ctx->ctx_clen - 1)?(56):(size - 56 * i));
     }

     /*
      * Adding the extra bit so that L + 1 + K = 448 mod 512,
      * where L, K - length of the text (excluding '\0'), and
      * number of padded 0s.
      */
     void *exby = (struct sha2_chnk *)ctx->ctx_chnk + (ctx->ctx_clen - 1);
     exby = (void *)((char *)exby +
		     size - 1 - (size / (ctx->ctx_clen * 56)));
     uint8_t addb = 128;
     memcpy(exby, &addb, 1);

     /*
      * 64 bits in the last 512-bit block is reserved for the len
      * of the text (excluding '\0') in bits.
      */
     uint64_t tbsz = htobe64((size - 1) * 8);
     exby = (struct sha2_chnk *)ctx->ctx_chnk + ctx->ctx_clen;
     exby = (void *)((char *)exby -
		     48 * sizeof(uint32_t) -
		     sizeof tbsz);
     memcpy(exby, &tbsz, sizeof tbsz);

     return true;
eror:
     eror_hndl(__FUNCTION__, __LINE__, errno);
     return false;
}

static void chnk_work(struct usha_ctx *ctx)
{
     uint32_t *wsch = NULL;
     uint32_t tmp1 = 0;
     uint32_t tmp2 = 0;
     uint32_t work[8];

     for (uint32_t i = 0; i < ctx->ctx_clen; ++i) {
	  wsch = (uint32_t *)((struct sha2_chnk *)ctx->ctx_chnk + i);

	  for (uint8_t j = 0; j < 16; ++j) {
	       wsch[j] = htobe32(wsch[j]);
	  }
	  for (uint8_t j = 16; j < WSCHEDLNUM; ++j) {
	       wsch[j]  = 0;
	       wsch[j] += wsch[j-16];
	       wsch[j] += wsch[j-7];
	       wsch[j] += sigm_fun0(wsch[j-15]);
	       wsch[j] += sigm_fun1(wsch[j-2]);
	  }

	  for (uint8_t j = 0; j < 8; ++j) {
	       work[j] = ctx->ctx_hash[j];
	  }

	  for (uint8_t j = 0; j < WSCHEDLNUM; ++j) {
	       tmp1  = work[7]; // [h]
	       tmp1 += summ_fun1(work[4]); // pass [e]
	       tmp1 += choi_func(work);
	       tmp1 += kval[j];
	       tmp1 += wsch[j];

	       tmp2  = summ_fun0(work[0]); // pass [a]
	       tmp2 += majr_func(work);

	       work[7] = work[6];        // [h] = [g]
	       work[6] = work[5];        // [g] = [f]
	       work[5] = work[4];        // [f] = [e]
	       work[4] = work[3] + tmp1; // [e] = [d] + [t1]
	       work[3] = work[2];        // [d] = [c]
	       work[2] = work[1];        // [c] = [b]
	       work[1] = work[0];        // [b] = [a]
	       work[0] = tmp1 + tmp2;             // [a] = [t1] + [t2]
	  }

	  for (uint8_t j = 0; j < 8; ++j) {
	       ctx->ctx_hash[j] += work[j];
	       printf("%u: %u\n", j, ctx->ctx_hash[j]);
	  }
     }
}

static void hash_init(struct usha_ctx *ctx)
{
     ctx->ctx_hash[0] = 0x6a09e667;
     ctx->ctx_hash[1] = 0xbb67ae85;
     ctx->ctx_hash[2] = 0x3c6ef372;
     ctx->ctx_hash[3] = 0xa54ff53a;
     ctx->ctx_hash[4] = 0x510e527f;
     ctx->ctx_hash[5] = 0x9b05688c;
     ctx->ctx_hash[6] = 0x1f83d9ab;
     ctx->ctx_hash[7] = 0x5be0cd19;
}

int main(void)
{
     /* the byte for '\0' is considered
      * as the additional bit space
      */
     char text[] = "SAHAK";
     uint64_t tsiz = sizeof text;

     ushactx.ctx_fret = chnk_init(&ushactx, text, tsiz);
     if (errl = __LINE__, false == ushactx.ctx_fret)
	  goto eror;

     hash_init(&ushactx);
     chnk_work(&ushactx);

     prnt_bits(ushactx.ctx_chnk, ushactx.ctx_clen * sizeof *ushactx.ctx_chnk);
     return EXIT_SUCCESS;
eror:
     eror_hndl(__FUNCTION__, __LINE__, errno);
     return EXIT_FAILURE;
}
