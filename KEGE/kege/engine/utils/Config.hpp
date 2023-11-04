//
//  Config.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 6/12/23.
//

#ifndef Config_hpp
#define Config_hpp
#include <stdlib.h>
#include <vector>
#include <unordered_map>
#include "kege-string.hpp"
#include "kege-logger.hpp"
#include "reader/FileReader.hpp"
#include "Ref.hpp"
namespace kege
{
    struct ConfigElement : public kege::RefCounter
    {
    public:

        virtual const ConfigElement& operator []( const kege::string& name )const;
        virtual ConfigElement& operator []( const kege::string& name );

        bool operator ==( const kege::string& name )const;

        virtual ConfigElement& operator =( const kege::string& value );
        virtual operator kege::string()const;

        virtual ConfigElement& setU64( uint64_t value );
        virtual ConfigElement& setU32( uint32_t value );
        virtual ConfigElement& setU16( uint16_t value );
        virtual ConfigElement& setU8( uint8_t value );
        virtual ConfigElement& setI64( int64_t value );
        virtual ConfigElement& setI32( int32_t value );
        virtual ConfigElement& setI16( int16_t value );
        virtual ConfigElement& setI8( int8_t value );
        virtual ConfigElement& setF32( double value );
        virtual ConfigElement& setF16( float value );
        virtual ConfigElement& setBool( bool value );

        virtual const char* getStr()const;
        virtual uint64_t getU64()const;
        virtual uint32_t getU32()const;
        virtual uint16_t getU16()const;
        virtual uint8_t getU8()const;
        virtual int64_t getI64()const;
        virtual int32_t getI32()const;
        virtual int16_t getI16()const;
        virtual int8_t getI8()const;
        virtual double getF32()const;
        virtual float getF16()const;
        virtual bool getBool()const;

        const kege::string& name()const;
        virtual ~ConfigElement(){}

    protected:

        ConfigElement& operator =( const ConfigElement& ) = default;
        ConfigElement( const ConfigElement& ) = default;
        ConfigElement( const kege::string& name );
        ConfigElement(){}
        kege::string _name;
    };
}



namespace kege
{
    struct ConfigSetting : public ConfigElement
    {
    public:

        ConfigSetting( const kege::string& name, const kege::string& value );
        ConfigElement& operator =( const kege::string& value );
        operator kege::string()const;

        ConfigElement& setU64( uint64_t value );
        ConfigElement& setU32( uint32_t value );
        ConfigElement& setU16( uint16_t value );
        ConfigElement& setU8( uint8_t value );
        ConfigElement& setI64( int64_t value );
        ConfigElement& setI32( int32_t value );
        ConfigElement& setI16( int16_t value );
        ConfigElement& setI8( int8_t value );
        ConfigElement& setF32( double value );
        ConfigElement& setF16( float value );
        ConfigElement& setBool( bool value );

        const char* getStr()const;
        uint64_t getU64()const;
        uint32_t getU32()const;
        uint16_t getU16()const;
        uint8_t getU8()const;
        int64_t getI64()const;
        int32_t getI32()const;
        int16_t getI16()const;
        int8_t getI8()const;
        double getF32()const;
        float getF16()const;
        bool getBool()const;

    private:

        kege::string _value;
    };
}



namespace kege
{
    struct Config : public ConfigElement
    {
    public:

        typedef std::vector< kege::Ref< ConfigElement > > Elements;
        typedef std::unordered_map< uint64_t, Config::Elements* > Map;

        void insert( const kege::string& name, kege::Ref< ConfigElement > config );
        void insert( const kege::string& sid, const kege::string& value );
        void parse( const std::vector< kege::string >& params );

        const ConfigElement& operator []( const kege::string& name )const;
        ConfigElement& operator []( const kege::string& name );

        bool load( const kege::string &filename );
        void erase( const kege::string &sid );

        const Config::Map& elements()const;
        Config::Map& elements();

        Config( const kege::string &name );
        Config();
        
    private:

        void parseBody( Config& config, kege::FileReader& reader );

    private:

        Config::Map _configs;
    };
}
#endif /* Config_hpp */
