//
//  Config.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 6/12/23.
//
#include "Config.hpp"
namespace kege
{
    const ConfigElement& ConfigElement::operator []( const kege::string& name )const
    {
        return *this;
    }
    ConfigElement& ConfigElement::operator []( const kege::string& name )
    {
        return *this;
    }
    bool ConfigElement::operator ==( const kege::string& name )const
    {
        return this->name() == name;
    }
    ConfigElement& ConfigElement::operator =( const kege::string& value )
    {
        return *this;
    }
    ConfigElement::operator kege::string()const
    {
        return "";
    }
    ConfigElement& ConfigElement::setU64( uint64_t value )
    {
        return *this;
    }
    ConfigElement& ConfigElement::setU32( uint32_t value )
    {
        return *this;
    }
    ConfigElement& ConfigElement::setU16( uint16_t value )
    {
        return *this;
    }
    ConfigElement& ConfigElement::setU8( uint8_t value )
    {
        return *this;
    }
    ConfigElement& ConfigElement::setI64( int64_t value )
    {
        return *this;
    }
    ConfigElement& ConfigElement::setI32( int32_t value )
    {
        return *this;
    }
    ConfigElement& ConfigElement::setI16( int16_t value )
    {
        return *this;
    }
    ConfigElement& ConfigElement::setI8( int8_t value )
    {
        return *this;
    }
    ConfigElement& ConfigElement::setF32( double value )
    {
        return *this;
    }
    ConfigElement& ConfigElement::setF16( float value )
    {
        return *this;
    }
    ConfigElement& ConfigElement::setBool( bool value )
    {
        return *this;
    }

    const char* ConfigElement::getStr()const
    {
        return nullptr;
    }
    uint64_t ConfigElement::getU64()const
    {
        return 0ULL;
    }
    uint32_t ConfigElement::getU32()const
    {
        return 0;
    }
    uint16_t ConfigElement::getU16()const
    {
        return 0;
    }
    uint8_t ConfigElement::getU8()const
    {
        return 0;
    }
    int64_t ConfigElement::getI64()const
    {
        return 0LL;
    }
    int32_t ConfigElement::getI32()const
    {
        return 0;
    }
    int16_t ConfigElement::getI16()const
    {
        return 0;
    }
    int8_t ConfigElement::getI8()const
    {
        return 0;
    }
    double ConfigElement::getF32()const
    {
        return 0.0;
    }
    float ConfigElement::getF16()const
    {
        return 0.0f;
    }
    bool ConfigElement::getBool()const
    {
        return false;
    }

    const kege::string& ConfigElement::name()const
    {
        return _name;
    }

    ConfigElement::ConfigElement( const kege::string& name )
    :   _name( name )
    {}
}



namespace kege
{
    ConfigSetting::ConfigSetting( const kege::string& name, const kege::string& value )
    :   ConfigElement( name )
    ,   _value( value )
    {}

    ConfigElement& ConfigSetting::operator =( const kege::string& value )
    {
        _value = value;
        return *this;
    }
    ConfigSetting::operator kege::string()const
    {
        return _value;
    }

    ConfigElement& ConfigSetting::setU64( uint64_t value )
    {
        char str[32] = {0,0,0,0,0,0,0,0};
        long w = snprintf(str, 32, "%llu", value);
        str[w] = 0;
        _value = str;
        return *this;
    }
    ConfigElement& ConfigSetting::setU32( uint32_t value )
    {
        char str[16] = {0,0,0,0,0,0,0,0};
        long w = snprintf(str, 16, "%iu", value);
        str[w] = 0;
        _value = str;
        return *this;
    }
    ConfigElement& ConfigSetting::setU16( uint16_t value )
    {
        char str[8] = {0,0,0,0,0,0,0,0};
        long w = snprintf(str, 8, "%ui", value);
        str[w] = 0;
        _value = str;
        return *this;
    }
    ConfigElement& ConfigSetting::setU8( uint8_t value )
    {
        char str[4] = {0,0,0,0};
        long w = snprintf(str, 4, "%iu", value);
        str[w] = 0;
        _value = str;
        return *this;
    }
    ConfigElement& ConfigSetting::setI64( int64_t value )
    {
        char str[32] = {0,0,0,0,0,0,0,0};
        long w = snprintf(str, 32, "%lld", value);
        str[w] = 0;
        _value = str;
        return *this;
    }
    ConfigElement& ConfigSetting::setI32( int32_t value )
    {
        char str[32] = {0,0,0,0,0,0,0,0};
        long w = snprintf(str, 16, "%i", value);
        str[w] = 0;
        _value = str;
        return *this;
    }
    ConfigElement& ConfigSetting::setI16( int16_t value )
    {
        char str[16] = {0,0,0,0,0,0,0,0};
        long w = snprintf(str, 8, "%iu", value);
        str[w] = 0;
        _value = str;
        return *this;
    }
    ConfigElement& ConfigSetting::setI8( int8_t value )
    {
        char str[4] = {0,0,0,0};
        long w = snprintf(str, 4, "%i", value);
        str[w] = 0;
        _value = str;
        return *this;
    }
    ConfigElement& ConfigSetting::setF32( double value )
    {
        char str[32] = {0,0,0,0,0,0,0,0};
        long w = snprintf(str, 32, "%f", value);
        str[w] = 0;
        _value = str;
        return *this;
    }
    ConfigElement& ConfigSetting::setF16( float value )
    {
        char str[32] = {0,0,0,0,0,0,0,0};
        long w = snprintf(str, 32, "%f", value);
        str[w] = 0;
        _value = str;
        return *this;
    }
    ConfigElement& ConfigSetting::setBool( bool value )
    {
        char str[2] = {0,0};
        long w = snprintf(str, 1, "%i", value);
        str[w] = 0;
        _value = str;
        return *this;
    }

