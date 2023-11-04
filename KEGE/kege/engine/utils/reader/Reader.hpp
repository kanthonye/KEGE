//
//  Reader.hpp
//  Reader
//
//  Created by Kenneth Esdaile on 7/19/23.
//

#ifndef Reader_hpp
#define Reader_hpp
#include <stdlib.h>
#include "kege-string.hpp"
namespace kege
{
    class Reader
    {
    public:

        //virtual bool operator == ( const char* str ) = 0;
        virtual uint64_t read( char* bufr, uint32_t length, const char* flags ) = 0;
        virtual kege::string gets( const char* flags )const = 0;
        virtual bool ischar( const char* str )const = 0;
        virtual uint64_t seek( const char* flags ) = 0;
        virtual uint64_t skip( const char* flags ) = 0;
        //virtual bool cmp( const char* str )const = 0;
        virtual bool eof() = 0;

//        virtual double getDouble()const = 0;
//        virtual float getFloat()const = 0;
//        virtual uint64_t getU64()const = 0;
//        virtual uint32_t getU32()const = 0;
//        virtual uint16_t getU16()const = 0;
//        virtual uint8_t getU8()const = 0;
//        virtual int64_t getI64()const = 0;
//        virtual int32_t getI32()const = 0;
//        virtual int16_t getI16()const = 0;
//        virtual int8_t getI8()const = 0;

    protected:

        virtual ~Reader(){}
        Reader() = default;
    };
}
#endif /* Reader_hpp */
