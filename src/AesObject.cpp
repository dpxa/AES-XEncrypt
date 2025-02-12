#include "../header/AesObject.h"
#include "../header/encrypted.h"
#include <vector>

// xor-ing bytes of state with bytes of round
void AesObject::addRoundKey(byte st[16], const byte rk[16]) {
    for (int i = 0; i < 16; ++i) {
        st[i] ^= rk[i];
    }
}

// substitute bytes with sbox
void AesObject::subBytes(byte st[16]) {
    for (int i = 0; i < 16; i++) {
        st[i] = sbox[st[i]];
    }
}

// substitute bytes with inverse of sbox
void AesObject::invSubBytes(byte st[16]) {
    for (int i = 0; i < 16; i++) {
        st[i] = rsbox[st[i]];
    }
}

// shift rows of state matrix
void AesObject::shiftRows(byte st[16]) {
    byte t = st[1];
    st[1] = st[5];
    st[5] = st[9];
    st[9] = st[13];
    st[13] = t;

    byte t1 = st[2];
    byte t2 = st[6];
    st[2] = st[10];
    st[6] = st[14];
    st[10] = t1;
    st[14] = t2;

    t = st[3];
    st[3] = st[15];
    st[15] = st[11];
    st[11] = st[7];
    st[7] = t;
}

// reverse shift of rows of state matrix
void AesObject::invShiftRows(byte st[16]) {
    byte t = st[13];
    st[13] = st[9];
    st[9] = st[5];
    st[5] = st[1];
    st[1] = t;

    byte t1 = st[2];
    byte t2 = st[6];
    st[2] = st[10];
    st[6] = st[14];
    st[10] = t1;
    st[14] = t2;

    t = st[7];
    st[7] = st[11];
    st[11] = st[15];
    st[15] = st[3];
    st[3] = t;
}

// multiply byte by 2 in GF(2^8) finite field
static inline byte xtime(byte x) {
    return (x << 1) ^ ((x & 0x80) ? 0x1B : 0);
}

// multiplication in GF(2^8)
static inline byte GFmul(byte x, byte y) {
    byte res = 0;

    while (y) {
        if (y & 1) {
            res ^= x;
        }

        x = xtime(x);
        y >>= 1;
    }

    return res;
}

// mix data within each column of state matrix
void AesObject::mixColumns(byte st[16]) {
    for (int c = 0; c < 4; c++) {
        int col = 4 * c;
        byte x0 = st[col], x1 = st[col + 1], x2 = st[col + 2], x3 = st[col + 3];

        st[col] = GFmul(x0, 2) ^ GFmul(x1, 3) ^ x2 ^ x3;
        st[col + 1] = x0 ^ GFmul(x1, 2) ^ GFmul(x2, 3) ^ x3;
        st[col + 2] = x0 ^ x1 ^ GFmul(x2, 2) ^ GFmul(x3, 3);
        st[col + 3] = GFmul(x0, 3) ^ x1 ^ x2 ^ GFmul(x3, 2);
    }
}

// reverse mixing process
void AesObject::invMixColumns(byte st[16]) {
    for (int c = 0; c < 4; c++) {
        int col = 4 * c;
        byte x0 = st[col], x1 = st[col + 1], x2 = st[col + 2], x3 = st[col + 3];

        st[col] = GFmul(x0, 0x0E) ^ GFmul(x1, 0x0B) ^ GFmul(x2, 0x0D) ^ GFmul(x3, 0x09);
        st[col + 1] = GFmul(x0, 0x09) ^ GFmul(x1, 0x0E) ^ GFmul(x2, 0x0B) ^ GFmul(x3, 0x0D);
        st[col + 2] = GFmul(x0, 0x0D) ^ GFmul(x1, 0x09) ^ GFmul(x2, 0x0E) ^ GFmul(x3, 0x0B);
        st[col + 3] = GFmul(x0, 0x0B) ^ GFmul(x1, 0x0D) ^ GFmul(x2, 0x09) ^ GFmul(x3, 0x0E);
    }
}

// expands key into full key schedule for all rounds
// Rijndael key expansion algorithm
void AesObject::keyEx(const byte key[AES_KEY_SIZE], byte rks[expKeySize]) {
    // copy into first part
    for (int i = 0; i < AES_KEY_SIZE; i++) {
        rks[i] = key[i];
    }

    int bytes = AES_KEY_SIZE;
    int ri = 0;
    // last 4 of exp key
    byte t[4];

    while (bytes < expKeySize) {
        for (int i = 0; i < 4; i++) {
            t[i] = rks[bytes - 4 + i];
        }

        // key schedule core every AES_KEY_SIZE bytes
        if (bytes % AES_KEY_SIZE == 0) {
            byte temp = t[0];
            t[0] = t[1];
            t[1] = t[2];
            t[2] = t[3];
            t[3] = temp;

            for (int i = 0; i < 4; i++) {
                t[i] = sbox[t[i]];
            }

            t[0] ^= Rcon[ri];
            ri++;
        }

        if (bytes % AES_KEY_SIZE == 16) {
            for (int i = 0; i < 4; i++) {
                t[i] = sbox[t[i]];
            }
        }

        for (int i = 0; i < 4; i++) {
            rks[bytes] = rks[bytes - AES_KEY_SIZE] ^ t[i];
            bytes++;
        }
    }
}

// encrypts single 16-byte block with the expanded round keys
void AesObject::encB(byte b[16], const byte rks[expKeySize]) {
    addRoundKey(b, &rks[0]);

    for (int r = 1; r < rounds; r++) {
        subBytes(b);
        shiftRows(b);
        mixColumns(b);
        addRoundKey(b, &rks[r * 16]);
    }

    subBytes(b);
    shiftRows(b);
    addRoundKey(b, &rks[expKeySize - 16]);
}

// decrypts single 16-byte block with the expanded round keys
void AesObject::decB(byte b[16], const byte rks[expKeySize]) {
    addRoundKey(b, &rks[expKeySize - 16]);
    invShiftRows(b);
    invSubBytes(b);

    for (int r = rounds - 1; r > 0; r--) {
        addRoundKey(b, &rks[r * 16]);
        invMixColumns(b);
        invShiftRows(b);
        invSubBytes(b);
    }

    addRoundKey(b, &rks[0]);
}

// PKCS#7 padding - ensure length is multiple of 16
void AesObject::Pad(std::vector<byte> &data) {
    size_t paddingV = 16 - (data.size() % 16);

    for (size_t i = 0; i < paddingV; ++i) {
        data.push_back(static_cast<byte>(paddingV));
    }
}

// trims padding bytes
void AesObject::Unpad(std::vector<byte> &data) {
    byte paddingV = data.back();

    data.resize(data.size() - paddingV);
}
