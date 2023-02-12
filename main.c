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
     return true;
eror:
     eror_hndl(__FUNCTION__, __LINE__, errno);
     return false;
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

     prnt_bits(ushactx.ctx_chnk, ushactx.ctx_clen * sizeof *ushactx.ctx_chnk);

     return EXIT_SUCCESS;
eror:
     eror_hndl(__FUNCTION__, __LINE__, errno);
     return EXIT_FAILURE;
}
