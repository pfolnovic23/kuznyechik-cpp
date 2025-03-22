#include <iostream>
#include <array>
#include <cstdint>
#include <vector>
#include <iomanip>
#include <cstring>

using namespace std;

using uint128_t = array<uint64_t, 2>; 
using uint256_t = array<uint64_t, 4>; 

void generateRoundKeys(const uint256_t& masterKey, array<uint128_t, 10>& roundKeys);
void F(const uint8_t* key, uint8_t* a1, uint8_t* a0);
void X(uint8_t* state, const uint8_t* key);
void L(uint8_t* state);
void L_inv(uint8_t* state);
void S(uint8_t* state);
void S_inv(uint8_t* state);
void R(uint8_t* state);
void R_inv(uint8_t* state);
uint8_t gf_mul(uint8_t a, uint8_t b);   

const uint8_t Pi[256] = {
    252, 238, 221,  17, 207, 110,  49,  22, 251, 196, 250, 218,  35, 197,   4,  77,
    233, 119, 240, 219, 147,  46, 153, 186,  23,  54, 241, 187,  20, 205,  95, 193,
    249,  24, 101,  90, 226,  92, 239,  33, 129,  28,  60,  66, 139,   1, 142,  79,
      5, 132,   2, 174, 227, 106, 143, 160,   6,  11, 237, 152, 127, 212, 211,  31,
    235,  52,  44,  81, 234, 200,  72, 171, 242,  42, 104, 162, 253,  58, 206, 204,
    181, 112,  14,  86,   8,  12, 118,  18, 191, 114,  19,  71, 156, 183,  93, 135,
     21, 161, 150,  41,  16, 123, 154, 199, 243, 145, 120, 111, 157, 158, 178, 177,
     50, 117,  25,  61, 255,  53, 138, 126, 109,  84, 198, 128, 195, 189,  13,  87,
    223, 245,  36, 169,  62, 168,  67, 201, 215, 121, 214, 246, 124,  34, 185,   3,
    224,  15, 236, 222, 122, 148, 176, 188, 220, 232,  40,  80,  78,  51,  10,  74,
    167, 151,  96, 115,  30,   0,  98,  68,  26, 184,  56, 130, 100, 159,  38,  65,
    173,  69,  70, 146,  39,  94,  85,  47, 140, 163, 165, 125, 105, 213, 149,  59,
      7,  88, 179,  64, 134, 172,  29, 247,  48,  55, 107, 228, 136, 217, 231, 137,
    225,  27, 131,  73,  76,  63, 248, 254, 141,  83, 170, 144, 202, 216, 133,  97,
     32, 113, 103, 164,  45,  43,   9,  91, 203, 155,  37, 208, 190, 229, 108,  82,
     89, 166, 116, 210, 230, 244, 180, 192, 209, 102, 175, 194,  57,  75,  99, 182
};

const uint8_t Pi_inv[256] = {
    165,  45,  50, 143,  14,  48,  56, 192,  84, 230, 158,  57,  85, 126,  82, 145,
    100,   3,  87,  90,  28,  96,   7,  24,  33, 114, 168, 209,  41, 198, 164,  63,
    224,  39, 141,  12, 130, 234, 174, 180, 154,  99,  73, 229,  66, 228,  21, 183,
    200,   6, 112, 157,  65, 117,  25, 201, 170, 252,  77, 191,  42, 115, 132, 213,
    195, 175,  43, 134, 167, 177, 178,  91,  70, 211, 159, 253, 212,  15, 156,  47,
    155,  67, 239, 217, 121, 182,  83, 127, 193, 240,  35, 231,  37,  94, 181,  30,
    162, 223, 166, 254, 172,  34, 249, 226,  74, 188,  53, 202, 238, 120,   5, 107,
     81, 225,  89, 163, 242, 113,  86,  17, 106, 137, 148, 101, 140, 187, 119,  60,
    123,  40, 171, 210,  49, 222, 196,  95, 204, 207, 118,  44, 184, 216,  46,  54,
    219, 105, 179,  20, 149, 190,  98, 161,  59,  22, 102, 233,  92, 108, 109, 173,
     55,  97,  75, 185, 227, 186, 241, 160, 133, 131, 218,  71, 197, 176,  51, 250,
    150, 111, 110, 194, 246,  80, 255,  93, 169, 142,  23,  27, 151, 125, 236,  88,
    247,  31, 251, 124,   9,  13, 122, 103,  69, 135, 220, 232,  79,  29,  78,   4,
    235, 248, 243,  62,  61, 189, 138, 136, 221, 205,  11,  19, 152,   2, 147, 128,
    144, 208,  36,  52, 203, 237, 244, 206, 153,  16,  68,  64, 146,  58,   1,  38,
     18,  26,  72, 104, 245, 129, 139, 199, 214,  32,  10,   8,   0,  76, 215, 116
};


