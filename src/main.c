/* Constants */
int32_t K0_19 = 0x5a827999;
int32_t K20_39 = 0x6ed9eba1;
int32_t K40_59 = 0x8f1bbcdc;
int32_t K60_79 = 0xca62c1d6;

/* Initial hash values */
int32_t h0_0 = 0x67452301;
int32_t h0_1 = 0xefcdab89;
int32_t h0_2 = 0x98badcfe;
int32_t h0_3 = 0x10325476;
int32_t h0_4 = 0xc3d2e1f0;

/* Functions */
int32_t
Ch(int32_t x, int32_t y, int32_t z)
{
    /* (x AND y) XOR ((NOT x) AND z)*/
    return (x & y) ^ ((~x) & z)
}

int32_t
Parity(int32_t x, int32_t y, int32_t z)
{
    /* x XOR y XOR z */
    return x ^ y ^ z
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
    unsigned char M[2] = { 'a', 'b' };
    int64_t l = 2;

    unsigned char *Mbuf = malloc(unsigned sizeof(char) * 20000); /* TODO: Fix me */
    memcpy(Mbuf, M, l);
    /* Preprocessing */
    /* Preprocessing consists of three steps: padding the message, M (Sec. 5.1), parsing the message
        into message blocks (Sec. 5.2), and setting the initial hash value, H(0) (Sec. 5.3). */

    /* Append the bit “1” to the end of the
        message, followed by k zero bits, where k is the smallest, non-negative solution to the equation l + 1 + k = 448 mod 512 */
    uint64_t k = 0;
    while (l + 1 + k != 448 % 512) k++; /* TODO: Simplify? */
    /* how to append a single bit? */

    /* Then append the 64-bit block that is equal to the number expressed
        using a binary representation */
    
    return 0;
}
