#include <cstdio>
#include <cstring>
#include <string>
#include "base64.h"

using std::string;

const char conv[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


static inline string asc2b64_seg(char c1, char c2 = 0, char c3 = 0){
    unsigned int buff = (c1 << 24) + (c2 << 16) + (c3 << 8);
    string out;
    for (int i = 0; i < 4 ; i ++){
        //printf("%lu, %u\n", sizeof(unsigned int), buff >> 26);
        out += conv[buff >> 26];
        buff <<= 6;
    }
    return out;
}

/*
 * pass source ascii as well as an pointer of base64 string to be stored.
 * please make sure that pointer base64 have enough space.
 *
 */
string base64::encode(string ascii){
    string base64;
    while (ascii.length() > 2){
        base64 += asc2b64_seg(ascii[0], ascii[1], ascii[2]);
        ascii = ascii.substr(3);
    }
    if (ascii.length() == 1){
        base64 += asc2b64_seg(ascii[0]);
        base64 = base64.substr(0, base64.length() - 2) + "==";
    }
    
    if (ascii.length() == 2){
        base64 += asc2b64_seg(ascii[0], ascii[1]);
        base64 = base64.substr(0, base64.length() - 1) + "=";
    }
    return base64;
}

/*
 * pass source base64 as well as an pointer of ascii string to be stored.
 * please make sure that pointer ascii have enough space.
 *
 */
string base64::decode(string base64){
    string ascii;
    if (base64.length() % 4 != 0) fprintf(stderr, "BASE 64 STRING IS INCOMPLETE!\n");
    
    unsigned long buf;
    while (base64.length() != 0){
        buf = 0;
        for (int i = 0; i < 4; i ++){
            buf <<= 6;
            if (base64[0] >= 'A' && base64[0] <= 'Z') buf += base64[0] - 'A';
            else if (base64[0] >= 'a' && base64[0] <= 'z') buf += base64[0] - 'a' + 26;
            else if (base64[0] >= '0' && base64[0] <= '9') buf += base64[0] - '0' + 52;
            else if (base64[0] == '+')buf += 62;
            else if (base64[0] == '/')buf += 63;
            else if (base64[0] == '=')buf += 0;
            base64 = base64.substr(1);
        }
        ascii += (char)(buf >> 16);
        ascii += (char)(buf >> 8);
        ascii += (char)buf;
    }
    return ascii;
}
