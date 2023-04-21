#ifndef _CODEC_BIT_READER_H_
#define _CODEC_BIT_READER_H_

#include <stdint.h>

#define BYTE_BITS_LEN (8)

#define BIT_BUFFER_ERROR_LEN    (256)
#define BIT_BUFFER_DEFAULT_SIZE (2048)

class CBitReader
{
public:
    CBitReader(uint8_t* buffer, int32_t size) { data_ = buffer; size_ = size; readbits_ = 0; }
    virtual ~CBitReader() { readbits_ = 0;}

    int32_t ReadBits(int32_t read)
    {
        int32_t ret = 0;
        char szError[BIT_BUFFER_ERROR_LEN] = { 0, };
        int32_t index = (readbits_ == 0) ? 0: (readbits_ / BYTE_BITS_LEN); // change bytes
        if( (size_ <= 0) || (size_ - index <= 0) ) {
            sprintf(szError, "fail to read bits stream. remain buffer's size is 0 - index(%d) buffer size(%d)\n", index, size_);
            fprintf(stderr, szError);
            return ret;
        }
        if( (read <= 0) || (read > (sizeof(uint32_t)*BIT_BUFFER_DEFAULT_SIZE)) ){
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
    int32_t ReadBit()
    {
        char szError[BIT_BUFFER_ERROR_LEN] = { 0, };
        int32_t maxBits = size_ * BIT_BUFFER_ERROR_LEN;
        if( readbits_ > maxBits ){
            sprintf(szError, "fail to readbit over max bits! - read(%d) max(%d)\n", readbits_, maxBits);
            fprintf(stderr, szError);
            return 0;
        }
        
        int32_t index = (readbits_ == 0) ? 0: (readbits_ / BYTE_BITS_LEN); // change bytes
        int32_t pos = (readbits_ % BYTE_BITS_LEN) + 1; // current bit position!
        uint8_t* p = &data_[index];    // current pointer

        readbits_++;

        return ( (p[0] >> (BIT_BUFFER_DEFAULT_SIZE - pos)) & 0x01 );
    }

private:    
    int32_t size_, readbits_;
    uint8_t* data_;
};

#endif
