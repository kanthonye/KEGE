//
//  Cuboid.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 8/20/23.
//

#include "Cuboid.hpp"
namespace kege
{
    Cuboid::Cuboid( const kege::vec3& center, const kege::vec3& lengths )
    {
        kege::vec3 p[ 8 ];
        p[0] = center + kege::vec3(  lengths.x,  lengths.y,  lengths.z );
        p[1] = center + kege::vec3( -lengths.x,  lengths.y,  lengths.z );
        p[2] = center + kege::vec3( -lengths.x, -lengths.y,  lengths.z );
        p[3] = center + kege::vec3(  lengths.x, -lengths.y,  lengths.z );

        p[4] = center + kege::vec3(  lengths.x,  lengths.y, -lengths.z );
        p[5] = center + kege::vec3( -lengths.x,  lengths.y, -lengths.z );
        p[6] = center + kege::vec3( -lengths.x, -lengths.y, -lengths.z );
        p[7] = center + kege::vec3(  lengths.x, -lengths.y, -lengths.z );

        _attributes = kege::Vertex::attributes();
        _vertex_stride = sizeof( kege::Vertex );
        _index_type = core::Index::UI32;
        _vertex_count = 24;
        _index_count = 36;

        _vertex_data.resize( _vertex_count * sizeof( kege::Vertex ) );
        kege::Vertex* vertices = reinterpret_cast< kege::Vertex* >( _vertex_data.data() );
        // front face
        vertices[0]._position = p[0];
        vertices[1]._position = p[1];
        vertices[2]._position = p[2];
        vertices[3]._position = p[3];

        vertices[0]._normal = vec3::zaxis;
        vertices[1]._normal = vec3::zaxis;
        vertices[2]._normal = vec3::zaxis;
        vertices[3]._normal = vec3::zaxis;

        vertices[0]._texcoord = vec2(1.0f, 1.0f);
        vertices[1]._texcoord = vec2(0.0f, 1.0f);
        vertices[2]._texcoord = vec2(0.0f, 0.0f);
        vertices[3]._texcoord = vec2(0.0f, 1.0f);


        // back face
        vertices[4]._position = p[5];
        vertices[5]._position = p[4];
        vertices[6]._position = p[7];
        vertices[7]._position = p[6];

        vertices[4]._normal = -vec3::zaxis;
        vertices[5]._normal = -vec3::zaxis;
        vertices[6]._normal = -vec3::zaxis;
        vertices[7]._normal = -vec3::zaxis;

        vertices[4]._texcoord = vec2(1.0f, 1.0f);
        vertices[5]._texcoord = vec2(0.0f, 1.0f);
        vertices[6]._texcoord = vec2(0.0f, 0.0f);
        vertices[7]._texcoord = vec2(0.0f, 1.0f);

        // left face
        vertices[ 8]._position = p[1];
        vertices[ 9]._position = p[5];
        vertices[10]._position = p[6];
        vertices[11]._position = p[2];

        vertices[ 8]._normal = -vec3::xaxis;
        vertices[ 9]._normal = -vec3::xaxis;
        vertices[10]._normal = -vec3::xaxis;
        vertices[11]._normal = -vec3::xaxis;

        vertices[ 8]._texcoord = vec2(1.0f, 1.0f);
        vertices[ 9]._texcoord = vec2(0.0f, 1.0f);
        vertices[10]._texcoord = vec2(0.0f, 0.0f);
        vertices[11]._texcoord = vec2(0.0f, 1.0f);

        // right face
        vertices[12]._position = p[4];
        vertices[13]._position = p[0];
        vertices[14]._position = p[3];
        vertices[15]._position = p[7];

        vertices[12]._normal = vec3::xaxis;
        vertices[13]._normal = vec3::xaxis;
        vertices[14]._normal = vec3::xaxis;
        vertices[15]._normal = vec3::xaxis;

        vertices[12]._texcoord = vec2(1.0f, 1.0f);
        vertices[13]._texcoord = vec2(0.0f, 1.0f);
        vertices[14]._texcoord = vec2(0.0f, 0.0f);
        vertices[15]._texcoord = vec2(0.0f, 1.0f);

        // top face
        vertices[16]._position = p[0];
        vertices[17]._position = p[4];
        vertices[18]._position = p[5];
        vertices[19]._position = p[1];

        vertices[16]._normal = vec3::yaxis;
        vertices[17]._normal = vec3::yaxis;
        vertices[18]._normal = vec3::yaxis;
        vertices[19]._normal = vec3::yaxis;

        vertices[16]._texcoord = vec2(1.0f, 1.0f);
        vertices[17]._texcoord = vec2(0.0f, 1.0f);
        vertices[18]._texcoord = vec2(0.0f, 0.0f);
        vertices[19]._texcoord = vec2(0.0f, 1.0f);

        // bottom face
        vertices[20]._position = p[7];
        vertices[21]._position = p[3];
        vertices[22]._position = p[2];
        vertices[23]._position = p[6];

        vertices[20]._normal = -vec3::yaxis;
        vertices[21]._normal = -vec3::yaxis;
        vertices[22]._normal = -vec3::yaxis;
        vertices[23]._normal = -vec3::yaxis;

        vertices[20]._texcoord = vec2(1.0f, 1.0f);
        vertices[21]._texcoord = vec2(0.0f, 1.0f);
        vertices[22]._texcoord = vec2(0.0f, 0.0f);
        vertices[23]._texcoord = vec2(0.0f, 1.0f);

        for (int i=0; i<_vertex_count; i++)
        {
            vertices[i]._color = kege::fvec4( 1.0f );
        }

        uint32_t indices[] =
        {
            // front plane
            0 , 1 , 2 , 2 , 3 , 0 ,
            // back plane
            4 , 5 , 6 , 6 , 7 , 4 ,
            // left plane
            8 , 9 , 10, 10, 11, 8 ,
            // right plane
            12, 13, 14, 14, 15, 12,
            // left plane
            16, 17, 18, 18, 19, 16,
            // right plane
            20, 21, 22, 22, 23, 20
        };
        uint64 isize = sizeof( indices );
        _index_data.resize( isize );
        memcpy( _index_data.data(), indices, isize );
    }
}