const uint16_t field_polynomial = 0x1C3; // x^8 + x^7 + x^6 + x + 1

uint8_t gf_mul(uint8_t a, uint8_t b) {
    uint8_t p = 0; 
    uint8_t carry;

    for (int i = 0; i < 8; i++) {
        if (b & 1) {
            p ^= a; 
        }
        carry = a & 0x80;
        a <<= 1; 
        if (carry) {
            a ^= 0xC3; 
        }
        b >>= 1; 
    }
    return p;
}


void S(uint8_t* state) {
    for (int i = 0; i < 16; i++) {
        state[i] = Pi[state[i]];
    }
}

void L(uint8_t* state) {
    for (int i = 0; i < 16; i++) {
        R(state);
    }
}

void R(uint8_t* state) {
    static const uint8_t c[16] = {
        0x94, 0x20, 0x85, 0x10, 0xc2, 0xc0, 0x01, 0xfb,
        0x01, 0xc0, 0xc2, 0x10, 0x85, 0x20, 0x94, 0x01
    };
    uint8_t a15_new = 0;

    for (int i = 0; i < 16; i++) {
        a15_new ^= gf_mul(state[i], c[i]);
    }

    memmove(state + 1, state, 15);
    state[0] = a15_new;
}

void S_inv(uint8_t* state) {
    for (int i = 0; i < 16; i++) {
        state[i] = Pi_inv[state[i]];
    }
}

void L_inv(uint8_t* state) {
    for (int i = 0; i < 16; i++) {
        R_inv(state);
    }
}

void R_inv(uint8_t* state) {
    static const uint8_t c[16] = {
        0x94, 0x20, 0x85, 0x10, 0xc2, 0xc0, 0x01, 0xfb,
        0x01, 0xc0, 0xc2, 0x10, 0x85, 0x20, 0x94, 0x01
    };

    uint8_t b0 = state[0];

    memmove(state, state + 1, 15);

    uint8_t partial = 0;
    for (int i = 0; i < 15; i++) {
        partial ^= gf_mul(state[i], c[i]);
    }

    state[15] = b0 ^ partial;
}

void X(uint8_t* state, const uint8_t* key) {
    for (int i = 0; i < 16; i++) {
        state[i] ^= key[i];
    }
}

void F(const uint8_t* key, uint8_t* a1, uint8_t* a0) {
    uint8_t temp[16];

    for (int i = 0; i < 16; i++) {
        temp[i] = a1[i] ^ key[i];
    }

    S(temp);

    L(temp);

    for (int i = 0; i < 16; i++) {
        temp[i] ^= a0[i];
    }

    memcpy(a0, a1, 16);   
    memcpy(a1, temp, 16); 
}


void uint128_to_bytes(const uint128_t& value, uint8_t* bytes) {
    for (int i = 0; i < 8; i++) {
        bytes[i] = (value[0] >> (56 - 8 * i)) & 0xFF;
        bytes[i + 8] = (value[1] >> (56 - 8 * i)) & 0xFF;
    }
}

void bytes_to_uint128(const uint8_t* bytes, uint128_t& value) {
    value[0] = 0;
    value[1] = 0;
    for (int i = 0; i < 8; i++) {
        value[0] |= static_cast<uint64_t>(bytes[i]) << (56 - 8 * i);
        value[1] |= static_cast<uint64_t>(bytes[i + 8]) << (56 - 8 * i);
    }
}

void int_to_vec128(uint32_t i, uint8_t* vec) {
    memset(vec, 0, 16);
    vec[15] = (i >> 0) & 0xFF;
    vec[14] = (i >> 8) & 0xFF;
    vec[13] = (i >> 16) & 0xFF;
    vec[12] = (i >> 24) & 0xFF;
}

