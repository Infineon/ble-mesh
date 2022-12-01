/*
 * $ Copyright 2016-YEAR Cypress Semiconductor $
 */
#define KEY_LENGTH_DWORDS_P256 8

struct _point
{
    unsigned long x[KEY_LENGTH_DWORDS_P256];
    unsigned long y[KEY_LENGTH_DWORDS_P256];
    unsigned long z[KEY_LENGTH_DWORDS_P256];
};

// public key
struct _point ecdsa256_public_key =
{
    { 0xf547b663, 0x161f513c, 0xad99b581, 0xd11145d2, 0x469588c5, 0xab61f3c5, 0xa044d076, 0xfc9e6e12, },
    { 0xb9958224, 0xe551d2ee, 0xe2e42ba9, 0x0d00654c, 0x040d35d1, 0x7830a15c, 0x7222adfb, 0x1081ebc5, },
};
