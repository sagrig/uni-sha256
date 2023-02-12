#include <stdint.h>

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
