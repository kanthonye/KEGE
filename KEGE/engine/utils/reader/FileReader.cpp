//
//  FileReader.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 7/18/23.
//

#include "FileReader.hpp"
namespace kege
{
//    bool FileReader::operator == ( const char* str )
//    {
//        return cmp( str );
//    }

    bool FileReader::ischar( const char* flags )const
    {
        while ( *flags != _char && *flags != 0 ) flags++;
        return *flags != 0;
    }

    uint64_t FileReader::seek( const char* flags )
    {
        const char* f =  nullptr;
        _current_position = ftell( _file );

        while ( _char != EOF )
        {
            f = flags;
            if ( ischar( f ) )
            {
                break;
            }
            else
            {
                getc();
            }
        }
        return _char != EOF;
    }

    uint64_t FileReader::skip( const char* flags )
    {
        const char* f =  nullptr;
        _current_position = ftell( _file );

        while ( _char != EOF )
        {
            f = flags;
            if ( ischar( f ) )
            {
                getc();
            }
            else
            {
                break;
            }
        }
        return _char != EOF;
    }

    uint64_t FileReader::read( char* bufr, uint32_t length, const char* flags )
    {
        uint32_t count = 0;
        const char* f = nullptr;
        _previous_position = _current_position;

        while ( _char != EOF && count < length && !ischar( flags ) )
        {
            bufr[ count++ ] = _char;
            getc();
        }
        _current_position = ftell( _file );
        bufr[ count ] = 0;
        return count;
    }

//    bool FileReader::cmp( const char* str )const
//    {
//        const char* s = _buffer;
//        while ( *s == *str && *s != 0 && *str != 0 )
//        {
//            str++;
//            s++;
//        }
//        if ( (*s != 0 && *str == 0) || (*s == 0 && *str != 0) )
//        {
//            return false;
//        }
//        return *s == *str;
//    }

    bool FileReader::open( const char* str )
    {
        if ( _file != nullptr )
        {
            fclose( _file );
        }
//        if ( _buffer == nullptr )
//        {
//            _buffer = new char[ _bufrsize ];
//        }
        _file = fopen( str, "r");
        getc();
        return _file != nullptr;
    }

//    const char* FileReader::getstr()const
//    {
//        return _buffer;
//    }
//
//    double FileReader::getDouble()const
//    {
//        return atof( _buffer );
//    }
//
//    float FileReader::getFloat()const
//    {
//        return atof( _buffer );
//    }
//
//    uint64_t FileReader::getU64()const
//    {
//        return atoll( _buffer );
//    }
//    uint32_t FileReader::getU32()const
//    {
//        return atoi( _buffer );
//    }
//    uint16_t FileReader::getU16()const
//    {
//        return atoi( _buffer );
//    }
//    uint8_t FileReader::getU8()const
//    {
//        return atoi( _buffer );
//    }
//    int64_t FileReader::getI64()const
//    {
//        return atoll( _buffer );
//    }
//    int32_t FileReader::getI32()const
//    {
//        return atoi( _buffer );
//    }
//    int16_t FileReader::getI16()const
//    {
//        return atoi( _buffer );
//    }
//    int8_t FileReader::getI8()const
//    {
//        return atoi( _buffer );
//    }

    bool FileReader::eof()
    {
        return _char == EOF || _file == nullptr;
    }

    kege::string FileReader::gets( const char* flags )const
    {
        char s[64];
        uint32_t i = 0;
        kege::string str;
        while ( !ischar( flags ) && _char != EOF )
        {
            s[ i++ ] = _char;
            if ( i >= 63 )
            {
                s[i] = 0;
                str += s;
            }
            getc();
        }
        s[i] = 0;
        str += s;
        return str;
    }

    int8_t FileReader::getc()const
    {
        _char = fgetc( _file );
        return _char;
    }

    FileReader::operator bool()const
    {
        return _file != nullptr;
    }

    FileReader::FileReader( const kege::string& filename, const char* flags )
    :   _previous_position( 0 )
    ,   _current_position( 0 )
    ,   _file( nullptr )
    {
        _file = fopen( filename.c_str(), flags );
        if ( _file ) getc();
    }

    FileReader::FileReader( FILE* file )
    :   _previous_position( 0 )
    ,   _current_position( 0 )
//    ,   _bufrsize( 256 )
//    ,   _buffer( nullptr )
    ,   _file( file )
    ,   _char(0)
    {
        getc();
//        _buffer = new char[ _bufrsize ];
    }
    
    FileReader::~FileReader()
    {
//        if ( _buffer )
//        {
//            delete [] _buffer;
//            _buffer = nullptr;
//        }
    }

    FileReader::FileReader()
    :   _previous_position( 0 )
    ,   _current_position( 0 )
//    ,   _bufrsize( 256 )
//    ,   _buffer( nullptr )
    ,   _file( nullptr )
    ,   _char(0)
    {
    }

}
