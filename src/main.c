#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

int32_t K0_19 = 0x5a827999;
int32_t K20_39 = 0x6ed9eba1;
int32_t K40_59 = 0x8f1bbcdc;
int32_t K60_79 = 0xca62c1d6;

int32_t h0_0 = 0x67452301;
int32_t h0_1 = 0xefcdab89;
int32_t h0_2 = 0x98badcfe;
int32_t h0_3 = 0x10325476;
int32_t h0_4 = 0xc3d2e1f0;

/* BitStream, constants, struct and functions */
#define BITSTREAM_MAX_BYTES 2048
typedef struct {
    uint8_t data[BITSTREAM_MAX_BYTES];
    uint64_t len;
} BitStream;

void
BitStream_append_bit(BitStream *bs, uint32_t bit)
{
    if (bs->len >= BITSTREAM_MAX_BYTES * 8) {
        return;
    }

    size_t byte_index = bs->len / 8; /* Last byte */
    size_t bit_index = 7 - (bs->len % 8); /* Last free bit */

    if (bit) {
        bs->data[byte_index] |= (1 << bit_index); /* Shift the bit to position like 00010000 and OR it in */
    } else {
        bs->data[byte_index] &= ~(1 << bit_index); /* Shift the bit to position, flip all bits like 11101111 and AND */
    }

    bs->len++;
}

/* Functions */
int32_t
Ch(int32_t x, int32_t y, int32_t z)
{
    /* (x AND y) XOR ((NOT x) AND z)*/
    return (x & y) ^ ((~x) & z);
}

int32_t
Parity(int32_t x, int32_t y, int32_t z)
{
    /* x XOR y XOR z */
    return x ^ y ^ z;
}

int32_t
Maj(int32_t x, int32_t y, int32_t z)
{
    /* (x AND y) XOR (x AND z) XOR (y AND z) */
    return (x & y) ^ (x & z) ^ (y & z);
}

int
main(int argc, char **argv)
{
    /* Input for testing purposes */
    BitStream bs = {0};
    BitStream_append_bit(&bs, 1);
    BitStream_append_bit(&bs, 1);
    BitStream_append_bit(&bs, 0);
    BitStream_append_bit(&bs, 1);

    /* Preprocessing */
    /* Preprocessing consists of three steps: padding the message, M (Sec. 5.1), parsing the message
        into message blocks (Sec. 5.2), and setting the initial hash value, H(0) (Sec. 5.3). */

    /* Append the bit “1” to the end of the
        message, followed by k zero bits, where k is the smallest, non-negative solution to the equation l + 1 + k = 448 mod 512 */

    uint64_t l = bs.len;
    BitStream_append_bit(&bs, 1);
    while ((bs.len % 512) != 448) {
        BitStream_append_bit(&bs, 0);
    }

    /* Then append the 64-bit block that is equal to the number l expressed
        using a binary representation */
    size_t byte_index = bs.len / 8; /* Last byte */
    size_t bit_index = 7 - (bs.len % 8); /* Last free bit */

    /* append "l" as 64 bits */
    /* which bits need to be set? */
    printf("%d\n", l);
    int length[64] = {0};
    
    return 0;
}
