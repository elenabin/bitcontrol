#ifndef _BIT_IO_H_
#define _BIT_IO_H_

#include <stdint.h>

// for read bits
#define BIT_IO_READ_BITS16(x) ((*x << 8) | *(x + 1))
#define BIT_IO_READ_BITS24(x) ((*x << 16) | (*(x + 1) << 8) | *(x + 2))
#define BIT_IO_READ_BITS32(x) ((*x << 24) | (*(x + 1) << 16) | (*(x + 2) << 8) | *(x + 3))

#define BIT_IO_READ_BITS32_LE(x) ((*x) | (*(x + 1) << 8) | (*(x + 2) << 16) | (*(x + 3) << 24))

inline int32_t READ_BITSINT24(uint8_t* x)
{    
    int32_t ret = 0;
    if( x[0] & 0xF0 == 0xF0 )
    {
        int8_t val1 = ~x[0];
        int8_t val2 = ~x[1];
        int8_t val3 = ~x[2];

        ret = (((val1 << 16 | val2 << 8 | val3) + 1) * -1);
    }
    else
    {
        ret = ((*x << 16) | (*(x + 1) << 8) | *(x + 2));
    }
    return ret;
}

// for write bits
#define WRITE_BITS8(output, val)          \
    {                                     \
        uint8_t *buf = (uint8_t *)output; \
        buf[0] = val;                     \
    }

#define WRITE_BITS16(output, val)         \
    {                                     \
        uint8_t *buf = (uint8_t *)output; \
        buf[1] = val & 0xff;              \
        buf[0] = val >> 8;                \
    }
#define WRITE_BITS24(output, val)         \
    {                                     \
        uint8_t *buf = (uint8_t *)output; \
        buf[2] = val & 0xff;              \
        buf[1] = (val & 0xff00) >> 8;     \
        buf[0] = val >> 16;               \
    }
#define WRITE_BITS32(output, val)         \
    {                                     \
        uint8_t *buf = (uint8_t *)output; \
        buf[3] = val & 0xff;              \
        buf[2] = (val & 0xff00) >> 8;     \
        buf[1] = (val & 0xff0000) >> 16;  \
        buf[0] = val >> 24;               \
    }

#endif
