#include "PasswordHasher.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <cstring>

using namespace std;

// *** SHA256 IMPLEMENTATION (pure C++) ***
/* ---- BEGIN SHA256 ---- */
typedef unsigned char uint8;
typedef unsigned int uint32;
typedef unsigned long long uint64;

#define SHA256_BLOCK_SIZE 32

typedef struct {
    uint8 data[64];
    uint32 datalen;
    uint64 bitlen;
    uint32 state[8];
} SHA256_CTX;

uint32 k[64] = {
    0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
    0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
    0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
    0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
    0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
    0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
    0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
    0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

uint32 ROTRIGHT(uint32 a, uint32 b) {
    return ((a >> b) | (a << (32-b)));
}

void SHA256Transform(SHA256_CTX *ctx, const uint8 data[]) {
    uint32 a, b, c, d, e, f, g, h, i, j, m[64], t1, t2;

    for (i=0, j=0; i < 16; ++i, j += 4)
        m[i] = (data[j] << 24) | (data[j+1] << 16) | (data[j+2] << 8) | (data[j+3]);
    for ( ; i < 64; ++i)
        m[i] = m[i-16] + (ROTRIGHT(m[i-15],7) ^ ROTRIGHT(m[i-15],18) ^ (m[i-15] >> 3))
                     + m[i-7] + (ROTRIGHT(m[i-2],17) ^ ROTRIGHT(m[i-2],19) ^ (m[i-2] >> 10));

    a=ctx->state[0]; b=ctx->state[1]; c=ctx->state[2]; d=ctx->state[3];
    e=ctx->state[4]; f=ctx->state[5]; g=ctx->state[6]; h=ctx->state[7];

    for (i = 0; i < 64; ++i) {
        t1 = h + (ROTRIGHT(e,6) ^ ROTRIGHT(e,11) ^ ROTRIGHT(e,25))
             + ((e & f) ^ (~e & g)) + k[i] + m[i];
        t2 = (ROTRIGHT(a,2) ^ ROTRIGHT(a,13) ^ ROTRIGHT(a,22))
             + ((a & b) ^ (a & c) ^ (b & c));
        h = g; g = f; f = e; e = d + t1;
        d = c; c = b; b = a; a = t1 + t2;
    }

    ctx->state[0] += a; ctx->state[1] += b;
    ctx->state[2] += c; ctx->state[3] += d;
    ctx->state[4] += e; ctx->state[5] += f;
    ctx->state[6] += g; ctx->state[7] += h;
}

void SHA256Init(SHA256_CTX *ctx) {
    ctx->datalen = 0;
    ctx->bitlen = 0;
    ctx->state[0] = 0x6a09e667;
    ctx->state[1] = 0xbb67ae85;
    ctx->state[2] = 0x3c6ef372;
    ctx->state[3] = 0xa54ff53a;
    ctx->state[4] = 0x510e527f;
    ctx->state[5] = 0x9b05688c;
    ctx->state[6] = 0x1f83d9ab;
    ctx->state[7] = 0x5be0cd19;
}

void SHA256Update(SHA256_CTX *ctx, const uint8 data[], uint32 len) {
    for (uint32 i=0; i < len; ++i) {
        ctx->data[ctx->datalen] = data[i];
        ctx->datalen++;

        if (ctx->datalen == 64) {
            SHA256Transform(ctx, ctx->data);
            ctx->bitlen += 512;
            ctx->datalen = 0;
        }
    }
}

void SHA256Final(SHA256_CTX *ctx, uint8 hash[]) {
    uint32 i = ctx->datalen;

    if (ctx->datalen < 56) {
        ctx->data[i++] = 0x80;
        while (i < 56)
            ctx->data[i++] = 0x00;
    } else {
        ctx->data[i++] = 0x80;
        while (i < 64)
            ctx->data[i++] = 0x00;

        SHA256Transform(ctx, ctx->data);
        memset(ctx->data, 0, 56);
    }

    ctx->bitlen += ctx->datalen * 8;
    ctx->data[63] = ctx->bitlen;
    ctx->data[62] = ctx->bitlen >> 8;
    ctx->data[61] = ctx->bitlen >> 16;
    ctx->data[60] = ctx->bitlen >> 24;
    ctx->data[59] = ctx->bitlen >> 32;
    ctx->data[58] = ctx->bitlen >> 40;
    ctx->data[57] = ctx->bitlen >> 48;
    ctx->data[56] = ctx->bitlen >> 56;

    SHA256Transform(ctx, ctx->data);

    for (i=0; i < 4; ++i) {
        hash[i]      = (ctx->state[0] >> (24 - i * 8)) & 0xff;
        hash[i+4]    = (ctx->state[1] >> (24 - i * 8)) & 0xff;
        hash[i+8]    = (ctx->state[2] >> (24 - i * 8)) & 0xff;
        hash[i+12]   = (ctx->state[3] >> (24 - i * 8)) & 0xff;
        hash[i+16]   = (ctx->state[4] >> (24 - i * 8)) & 0xff;
        hash[i+20]   = (ctx->state[5] >> (24 - i * 8)) & 0xff;
        hash[i+24]   = (ctx->state[6] >> (24 - i * 8)) & 0xff;
        hash[i+28]   = (ctx->state[7] >> (24 - i * 8)) & 0xff;
    }
}
/* ---- END SHA256 ---- */

string PasswordHasher::hash(const string& password) {
    if (password.empty()) return "";

    SHA256_CTX ctx;
    SHA256Init(&ctx);
    SHA256Update(&ctx, (uint8*)password.c_str(), password.size());

    uint8 hashResult[SHA256_BLOCK_SIZE];
    SHA256Final(&ctx, hashResult);

    stringstream ss;
    for (int i=0; i < SHA256_BLOCK_SIZE; i++)
        ss << hex << setw(2) << setfill('0') << (int)hashResult[i];

    return ss.str();
}

bool PasswordHasher::verify(const string& password, const string& hashed) {
    if (password.empty() || hashed.empty()) return false;
    return hash(password) == hashed;
}


