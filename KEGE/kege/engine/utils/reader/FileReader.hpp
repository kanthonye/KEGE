//
//  FileReader.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 7/18/23.
//

#ifndef FileReader_hpp
#define FileReader_hpp
#include <stdio.h>
#include "../reader/Reader.hpp"
namespace kege
{
    class FileReader : public kege::Reader
    {
    public:

        uint64_t read( char* bufr, uint32_t length, const char* flags );
        //bool operator == ( const char* str );
        bool ischar( const char* str )const;
        uint64_t seek( const char* flags );
        uint64_t skip( const char* flags );
        //bool cmp( const char* str )const;
        bool open( const char* str );
        bool eof();

//        double getDouble()const;
//        float getFloat()const;
//        uint64_t getU64()const;
//        uint32_t getU32()const;
//        uint16_t getU16()const;
//        uint8_t getU8()const;
//        int64_t getI64()const;
//        int32_t getI32()const;
//        int16_t getI16()const;
//        int8_t getI8()const;

        operator bool()const;
        
        FileReader( const kege::string& filename, const char* flags );
        FileReader( FILE* file );
        ~FileReader();
        FileReader();


        kege::string gets( const char* flags )const;
        int8_t getc()const;

    private:

        uint64_t _previous_position;
        uint64_t _current_position;
        //uint64_t _bufrsize;
        //char* _buffer;
        mutable char _char;
        FILE* _file;
    };
}
#endif /* FileReader_hpp */
