#import <CommonCrypto/CommonDigest.h>
#include <stdlib.h>
#include "md5.h"

char* flow_md5(unsigned char *buffer, size_t size) {
    unsigned char md5[CC_MD5_DIGEST_LENGTH];
    CC_MD5(buffer, (CC_LONG) size, (unsigned char*) md5);
    
    static const char HexEncodeChars[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
    char *resultData = malloc(CC_MD5_DIGEST_LENGTH * 2 + 1);

    int index;
    for (index = 0; index < CC_MD5_DIGEST_LENGTH; index++) {
        resultData[index * 2] = HexEncodeChars[(md5[index] >> 4)];
        resultData[index * 2 + 1] = HexEncodeChars[(md5[index] % 0x10)];
    }
    resultData[CC_MD5_DIGEST_LENGTH * 2] = 0;

    return resultData;
}