void generateRoundKeys(const uint256_t& masterKey,
                       array<uint128_t, 10>& roundKeys) 
{
    roundKeys[0] = { masterKey[0], masterKey[1] };  
    roundKeys[1] = { masterKey[2], masterKey[3] };  

    vector<array<uint8_t, 16>> constants(32);
    for (int i = 0; i < 32; i++) {
        uint8_t vec[16] = { 0 };
        int_to_vec128(i + 1, vec);
        L(vec);                     
        copy(vec, vec + 16, constants[i].begin());
    }

    for (int i = 1; i <= 4; i++) {

        uint8_t a1[16], a0[16];
        uint128_to_bytes(roundKeys[2 * i - 2], a1);  
        uint128_to_bytes(roundKeys[2 * i - 1], a0); 

        for (int j = 0; j < 8; j++) {
            uint8_t k[16];
            copy(constants[8 * (i - 1) + j].begin(),
                      constants[8 * (i - 1) + j].end(),
                      k);
            
            F(k, a1, a0);
        }

        bytes_to_uint128(a1, roundKeys[2 * i]);
        bytes_to_uint128(a0, roundKeys[2 * i + 1]);
    }
}


uint128_t encryptBlock(const uint128_t& plaintext, const array<uint128_t, 10>& roundKeys) {
    uint8_t state[16];
   
    for (int i = 0; i < 8; i++) {
        state[i] = (plaintext[0] >> (56 - 8 * i)) & 0xFF;
        state[i + 8] = (plaintext[1] >> (56 - 8 * i)) & 0xFF;
    }

    uint8_t key[16];

    
    for (int i = 0; i < 9; i++) {
        
        for (int j = 0; j < 8; j++) {
            key[j] = (roundKeys[i][0] >> (56 - 8 * j)) & 0xFF;
            key[j + 8] = (roundKeys[i][1] >> (56 - 8 * j)) & 0xFF;
        }
        X(state, key);

        S(state);

        L(state);
    }

    for (int j = 0; j < 8; j++) {
        key[j] = (roundKeys[9][0] >> (56 - 8 * j)) & 0xFF;
        key[j + 8] = (roundKeys[9][1] >> (56 - 8 * j)) & 0xFF;
    }
    X(state, key);

    uint128_t ciphertext;
    ciphertext[0] = 0;
    ciphertext[1] = 0;
    for (int i = 0; i < 8; i++) {
        ciphertext[0] |= static_cast<uint64_t>(state[i]) << (56 - 8 * i);
        ciphertext[1] |= static_cast<uint64_t>(state[i + 8]) << (56 - 8 * i);
    }
    return ciphertext;
}

void print_uint128_hex(const uint128_t& value) {
    cout << hex << setw(16) << setfill('0') << value[0]
              << setw(16) << setfill('0') << value[1] << dec << endl;
}

uint128_t decryptBlock(const uint128_t& ciphertext, const array<uint128_t, 10>& roundKeys) {
    uint8_t state[16];
    uint128_to_bytes(ciphertext, state);

    {
        uint8_t key[16];
        uint128_to_bytes(roundKeys[9], key);
        X(state, key);
    }

    for (int i = 9; i > 0; i--) {
        L_inv(state);
        S_inv(state);

        uint8_t key[16];
        uint128_to_bytes(roundKeys[i - 1], key);
        X(state, key);
    }

    uint128_t plaintext{0, 0};
    bytes_to_uint128(state, plaintext);
    return plaintext;
}

int main() {
    int choice;
    cout << "Odaberite opciju:\n";
    cout << "1. Testiraj enkripciju\n";
    cout << "2. Testiraj dekripciju\n";
    cout << "Unesite svoj odabir: ";
    cin >> choice;

    uint8_t masterKeyBytes[32] = {
        0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
        0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
        0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10,
        0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef
    };

    uint256_t masterKey;
    for (int i = 0; i < 4; i++) {
        masterKey[i] = 0;
        for (int j = 0; j < 8; j++) {
            masterKey[i] |= (uint64_t)masterKeyBytes[i * 8 + j] << (56 - 8 * j);
        }
    }

    array<uint128_t, 10> roundKeys;
    generateRoundKeys(masterKey, roundKeys);

    if (choice == 1) {
        uint128_t plaintext = { 0x1122334455667700, 0xffeeddccbbaa9988 };
        uint128_t ciphertext = encryptBlock(plaintext, roundKeys);

        cout << "Plaintext:  ";
        print_uint128_hex(plaintext);
        cout << "Ciphertext: ";
        print_uint128_hex(ciphertext);
    } else if (choice == 2) {
        uint128_t ciphertext = { 0x7f679d90bebc2430, 0x5a468d42b9d4edcd };
        uint128_t plaintext = decryptBlock(ciphertext, roundKeys);

        cout << "Ciphertext: ";
        print_uint128_hex(ciphertext);
        cout << "Decrypted plaintext: ";
        print_uint128_hex(plaintext);
    } else {
        cout << "Neispravan unos." << endl;
    }

    return 0;
}   