//
//  Ellipsoid.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 3/1/23.
//

#include "Ellipsoid.hpp"
namespace kege
{
    Ellipsoid::Ellipsoid( float x_radius, float y_radius, int cols, int rows )
    {
        // make sure the length of the x, y, radius is not zero.
        x_radius = ((x_radius<0.0f) ? 1.0f: x_radius);
        y_radius = ((y_radius<0.0f) ? 1.0f: y_radius);

        // calculate the size of the vertices
        cols = (cols<3) ?3 :cols;
        rows = (rows<2) ?2 :rows;

        _attributes.push_back({ 0, kege::Vertex::POSITION, kege::Vertex::F16_3, 0 });
        _attributes.push_back({ 0, kege::Vertex::NORMAL,   kege::Vertex::F16_3, kege::Vertex::sizeOf( kege::Vertex::F16_3 ) });
        _attributes.push_back({ 0, kege::Vertex::TEXCOORD, kege::Vertex::F16_2, kege::Vertex::sizeOf( kege::Vertex::F16_3 ) + _attributes[1]._offset });
        _attributes.push_back({ 0, kege::Vertex::TANGENT,  kege::Vertex::F16_3, kege::Vertex::sizeOf( kege::Vertex::F16_2 ) + _attributes[2]._offset });
        _attributes.push_back({ 0, kege::Vertex::COLOR,    kege::Vertex::F16_4, kege::Vertex::sizeOf( kege::Vertex::F16_3 ) + _attributes[3]._offset });

        _vertex_stride = sizeof( kege::Vertex );
        _index_type = core::Index::UI32;
        _index_count = 6 * cols * rows;

        genEllipsoidVertices( x_radius, y_radius, cols, rows, _vertex_data );
        genEllipsoidIndices( cols, rows, _index_data );

        _vertex_count = uint32_t(_vertex_data.size() / _vertex_stride);
        _index_count = uint32_t(_index_data.size() / 4);
    }
    
    void genEllipsoidVertices( float x_radius, float y_radius, int cols, int rows, std::vector< char >& buffer )
    {
        kege::vec2 angle;
        float ux, uy, cx, cy, sx, sy;
        int i,x,y;

        const float radianX = (6.28318530718 / (float)cols); // 360/col
        const float radianY = (3.14159265359 / (float)rows); // 180/row
        const float half_pi =  1.57079632679;
        const int   vsize = (rows + 1) * (cols + 1);

        buffer.resize( vsize * sizeof( kege::Vertex ) );
        kege::Vertex* vertices = (kege::Vertex*) buffer.data();

        i = 0;
        for (y=0; y<=rows; y++)
        {
            angle.y() = half_pi + (radianY * y);
            cy = cos(angle.y());
            sy = sin(angle.y());

            for (x=0; x<=cols; x++)
            {
                angle.x() = radianX * x;
                cx = cos(angle.x());
                sx = sin(angle.x());

                ux = sx * cy;
                uy = cx * cy;

                vertices[i]._position = kege::fvec3( ux * x_radius, sy * y_radius, uy * x_radius );
                vertices[i]._texcoord = kege::fvec2( 1.0f - float(x)/float(cols), 1.0f - float(y)/float(rows) );
                vertices[i]._normal   = kege::normalize( vertices[i]._position );
                vertices[i]._color    = kege::fvec4( 1.0f );
                
                i++;
            }
        }
    }

    void genEllipsoidIndices( int cols, int rows, std::vector< char >& buffer )
    {
        buffer.resize( 6 * cols * rows * sizeof(uint32_t) );
        uint32_t* indices = (uint32_t*) buffer.data();
        int a, b, c, d, x, y, i = 0;

        for (y=0; y<rows; y++)
        {
            for (x=0; x<cols; x++)
            {
                a = x + cols * y + y*1;
                b = a + 1;
                c = a + cols + 1;
                d = c + 1;

                indices[ i++ ] = a;
                indices[ i++ ] = c;
                indices[ i++ ] = d;

                indices[ i++ ] = d;
                indices[ i++ ] = b;
                indices[ i++ ] = a;
            }
        }
    }
}
