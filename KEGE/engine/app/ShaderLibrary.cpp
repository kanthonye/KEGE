//
//  ShaderLibrary.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/17/23.
//

#include "ShaderLibrary.hpp"
#include "../app/KEGE.hpp"
#include "../app/Core.hpp"
#include "../app/Graphics.hpp"
namespace kege
{
//    kege::core::Primitive::Type parsePrimitiveType( const kege::string& str )
//    {
//        if ( str == "triangle-list" )
//        {
//            return kege::core::Primitive::TRIANGLE_LIST;
//        }
//        else if ( str == "triangle-strip" )
//        {
//            return kege::core::Primitive::TRIANGLE_STRIP;
//        }
//        else if ( str == "triangle-fan" )
//        {
//            return kege::core::Primitive::TRIANGLE_FAN;
//        }
//        else if ( str == "line-list" )
//        {
//            return kege::core::Primitive::LINE_LIST;
//        }
//        else if ( str == "line-strip" )
//        {
//            return kege::core::Primitive::LINE_STRIP;
//        }
//        else if ( str == "point-list" )
//        {
//            return kege::core::Primitive::POINT_LIST;
//        }
//        else if ( str == "line-list-with-adjacency" )
//        {
//            return kege::core::Primitive::LINE_LIST_WITH_ADJACENCY;
//        }
//        else if ( str == "line-strip-with-adjacency" )
//        {
//            return kege::core::Primitive::LINE_STRIP_WITH_ADJACENCY;
//        }
//        else if ( str == "triangle-list-with-adjacency" )
//        {
//            return kege::core::Primitive::TRIANGLE_LIST_WITH_ADJACENCY;
//        }
//        else if ( str == "triangle-strip-with-adjacency" )
//        {
//            return kege::core::Primitive::TRIANGLE_STRIP_WITH_ADJACENCY;
//        }
//
//
//        throw std::runtime_error
//        ( ("UN-SUPPORTED Primitive::Type -> " + str
//           + ".\n Current supported Primitive::Type are: \n - triangle-list, \n - triangle-strip, \n - triangle-fan, \n - line-list, \n - line-strip, \n - line-list-with-adjacency, \n - line-strip-with-adjacency, \n - triangle-list-with-adjacenc, \n - triangle-strip-with-adjacency, \n - point-list"
//        ).c_str() );
//    }

//    kege::core::Polygon::Mode parsePolygonMode( const kege::string& str )
//    {
//        if ( str == "fill" )
//        {
//            return kege::core::Polygon::FILL;
//        }
//        else if ( str == "line" )
//        {
//            return kege::core::Polygon::LINE;
//        }
//        else if ( str == "point" )
//        {
//            return kege::core::Polygon::POINT;
//        }
//        throw std::runtime_error( ("UN-SUPPORTED polygon mode : parsePolygonMode() -> " + str).c_str() );
//    }

//    void parseShaderStages( std::vector< std::pair< core::Shader::StageType, kege::string > >& stages, FileReader& reader )
//    {
//        reader.skip( ": \t\n" );
//        if ( reader.ischar( "[" ) )
//        {
//            reader.skip( "[ \t\n" );
//            while ( reader.ischar( "{" ) )
//            {
//                reader.skip( "{ \t\n" );
//                reader.read( " :\t\n" );
//                kege::string type = reader.getstr();
//
//                reader.skip( " :\t\n" );
//                reader.read( " }\t\n" );
//                kege::string file = reader.getstr();
//
//                reader.skip( " }\t\n" );
//
//                if ( type == "vertex" )
//                {
//                    stages.push_back({ core::Shader::VERTEX, KEGE::core()->virdir()->fetch( file ) });
//                }
//                else if ( type == "fragment" )
//                {
//                    stages.push_back({ core::Shader::FRAGMENT, KEGE::core()->virdir()->fetch( file ) });
//                }
//                else if ( type == "tess-control" )
//                {
//                    stages.push_back({ core::Shader::TESS_CONTROL, KEGE::core()->virdir()->fetch( file ) });
//                }
//                else if ( type == "tess-evaluation" )
//                {
//                    stages.push_back({ core::Shader::TESS_EVALUATION, KEGE::core()->virdir()->fetch( file ) });
//                }
//                else if ( type == "geometry" )
//                {
//                    stages.push_back({ core::Shader::GEOMETRY, KEGE::core()->virdir()->fetch( file ) });
//                }
//                else if ( type == "compute" )
//                {
//                    stages.push_back({ core::Shader::COMPUTE, KEGE::core()->virdir()->fetch( file ) });
//                }
//            }
//            reader.skip( " ]\t\n" );
//        }
//    }

//    void parseVertexInputRates( core::Shader::VertexInputRates& vertex_input_rates, FileReader& reader )
//    {
//        reader.skip( ": \t\n" );
//        if ( reader.ischar( "[" ) )
//        {
//            while ( !reader.ischar( "]" ) )
//            {
//                reader.skip( "[ \t\n" );
//                core::Shader::InputRate input_rate;
//                while ( !reader.ischar( "}" ) )
//                {
//                    reader.skip( "{ \t\n" );
//                    reader.read( " :\t\n" );
//                    kege::string param = reader.getstr();
//
//                    reader.skip( " \t\n" );
//                    reader.read( " \t\n" );
//                    kege::string value = reader.getstr();
//
//                    if ( param == "input-rate" )
//                    {
//                        if ( value == "PER_VERTEX" )
//                        {
//                            input_rate.rate = core::VertexInputRate::PER_VERTEX;
//                        }
//                        else if ( value == "PER_INSTANCE" )
//                        {
//                            input_rate.rate = core::VertexInputRate::PER_INSTANCE;
//                        }
//                    }
//                    else if ( param == "input-stride" )
//                    {
//                        input_rate.stride = value.to_int32();
//                    }
//                    else if ( param == "input-binding" )
//                    {
//                        input_rate.binding = value.to_int32();
//                    }
//                    reader.skip( " \t\n" );
//                }
//                vertex_input_rates.push_back( input_rate );
//                reader.skip( " }\t\n" );
//            }
//            reader.skip( " ]\t\n" );
//        }
//    }

