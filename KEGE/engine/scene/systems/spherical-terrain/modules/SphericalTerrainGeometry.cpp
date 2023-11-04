//
//  SphericalTerrainTile.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 8/26/23.
//

#include "../../../../app/Core.hpp"
#include "../../../../app/KEGE.hpp"
#include "../../../../app/Graphics.hpp"
#include "../../spherical-terrain/modules/SphericalQuadtreeTerrain.hpp"
#include "../../spherical-terrain/modules/SphericalTerrainGeometry.hpp"

namespace kege
{
    void SphericalTerrainGeometry::draw( core::CommandBuffer* command, uint32_t first_instance, uint32_t instance_count )const
    {
        int i;
        for( i=0; i<_instance_buffers.size(); i++ )
        {
            //command->bindUniformLayout( 1, _instance_buffers[i].ref() );
            Geometry::draw( command, 0, _instance_buffers[i]->_drawcount );
        }
    }

    uint32_t getSizeCap( uint32_t count )
    {
        uint32_t max_count = 1;
        if ( count == 1 )
        {
            max_count = 1;
        }
        else if ( count <= 4 )
        {
            max_count = 4;
        }
        else if ( count <= 16 )
        {
            max_count = 16;
        }
        else if ( count <= 64 )
        {
            max_count = 64;
        }
        else if ( count <= 256 )
        {
            max_count = 256;
        }
        else if ( count <= 1024 )
        {
            max_count = 1024;
        }
        else if ( count <= 4096 )
        {
            max_count = 4096;
        }
        else if ( count <= 16384 )
        {
            max_count = 16384;
        }
        else if ( count <= 65536 )
        {
            max_count = 65536;
        }
        else // if ( count <= 262144 )
        {
            max_count = MAX_SPHERICAL_TERRAIN_INSTANCES;
        }
        return max_count;
    }

    const std::vector< Ref< kege::DrawInstanceBuffer > >& SphericalTerrainGeometry::getDrawInstanceBuffers()const
    {
        return _instance_buffers;
    }
    
    void SphericalTerrainGeometry::pushInstanceBuffer( uint32_t count, const kege::mat24* instance )
    {
        kege::Graphics* graphics = KEGE::core()->graphics();
        uint32_t max_count = getSizeCap( count );

        /*
         * if no buffer exist create one
         */
        if ( _instance_buffers.empty() || _buffer_count >= _instance_buffers.size() )
        {
            kege::Ref< core::Buffer > bufr = graphics->createBuffer
            (
                core::Buffer::STORAGE,
                max_count * sizeof( kege::mat24 ),
                instance, core::Buffer::DYNAMIC
            );

            DrawInstanceBuffer* dib = new DrawInstanceBuffer();
            dib->setBinding( 0, new kege::BufferBinding( "InstanceBuffer", 0, bufr ) );
            dib->_capacity = max_count;
            dib->_drawcount = count;

            _instance_buffers.push_back( dib );
        }
        else
        {
            /*
             * if the number of instance is greater than the current buffer capacity create a new buffer
             */
            if( count > _instance_buffers[ _buffer_count ]->_capacity )
            {
                kege::Ref< core::Buffer > bufr = graphics->createBuffer
                (
                    core::Buffer::STORAGE,
                    max_count * sizeof( kege::mat24 ),
                    instance, core::Buffer::DYNAMIC
                );

                _instance_buffers[ _buffer_count ]->setBinding( 0, new kege::BufferBinding( "InstanceBuffer", 0, bufr ) );
                _instance_buffers[ _buffer_count ]->_capacity = max_count;
                _instance_buffers[ _buffer_count ]->_drawcount = count;
            }
            /*
             * if the number of instance can fit the current buffer capacity copy instance data to buffer
             */
            else
            {
                _instance_buffers[ _buffer_count ]->getBinding( 0 )->getBuffer( 0 )->copyToSystemMemory( 0, count * _instance_data_stride, instance );
                _instance_buffers[ _buffer_count ]->_drawcount = count;
            }
        }
        _buffer_count++;
    }

    void SphericalTerrainGeometry::endInstanceBufferArray()
    {
        // if buffers are not being used return to the system
        if ( _buffer_count < _instance_buffers.size() )
        {
            _instance_buffers.resize( _buffer_count );
        }
        //_buffer_count = 0;
    }

    void SphericalTerrainGeometry::beginInstanceBufferArray()
    {
        _buffer_count = 0;
    }

    SphericalTerrainSubTile& SphericalTerrainGeometry::subtile()
    {
        return _subtile;
    }

    const kege::vec3& SphericalTerrainGeometry::xaxis()const
    {
        return _xaxis;
    }

    const kege::vec3& SphericalTerrainGeometry::yaxis()const
    {
        return _yaxis;
    }

    const kege::vec3& SphericalTerrainGeometry::zaxis()const
    {
        return _zaxis;
    }

    double SphericalTerrainGeometry::radius()const
    {
        return _radius;
    }

    SphericalTerrainGeometry::FaceId SphericalTerrainGeometry::faceId()const
    {
        return _face_id;
    }

