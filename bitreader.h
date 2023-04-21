#ifndef _CODEC_BIT_READER_H_
#define _CODEC_BIT_READER_H_

#include <stdint.h>

#define BYTE_BITS_LEN     (8)
#define BYTE32_BITS_COUNT (32)
#define BYTE64_BITS_COUNT (64)

#define BIT_BUFFER_ERROR_LEN    (256)
#define BIT_BUFFER_DEFAULT_SIZE (2048)

class CBitReader
{
public:
    CBitReader(uint8_t* buffer, int32_t size) { data_ = buffer; size_ = size; readbits_ = 0; }
    virtual ~CBitReader() { readbits_ = 0; }

    int32_t ReadBits(int32_t read)
    {
        int32_t ret = 0;
        char szError[BIT_BUFFER_ERROR_LEN] = { 0, };
        if(size_ <= 0) {
            sprintf(szError, "fail to read bits stream. buffer size is 0\n");
            fprintf(stderr, szError);
            return ret;
        }
        int32_t need = (read + readbits_ == 0) ? 0: (read + readbits_);
        if( need > (size_*BYTE_BITS_LEN) ) {
            sprintf(szError, "not enough you want is bit stream - want(%d)bits remain(%d)bits\n", need, ((size_*BYTE_BITS_LEN)-need));
            fprintf(stderr, szError);
            return ret;
        }
        if( (read <= 0) || (read > BYTE32_BITS_COUNT) ){
            sprintf(szError, "fail to read %d bits. not support over 32bits.\n", read);
            fprintf(stderr, szError);
            return ret;
        }
        
        for(int i = 0; i < read; i++)
        {
            ret |= ( ReadBit() << (read - (i + 1)) );
        }
        
        return ret;
    }

protected:
    uint8_t ReadBit()
    {
        char szError[BIT_BUFFER_ERROR_LEN] = { 0, };
        int32_t maxBits = size_ * BYTE_BITS_LEN;
        if( readbits_ > maxBits ){
            sprintf(szError, "fail to readbit over max bits! - read(%d) max(%d)\n", readbits_, maxBits);
            fprintf(stderr, szError);
            return 0;
        }
        
        int32_t index = (readbits_ == 0) ? 0: (readbits_ / BYTE_BITS_LEN); // change bytes
        int32_t pos = (readbits_ % BYTE_BITS_LEN) + 1; // current bit position!
        uint8_t* p = &data_[index];    // current pointer

        readbits_++;

        return ( (p[0] >> (BYTE_BITS_LEN - pos)) & 0x01 );
    }

private:    
    int32_t size_, readbits_;
    uint8_t* data_;
};

#endif