namespace kege
{
    SubDividedCuboid::SubDividedCuboid( int divisions, const kege::vec3& center, const kege::vec3& lengths )
    {
        int v, i, face, start_index, a[4];
        const int X = 0, Y = 1, Z = 2;
        const int faces = 6;
        const int length = divisions + 1;
        const int offset = length * 0.5;
        const int total_vertices = faces * length * length;
        const int total_indices = 6 * faces * divisions * divisions;
        float scale = (1.0 / float( offset ));
        float x, y;

        _attributes = kege::Vertex::attributes();
        _vertex_stride = sizeof( kege::Vertex );
        _index_type = core::Index::UI32;
        _vertex_count = total_vertices;
        _index_count = total_indices;

        _vertex_data.resize( total_vertices * sizeof( kege::Vertex ) );
        Vertex* vertices = reinterpret_cast< kege::Vertex* >( _vertex_data.data() );

        _index_data.resize( total_indices * sizeof( uint32_t ) );
        uint32_t* indices = reinterpret_cast< uint32_t* >( _index_data.data() );

        kege::mat33 axes[] =
        {
            kege::m33LookAt( kege::vec3(0.0f, 0.0f, 0.0f), kege::vec3( 1.0f,  0.0f,  0.0f), kege::vec3(0.0f,  1.0f,  0.0f) ), // RIGHT
            kege::m33LookAt( kege::vec3(0.0f, 0.0f, 0.0f), kege::vec3(-1.0f,  0.0f,  0.0f), kege::vec3(0.0f,  1.0f,  0.0f) ), // LEFT
            kege::m33LookAt( kege::vec3(0.0f, 0.0f, 0.0f), kege::vec3( 0.0f,  1.0f,  0.0f), kege::vec3(0.0f,  0.0f,  1.0f) ), // TOP
            kege::m33LookAt( kege::vec3(0.0f, 0.0f, 0.0f), kege::vec3( 0.0f, -1.0f,  0.0f), kege::vec3(0.0f,  0.0f, -1.0f) ), // BELOW
            kege::m33LookAt( kege::vec3(0.0f, 0.0f, 0.0f), kege::vec3( 0.0f,  0.0f,  1.0f), kege::vec3(0.0f,  1.0f,  0.0f) ), // FRONT
            kege::m33LookAt( kege::vec3(0.0f, 0.0f, 0.0f), kege::vec3( 0.0f,  0.0f, -1.0f), kege::vec3(0.0f,  1.0f,  0.0f) ), // BACK
        };

        v = 0;
        i = 0;
        start_index = 0;
        for ( face = 0 ; face < faces ; face++ )
        {
            for ( y = 0.0f ; y < length ; y += 1.0f )
            {
                for ( x = 0.0f ; x < length ; x += 1.0f )
                {
                    vertices[ v ]._position  = center + lengths.x * axes[ face ][ Z ] * (   offset   ) * scale;
                    vertices[ v ]._position += center + lengths.y * axes[ face ][ X ] * ( x - offset ) * scale;
                    vertices[ v ]._position += center + lengths.z * axes[ face ][ Y ] * ( y - offset ) * scale;

                    vertices[ v ]._texcoord = kege::vec2f(x,y) / float( length );
                    vertices[ v ]._normal = axes[ face ][ Y ];
                    v++;
                }
            }

            for ( y = 0 ; y < divisions ; y++ )
            {
                for ( x = 0 ; x < divisions ; x++ )
                {
                    a[ 0 ] = x + y * length + start_index;
                    a[ 1 ] = a[ 0 ] + 1;
                    a[ 2 ] = a[ 0 ] + length;
                    a[ 3 ] = a[ 2 ] + 1;

                    indices[ i++ ] = a[ 0 ];
                    indices[ i++ ] = a[ 1 ];
                    indices[ i++ ] = a[ 3 ];
                    indices[ i++ ] = a[ 3 ];
                    indices[ i++ ] = a[ 2 ];
                    indices[ i++ ] = a[ 0 ];
                }
            }
            start_index = v;
        }
    }
}


namespace kege
{
    CubicSphere::CubicSphere( int divisions, const kege::vec3& center, const kege::vec3& lengths )
    :   SubDividedCuboid( divisions, center, lengths )
    {
        Vertex* vertices = reinterpret_cast< kege::Vertex* >( _vertex_data.data() );
        for ( uint32_t i = 0 ; i < _vertex_count ; i++ )
        {
            vertices[i]._normal = kege::normalize( vertices[i]._position );
            vertices[i]._position = vertices[i]._normal * lengths;
        }

    }
}