    const char* ConfigSetting::getStr()const
    {
        return _value.c_str();
    }
    uint64_t ConfigSetting::getU64()const
    {
        return atoll( _value.c_str() );
    }
    uint32_t ConfigSetting::getU32()const
    {
        return atoi( _value.c_str() );
    }
    uint16_t ConfigSetting::getU16()const
    {
        return atoi( _value.c_str() );
    }
    uint8_t ConfigSetting::getU8()const
    {
        return atoi( _value.c_str() );
    }
    int64_t ConfigSetting::getI64()const
    {
        return atoll( _value.c_str() );
    }
    int32_t ConfigSetting::getI32()const
    {
        return atoi( _value.c_str() );
    }
    int16_t ConfigSetting::getI16()const
    {
        return atoi( _value.c_str() );
    }
    int8_t ConfigSetting::getI8()const
    {
        return atoi( _value.c_str() );
    }
    double ConfigSetting::getF32()const
    {
        return atof( _value.c_str() );
    }
    float ConfigSetting::getF16()const
    {
        return atof( _value.c_str() );
    }
    bool ConfigSetting::getBool()const
    {
        if ( _value == "false" )
        {
            return false;
        }
        else if ( _value == "true" )
        {
            return true;
        }
        return atoi( _value.c_str() );
    }
}



namespace kege
{
    void Config::insert( const kege::string& name, kege::Ref< ConfigElement > config )
    {
        uint64_t key = name.hash();
        auto m = _configs.find( key );
        Config::Elements* elements = nullptr;
        if ( m == _configs.end() )
        {
            elements = new Config::Elements();
            _configs[ key ] = elements;
        }
        else
        {
            elements = m->second;
        }

        for (auto& a : *elements )
        {
            if ( a->operator==( name ) )
            {
                a = config;
                return;
            }
        }

        elements->push_back( config );
    }

    void Config::insert( const kege::string& sid, const kege::string& value )
    {
        insert( sid, new ConfigSetting( sid, value ) );
    }
    void Config::parse( const std::vector< kege::string >& params )
    {

    }
    const ConfigElement& Config::operator []( const kege::string& name )const
    {
        auto m = _configs.find( name.hash() );
        if ( m != _configs.end() )
        {
            for (auto& a : *m->second )
            {
                if ( a->operator==( name ) )
                {
                    return *a.ref();
                }
            }
        }

        KEGE_FATAL("ConfigElement& ConfigNode::operator []( %s ): does not exist.", name.c_str())
        throw std::runtime_error("ConfigElement::settings");
        return *this;
    }
    
    ConfigElement& Config::operator []( const kege::string& name )
    {
        auto m = _configs.find( name.hash() );
        if ( m != _configs.end() )
        {
            for (auto& a : *m->second )
            {
                if ( a->operator==( name ) )
                {
                    return *a.ref();
                }
            }
        }

        KEGE_FATAL("ConfigElement& ConfigNode::operator []( %s ): does not exist.", name.c_str())
        throw std::runtime_error("ConfigElement::settings");
        return *this;
    }

    void Config::parseBody( Config& config, kege::FileReader& reader)
    {
        if ( reader.ischar( "{" ) )
        {
            reader.skip( " {\t\n" );
            while ( !reader.ischar( "}" ) && !reader.eof()  )
            {
                reader.skip( " \t\n" );
                kege::string name = reader.gets( ":\n" );
                reader.skip( " :\t\n" );

                kege::string value = reader.gets( "\n" );
                std::cout << name <<":  " << value <<"\n";
                reader.skip( " \n/t" );

                config.insert( name, value );
            }
            if ( !reader.ischar( "}" ) )
            {
                throw std::runtime_error( (config._name + " is missing '}'\n").c_str() );
            }
            reader.skip( " }\t" );
        }
        else if ( !reader.eof() )
        {
            throw std::runtime_error( (config._name + " is missing '{'\n").c_str() );
        }
    }

    bool Config::load( const kege::string &filename )
    {
        kege::FileReader reader;
        if( reader.open( filename.c_str() ) )
        {
            while ( !reader.eof() )
            {
                reader.skip(" \t\n");
                kege::string config_object = reader.gets( " :\t\n" );

                if( reader.ischar( ":" ) )
                {
                    reader.skip( " :\t\n" );

                    Config* config = new Config( config_object );
                    try
                    {
                        parseBody( *config, reader );
                        insert( config->_name, config );
                    }
                    catch ( const std::runtime_error& err )
                    {
                        delete config;
                        throw err;
                    }
                }
                else if ( !reader.eof() )
                {
                    throw std::runtime_error( "[ error ] missing ':'\n" );
                }
            }
            return true;
        };
        return false;
    }
    
    void Config::erase(const kege::string &sid)
    {
        auto m = _configs.find( sid.hash() );
        if ( m != _configs.end() )
        {
            _configs.erase( m );
        }
    }

    const Config::Map& Config::elements()const
    {
        return _configs;
    }
    Config::Map& Config::elements()
    {
        return _configs;
    }

    Config::Config( const kege::string &name )
    :   ConfigElement( name )
    {}

    Config::Config()
    {}

}