    bool loadShaderInfo( core::Shader::Info& info, FileReader& reader )
    {
        enum
        {
            GRAPHICS_PIPELINE, COMPUTE_PIPELINE, PIPELINE_NAME, PIPELINE_SHADERS, RASTERIZATION_STATE,
            POLYGON_MODE, POLYGON_TYPE, CULL_MODE,
            FILL, LINE, POINT,
            VERTEX_SHADER, FRAGMENT_SHADER, TESS_EVALUATION_SHADER, TESS_CONTROL_SHADER, GEOMETRY_SHADER, COMPUTE_SHADER,
            BREAK,
        };

        Hashmap< uint32_t > gotos;
        gotos.insert( "rasterization-state",RASTERIZATION_STATE );
        gotos.insert( "graphics-shader",    GRAPHICS_PIPELINE );
        gotos.insert( "compute-shader",     COMPUTE_PIPELINE );
        gotos.insert( "name",               PIPELINE_NAME );
        gotos.insert( "shader-stages",      PIPELINE_SHADERS );
        gotos.insert( "{",      BREAK );
        gotos.insert( "}",      BREAK );
        gotos.insert( "[",      BREAK );
        gotos.insert( "]",      BREAK );
        gotos.insert( ":",      BREAK );

        Hashmap< uint32_t > shaders_types;
        shaders_types.insert( "vertex",             VERTEX_SHADER );
        shaders_types.insert( "fragment",           FRAGMENT_SHADER );
        shaders_types.insert( "tess-evaluation",    TESS_EVALUATION_SHADER );
        shaders_types.insert( "tess-control",       TESS_CONTROL_SHADER );
        shaders_types.insert( "geometry",           GEOMETRY_SHADER );
        shaders_types.insert( "compute",            COMPUTE_SHADER );

        Hashmap< uint32_t > rasterization;
        rasterization.insert( "polygon-type",      POLYGON_TYPE );
        rasterization.insert( "polygon-mode",      POLYGON_MODE );
        rasterization.insert( "cull-mode",         CULL_MODE );

        Hashmap< kege::core::Polygon::Mode > polygon_modes;
        polygon_modes.insert( "fill",         kege::core::Polygon::FILL );
        polygon_modes.insert( "line",         kege::core::Polygon::LINE );
        polygon_modes.insert( "point",        kege::core::Polygon::POINT );

        Hashmap< kege::core::CullMode::Type > cull_modes;
        cull_modes.insert( "cw",     kege::core::CullMode::CW );
        cull_modes.insert( "ccw",    kege::core::CullMode::CCW );
        cull_modes.insert( "cw_ccw", kege::core::CullMode::CW_CCW );

        Hashmap< kege::core::Primitive::Type > polygon_types;
        polygon_types.insert( "point-list",                     kege::core::Primitive::POINT_LIST );
        polygon_types.insert( "point-list",                     kege::core::Primitive::POINT_LIST );
        polygon_types.insert( "triangle-list",                  kege::core::Primitive::TRIANGLE_LIST );
        polygon_types.insert( "triangle-strip",                 kege::core::Primitive::TRIANGLE_STRIP );
        polygon_types.insert( "triangle-fan",                   kege::core::Primitive::TRIANGLE_FAN );
        polygon_types.insert( "triangle-list-with-adjacency",   kege::core::Primitive::TRIANGLE_LIST_WITH_ADJACENCY );
        polygon_types.insert( "triangle-strip-with-adjacency",  kege::core::Primitive::TRIANGLE_STRIP_WITH_ADJACENCY );
        polygon_types.insert( "line-list",                      kege::core::Primitive::LINE_LIST );
        polygon_types.insert( "line-strip",                     kege::core::Primitive::LINE_STRIP );
        polygon_types.insert( "line-list-with-adjacency",       kege::core::Primitive::LINE_LIST_WITH_ADJACENCY );
        polygon_types.insert( "line-strip-with-adjacency",      kege::core::Primitive::LINE_STRIP_WITH_ADJACENCY );


        try
        {
            bool finish = false;
            while ( !reader.eof() && !finish )
            {
                reader.skip( " \t\n" );
                kege::string str = reader.gets( " :\t\n" );
                uint32_t go_to = gotos.get( str );

                switch ( go_to )
                {
                    case GRAPHICS_PIPELINE: 
                        info._pipeline_type = str;
                        reader.skip( " :{\t\n" );
                        break;

                    case COMPUTE_PIPELINE: 
                        info._pipeline_type = str;
                        reader.skip( " :{\t\n" );
                        break;

                    case PIPELINE_NAME:
                        reader.skip( ": \t\n" );
                        info._name = reader.gets( " \t\n" );
                        break;

                    case PIPELINE_SHADERS:
                    {
                        reader.skip( ":[ \t\n" );
                        while ( !reader.ischar( "]" ) )
                        {
                            reader.skip( "{ \t\n" );

                            kege::string name = reader.gets( ": " );
                            reader.skip( ": \t\n" );

                            kege::string value = reader.gets( " }\n\t" );
                            reader.skip( "\t \n" );

                            go_to = shaders_types.get( name );
                            switch ( go_to )
                            {
                                case VERTEX_SHADER:
                                    info._shader_stages.push_back({ core::Shader::VERTEX, KEGE::core()->virdir()->fetch( value ) });
                                    break;

                                case FRAGMENT_SHADER:
                                    info._shader_stages.push_back({ core::Shader::FRAGMENT, KEGE::core()->virdir()->fetch( value ) });
                                    break;

                                case TESS_CONTROL_SHADER:
                                    info._shader_stages.push_back({ core::Shader::TESS_CONTROL, KEGE::core()->virdir()->fetch( value ) });
                                    break;

                                case TESS_EVALUATION_SHADER:
                                    info._shader_stages.push_back({ core::Shader::TESS_EVALUATION, KEGE::core()->virdir()->fetch( value ) });
                                    break;

                                case GEOMETRY_SHADER:
                                    info._shader_stages.push_back({ core::Shader::GEOMETRY, KEGE::core()->virdir()->fetch( value ) });
                                    break;

                                case COMPUTE_SHADER:
                                    info._shader_stages.push_back({ core::Shader::COMPUTE, KEGE::core()->virdir()->fetch( value ) });
                                    break;

                                default: std::runtime_error( ("ERROR: " + name + " <- is an unsupported shader-stage type").c_str() );
                                    break;
                            }
                            reader.skip( "} \t\n" );
                        }
                        reader.skip( "}] \t\n" );
                        break;
                    }

                    case RASTERIZATION_STATE:{
                        reader.seek( "{" );
                        while ( !reader.ischar( "}" ) )
                        {
                            reader.skip( "{ \t\n" );

                            kege::string name = reader.gets( ": " );
                            reader.skip( ": \t\n" );

                            kege::string value = reader.gets( " \n\t" );
                            reader.skip( "\t \n" );

                            go_to = rasterization.get( name );
                            switch ( go_to )
                            {
                                case POLYGON_MODE:
                                    info._polygon_mode = polygon_modes.get( value );
                                    break;

                                case POLYGON_TYPE:
                                    info._primitive_topology = polygon_types.get( value );
                                    break;

                                case CULL_MODE:
                                    info._cull_mode = cull_modes.get( value );
                                    break;

                                default: std::runtime_error( ("ERROR: " + name + " <- is an unsupported cull-mode type").c_str() );
                                    break;
                            }
                        }
                        reader.skip( "} \t\n" );
                        break;
                    }

                    case BREAK: finish = true;
                        break;

                    default: std::runtime_error( ("ERROR: " + str + " <- is an unsupported parameter.").c_str() );
                        break;
                }
            }
        }
        catch ( const std::runtime_error& err )
        {
            throw err;
        }

        return true;
    }