    SphericalTerrainGeometry::SphericalTerrainGeometry( FaceId face_id, double radius, const kege::mat43& quad_points )
    :   _instance_data_stride( sizeof( kege::mat24 ) )
    ,   _buffer_count( 0 )
    ,   _radius( radius )
    ,   _face_id( face_id )
    {
        double sphere_radius = radius + radius * 0.2928932188; // = sin(45.0)
        kege::vec3 quad_center = (quad_points[0] + quad_points[1] + quad_points[2] + quad_points[3]) * radius * 0.25;
        _subtile.operator()( 0, 0, radius, sphere_radius, quad_center, this );

        _yaxis = kege::normalize( quad_center );
        _xaxis = kege::normalize( quad_points[0] - quad_points[1] );
        _zaxis = kege::normalize( quad_points[1] - quad_points[2] );

        kege::mat43 points =
        {
            quad_points[0] - _yaxis,
            quad_points[1] - _yaxis,
            quad_points[2] - _yaxis,
            quad_points[3] - _yaxis
        };

        const int32_t mesh_width = 8;
        const int32_t w1 = mesh_width + 1;
        std::vector< kege::vec3 > vdata;
        vdata.resize( kege::sq( w1 ) );

        kege::vec3 p[3];
        uint32_t count = 0;
        float step_length = 1.0f / float( mesh_width );
        for(float y = 0.0f; y <= 1.0f; y += step_length)
        {
            for(float x = 0.0f; x <= 1.0f; x += step_length)
            {
                p[0] = kege::lerp( points[1], points[0], x );
                p[1] = kege::lerp( points[2], points[3], x );
                p[2] = kege::lerp( p[0], p[1], y );
                vdata[ count++ ] = p[2];
            }
        }

        count = 0;
        struct QuadIndices{ uint32_t i[24]; };
        std::vector< QuadIndices > idata;
        idata.resize( kege::sq( mesh_width / 2 ) );
        for(int y = 0.0f; y < mesh_width; y+=2)
        {
            for(int x = 0.0f; x < mesh_width; x+=2)
            {
                idata[ count ].i[0] = x + y * w1;
                idata[ count ].i[1] = idata[ count ].i[0] + w1;
                idata[ count ].i[2] = idata[ count ].i[1] + 1;
                idata[ count ].i[3] = idata[ count ].i[2];
                idata[ count ].i[4] = idata[ count ].i[0] + 1;
                idata[ count ].i[5] = idata[ count ].i[0];

                //std::cout <<"{"<< idata[ count ].i[0] <<","<< idata[ count ].i[1] <<","<< idata[ count ].i[2]<<"}\n";
                //std::cout <<"{"<< idata[ count ].i[3] <<","<< idata[ count ].i[4] <<","<< idata[ count ].i[5]<<"}\n\n";

                idata[ count ].i[ 6] = idata[ count ].i[4] + 1;
                idata[ count ].i[ 7] = idata[ count ].i[4];
                idata[ count ].i[ 8] = idata[ count ].i[3];
                idata[ count ].i[ 9] = idata[ count ].i[2];
                idata[ count ].i[10] = idata[ count ].i[2] + 1;
                idata[ count ].i[11] = idata[ count ].i[6];

                //std::cout <<"{"<< idata[ count ].i[6] <<","<< idata[ count ].i[7] <<","<< idata[ count ].i[8]<<"}\n";
                //std::cout <<"{"<< idata[ count ].i[9] <<","<< idata[ count ].i[10] <<","<< idata[ count ].i[11]<<"}\n\n";

                idata[ count ].i[12] = idata[ count ].i[ 2];
                idata[ count ].i[13] = idata[ count ].i[ 1];
                idata[ count ].i[14] = idata[ count ].i[ 1] + w1;
                idata[ count ].i[15] = idata[ count ].i[14];
                idata[ count ].i[16] = idata[ count ].i[14] + 1;
                idata[ count ].i[17] = idata[ count ].i[12];

                //std::cout <<"{"<< idata[ count ].i[12] <<","<< idata[ count ].i[13] <<","<< idata[ count ].i[14]<<"}\n";
                //std::cout <<"{"<< idata[ count ].i[15] <<","<< idata[ count ].i[16] <<","<< idata[ count ].i[17]<<"}\n\n";

                idata[ count ].i[18] = idata[ count ].i[17];
                idata[ count ].i[19] = idata[ count ].i[16];
                idata[ count ].i[20] = idata[ count ].i[16] + 1;
                idata[ count ].i[21] = idata[ count ].i[20];
                idata[ count ].i[22] = idata[ count ].i[10];
                idata[ count ].i[23] = idata[ count ].i[18];

                //std::cout <<"{"<< idata[ count ].i[18] <<","<< idata[ count ].i[19] <<","<< idata[ count ].i[20]<<"}\n";
                //std::cout <<"{"<< idata[ count ].i[21] <<","<< idata[ count ].i[22] <<","<< idata[ count ].i[23]<<"}\n\n";
                
                count++;
            }
        }

        _first_index = 0;
        _draw_count = mesh_width * mesh_width * 6;

        kege::Ref< core::Buffer > vbo = KEGE::core()->graphics()->createBuffer( core::Buffer::VERTEX, vdata.size() * sizeof( kege::vec3 ), vdata.data(), core::Buffer::STATIC );
        kege::Ref< core::Buffer > ibo = KEGE::core()->graphics()->createBuffer( core::Buffer::INDEX, idata.size() * sizeof( QuadIndices ), idata.data(), core::Buffer::STATIC );

        _vertexarray = new core::VertexArrayIndexed
        (
            {{ 0, Vertex::POSITION, Vertex::F16_3, 0 }},
            {{ vbo, int32_t(sizeof(kege::vec3)), core::VertexInputRate::PER_VERTEX }},
            new core::IndexBuffer( core::Index::UI32, 0, _draw_count, ibo )
        );
    }

    SphericalTerrainGeometry::~SphericalTerrainGeometry()
    {
        _instance_buffers.clear();
        _vertexarray.clear();
        _buffer_count = 0;
    }

}
