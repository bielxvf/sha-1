/* SHA-1 implementation for learning purposes */
/* https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

/* The eighty constants */
uint32_t K0_19  = 0x5a827999;
uint32_t K20_39 = 0x6ed9eba1;
uint32_t K40_59 = 0x8f1bbcdc;
uint32_t K60_79 = 0xca62c1d6;

/* Initial hash values */
uint32_t h0_0 = 0x67452301;
uint32_t h0_1 = 0xefcdab89;
uint32_t h0_2 = 0x98badcfe;
uint32_t h0_3 = 0x10325476;
uint32_t h0_4 = 0xc3d2e1f0;

/* Fits 80 32-bit words */
#define BITSTREAM_MAX_BYTES 320
typedef struct {
    uint8_t data[BITSTREAM_MAX_BYTES];
    uint64_t len;
} BitStream;

static inline
void 
BitStream_append_bit(BitStream *bs, uint32_t bit)
{
    if (bs->len >= BITSTREAM_MAX_BYTES * 8 * sizeof(*bs->data)) return;
    size_t item_index = bs->len / (8 * sizeof(*bs->data));
    size_t bit_index = 7 - (bs->len % (8 * sizeof(*bs->data)));
    if (bit)
        bs->data[item_index] |= (1 << bit_index);
    else
        bs->data[item_index] &= ~(1 << bit_index);
    bs->len++;
}

/* SHA-1 functions */
static inline
uint32_t
rotr(uint32_t value, uint32_t count)
{
    return (value << count) | (value >> (8 * sizeof(value) - count));
}

static inline
int32_t
Ch(int32_t x, int32_t y, int32_t z)
{
    return (x & y) ^ ((~x) & z);
}

static inline
int32_t
Parity(int32_t x, int32_t y, int32_t z)
{
    return x ^ y ^ z;
}

static inline
int32_t
Maj(int32_t x, int32_t y, int32_t z)
{
    return (x & y) ^ (x & z) ^ (y & z);
}

int
main(int argc, char **argv)
{
    BitStream bs = {0};

    /* TODO: arguments, help, etc */

    /* Stdin reading */
    {
        int c;
        while ((c = getchar()) != EOF) {
            for (int i = 7; i >= 0; i--) {
                BitStream_append_bit(&bs, (c >> i) & 1);
            }
        }
    }

    /* 5.1.1 Padding */
    {
        uint64_t l = bs.len;
        BitStream_append_bit(&bs, 1);
        while ((bs.len % 512) != 448) {
            BitStream_append_bit(&bs, 0);
        }
        for (int8_t i = 63; i >= 0; i--) {
            BitStream_append_bit(&bs, (l >> i) & 1);
        }
    }

    /* 5.2.1 Parsing */
    /* For every 512-bit block (M) in the input BitStream */
    for (size_t offset = 0; offset < bs.len; offset += 512) {
        uint32_t W[80] = {0}; /* Message schedule */
        /* 6.1.2.1 Prepare the message schedule */
        for (int i = 0; i < 16; i++) {
            /* Get the word from our BitStream into the Message schedule */
            uint32_t word = 0;
            for (int j = 0; j < 4; j++) {
                size_t bit_index = offset + (i * 32) + (j * 8);
                for (int k = 0; k < 8; k++) {
                    word <<= 1;
                    size_t absolute_bit = bit_index + k;
                    size_t byte = absolute_bit / 8;
                    size_t bit = 7 - (absolute_bit % 8);
                    word |= (bs.data[byte] >> bit) & 1;
                }
            }
            W[i] = word;
        }

        /* 6.1.2.1 Prepare the message schedule */
        for (int t = 16; t < 80; t++) {
            W[t] = rotr(W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16], 1);
        }

        /* 6.1.2.2 Initialize the five working variables */
        uint32_t a = h0_0, b = h0_1, c = h0_2, d = h0_3, e = h0_4;

        /* 6.1.2.3 */
        for (int t = 0; t < 80; t++) {
            uint32_t f, k;
            if (t < 20) { 
                f = Ch(b, c, d); 
                k = K0_19; 
            } else if (t < 40) { 
                f = Parity(b, c, d); 
                k = K20_39; 
            } else if (t < 60) { 
                f = Maj(b, c, d); 
                k = K40_59; 
            } else { 
                f = Parity(b, c, d); 
                k = K60_79; 
            }

            uint32_t temp = rotr(a, 5) + f + e + k + W[t];
            e = d;
            d = c;
            c = rotr(b, 30);
            b = a;
            a = temp;
        }

        /* 6.1.2.4 Compute the ith intermediate hash value */
        h0_0 += a;
        h0_1 += b;
        h0_2 += c;
        h0_3 += d;
        h0_4 += e;
    }

    printf("%08x%08x%08x%08x%08x\n", h0_0, h0_1, h0_2, h0_3, h0_4);
    return 0;
}