    kege::Ref< core::Shader > ShaderLibrary::load( const kege::string& filename )
    {
        FileReader reader = KEGE::core()->virdir()->open( filename );
        if ( reader.eof() )
        {
            KEGE_ERROR( "shader file %s not fount.", filename.c_str() );
            return nullptr;
        }

        core::Shader::Info info;
        if ( loadShaderInfo( info, reader ) )
        {
            kege::Graphics* graphics = KEGE::core()->graphics();
            core::Renderer* renderer = graphics->renderer();
            return graphics->device()->createGraphicsShader( info, renderer->getSwapchainRenderpass() );
        }
        return nullptr;
    }
    
    kege::Ref< core::Shader > ShaderLibrary::get( const kege::string& name )
    {
        FileReader reader = KEGE::core()->virdir()->open( name );
        if ( reader.eof() )
        {
            KEGE_ERROR( "shader file %s not fount.", name.c_str() );
            return nullptr;
        }

        kege::Ref< core::Shader > shader;

        core::Shader::Info info;
        if ( loadShaderInfo( info, reader ) )
        {
            kege::Graphics* graphics = KEGE::core()->graphics();
            if ( info._pipeline_type == "graphics-shader" )
            {
                const core::Renderpass* renderpass = graphics->renderer()->getSwapchainRenderpass();
                shader = graphics->device()->createGraphicsShader( info, renderpass );
                if ( shader == nullptr )
                {
                    return nullptr;
                }
            }
            else if ( info._pipeline_type == "compute-shader" )
            {
                shader = graphics->device()->createComputeShader( info );
                if ( shader == nullptr )
                {
                    return nullptr;
                }
            }
            _shaders.add( name, shader );
        }

        return shader;
    }

    void ShaderLibrary::uninit()
    {
        _filepaths.clear();
        _shaders.purge();
    }
    bool ShaderLibrary::init()
    {
        FileReader reader = KEGE::core()->virdir()->open( "assets/config/msl.txt" );
        if ( reader.eof() )
        {
            KEGE_ERROR( "master-shader-library file assets/config/msl.txt not found." );
            return false;
        }

        while ( !reader.eof() )
        {
            reader.skip( " \t\n" );
            kege::string name = reader.gets( " \t\n" );

            reader.skip( " \t\n" );
            kege::string value = reader.gets( " \t\n" );

            reader.skip( " \t\n" );
            _filepaths.insert( name, value );

            std::cout <<"MSL: shader -> " << name << ", " << value <<'\n';
        }
        return true;
    }

    ShaderLibrary::~ShaderLibrary()
    {
        uninit();
    }
}
