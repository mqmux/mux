#ifndef MUX_RSA_H
#define MUX_RSA_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <stdlib.h>
#include <time.h>

uint64_t gcd(uint64_t a, uint64_t b) {
    while (b != 0) {
        uint64_t temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

uint64_t mod_inverse(uint64_t a, uint64_t m) {
    uint64_t m0 = m, t, q;
    int64_t x0 = 0, x1 = 1;

    if (m == 1) return 0;

    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m, a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

    if (x1 < 0) x1 += m0;

    return x1;
}

uint64_t power(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) result = (result * base) % mod;
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

int is_prime(uint64_t num) {
    if (num <= 1) return 0;
    if (num <= 3) return 1;
    if (num % 2 == 0 || num % 3 == 0) return 0;

    for (uint64_t i = 5; i * i <= num; i += 6) {
        if (num % i == 0 || num % (i + 2) == 0) return 0;
    }
    return 1;
}

uint64_t generate_prime(uint64_t min, uint64_t max) {
    uint64_t num;
    do {
        num = min + rand() % (max - min);
    } while (!is_prime(num));
    return num;
}

void generate_keys(uint64_t* n, uint64_t* e, uint64_t* d) {
    srand(time(NULL));

    uint64_t p, q;
    do {
        p = generate_prime(50, 500);
        q = generate_prime(50, 500);
    } while (p == q);

    *n = p * q;
    uint64_t phi = (p - 1) * (q - 1);

    *e = 17;
    while (gcd(*e, phi) != 1) (*e)++;

    *d = mod_inverse(*e, phi);
}

uint64_t encrypt(uint64_t plaintext, uint64_t e, uint64_t n) {
    return power(plaintext, e, n);
}

uint64_t decrypt(uint64_t ciphertext, uint64_t d, uint64_t n) {
    return power(ciphertext, d, n);
}

void encrypt_string(const char* str, uint64_t e, uint64_t n, uint64_t* encrypted, size_t length) {
    for (size_t i = 0; i < length; i++) {
        encrypted[i] = encrypt((uint64_t)str[i], e, n);
    }
}

void decrypt_string(uint64_t* encrypted, size_t length, uint64_t d, uint64_t n, char* decrypted) {
    for (size_t i = 0; i < length; i++) {
        decrypted[i] = (char)decrypt(encrypted[i], d, n);
    }
    decrypted[length] = '\0';
}
#endif