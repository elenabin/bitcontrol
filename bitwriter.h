#ifndef _BIT_WRITER_H_
#define _BIT_WRITER_H_

#include <stdio.h>
#include <stdint.h>

#define BIT_BUFFER_ERROR_LEN (256)

#define BIT_BYTE_BITS_LEN    (8)

#define BIT_BYTE32_BITS_COUNT (32)

/**
 * @brief       Write Bit Writer
 * @details
 */
class CBitWriter
{
public:
    CBitWriter(uint8_t* buffer, int32_t size) { data_ = buffer; size_ = size; writebits_ = 0; }
    virtual ~CBitWriter() {}

    /**
     * @brief       write bits in data_
     * @param[in]   int32_t write
     * @param[in]   int32_t value
     * @return      uint32_t write bits to data_
     * @note        if value is 0 or 1 is all write writeBits length, over 1, consider as a number, will be write to data_
     */
    int32_t WriteBits(int32_t write, int32_t value, bool enable = false )
    {
        int32_t ret = 0;
        char szError[BIT_BUFFER_ERROR_LEN] = { 0, };
        if(size_ <= 0) {
            sprintf(szError, "fail to write bits stream. buffer size is 0\n");
            fprintf(stderr, szError);
            return ret;
        }
        int32_t maxBits = size_ * BIT_BYTE_BITS_LEN;
        if( (write + writebits_) > maxBits ) {
            sprintf(szError, "not enough you want is bit stream - want(%d)bits write(%d),max(%d)bits\n", write, writebits_, maxBits);
            fprintf(stderr, szError);
            return ret;
        }
        if( write <= 0 ){
            sprintf(szError, "fail to write %d write bits.\n", write);
            fprintf(stderr, szError);
            return ret;
        }

        /*
        *   1. value 0 or 1 is bit!
        *   2. value over 1, value is number...
        */
        if( value <= 1) 
        {
            if( enable == true )
            {
                for(int32_t i = 0; i < write; i++)
                {
                    if( i == (write - 1) )
                    {
                        WriteBit(value);
                    }
                    else
                    {
                        WriteBit(0);
                    }
                }
            }
            else 
            {
                for(int32_t i = 0; i < write; i++) {
                    WriteBit(value);
                }
            }
        }
        else 
        {
            uint8_t szBinary[BIT_BYTE32_BITS_COUNT] = { 0, };
            int32_t digit = 0, num = value;
            for(digit = 0; digit < BIT_BYTE32_BITS_COUNT; digit++){
                if( num == 0 ) break;
                szBinary[digit] = (num % 2 == 1)? 1 : 0;
                num /= 2;
            }
            for( int32_t i = 0, j = 0; i < write; i++) {
                if(i < (write - digit)){
                    WriteBit(0);
                }
                else {
                    WriteBit(szBinary[(digit-1) - j]);
                    j++;
                }
            }
        }        
        return ret;
    }

protected:
    uint8_t WriteBit(uint32_t bit)
    {
        int32_t index = (writebits_ == 0) ? 0: (writebits_ / BIT_BYTE_BITS_LEN); // change bytes
        int32_t pos = (writebits_ % BIT_BYTE_BITS_LEN); // current bit position!
        uint8_t* p = &data_[index];    // current pointer

        writebits_++;

        if( bit == 1 ){
            p[0] |= (bit << ((BIT_BYTE_BITS_LEN - 1) - pos));
        }

        return (uint8_t)bit;
    }

private:    
    int32_t size_, writebits_;
    uint8_t* data_;    
};

#endif
