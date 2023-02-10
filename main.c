#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "unisha256.h"

static struct usha_ctx ushactx = {0};

static void prnt_bits(const void *txt, const uint64_t size)
{
     for (uint64_t i = 0; i < size; ++i) {
	  if (0 == (i % 4))
	       printf("\n");
	  for (uint8_t j = 8; j > 0; --j) {
	       printf("%d", 1 & *((char *)txt + i) >> (j - 1));
	  }
	  printf(" ");
     }
     printf("\n");
}

static void conv_blks(struct usha_ctx *ctx, const char *txt, const uint64_t size)
{
     /* total number of bytes
      * we add +1 to the size because of the
      * additional 1 bit from the algorithm
      */
     uint64_t bnum = size;

     /* following the equation:
      * L + 1 + K = 448 mod 512
      */
     uint64_t calc = bnum + 8;
     calc %= 64;
     calc  = !!calc?
	  (64 - calc):
	  0;
     bnum += calc;
     bnum /= 56;

     ctx->ctx_blen = sizeof(struct sha2_blck) * bnum;
}

static void init_blks(struct usha_ctx *ctx, const char *txt, const uint64_t size)
{
     ctx->ctx_blks = malloc(ctx->ctx_blen);
     if (NULL == ctx->ctx_blks)
	  return;
     memset(ctx->ctx_blks, 0, ctx->ctx_blen);
     memcpy(ctx->ctx_blks, txt, size);

     uint32_t extr = 128;
     void *exby = (void *)((char *)ctx->ctx_blks + size - 1);
     memcpy(exby, &extr, sizeof extr);
}

int main(void)
{
     char text[] = "SAHAK";
     /* the byte for '\0' is considered
      * as the additional bit space
      */
     uint64_t tsiz = sizeof text;

     conv_blks(&ushactx, text, tsiz);
     init_blks(&ushactx, text, tsiz);

     prnt_bits(ushactx.ctx_blks, ushactx.ctx_blen);

     return EXIT_SUCCESS;
}
