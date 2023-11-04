//
//  AtmosphericScatteringRenderTechnique.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/26/23.
//

#include "../../../include/systems.hpp"
#include "AtmosphereVolumeRenderTechnique.hpp"
namespace kege
{
    void AtmosphereVolumeRenderTechnique::execute( core::CommandBuffer* command )
    {
        if ( !_entities ) return;

        _shader->bind( command );
        _shader->bindUniformLayout( 0, system::Camera::_resource.ref() );
        //_shader->bindUniformLayout( 2, _render_phase->getInputRenderTarget()->getCurrentUniformLayout() );
        _shader->bind( _geometry.ref() );

        ObjectMatrices object_matrices;
        uint32_t sizeof_object_matrices = sizeof( ObjectMatrices );
        uint32_t sizeof_atmosphere = sizeof( component::Atmosphere );

        for ( const auto& id : *_entities )
        {
            Entity entity = id;
            component::Atmosphere* atmosphere = entity.get< component::Atmosphere >();
            _atmosphere_data.getBinding(0)->getBuffer(0)->copyToSystemMemory( sizeof_atmosphere, atmosphere );

            if ( entity.has< component::Child >() )
            {
                component::Child* child = entity.get< component::Child >();
                child->_transform.scale() = atmosphere->_max_altitude + atmosphere->_object_radius;
                object_matrices._transform = buildModelMatrix( child->_transform );
                object_matrices._rotation = child->_transform._rotation;
            }
            else
            {
                component::Transform* transform = entity.get< component::Transform >();
                transform->scale() = atmosphere->_max_altitude + atmosphere->_object_radius;
                object_matrices._transform = buildModelMatrix( *transform );
                object_matrices._rotation = transform->_rotation;
            }

            _shader->pushConstants( core::Shader::VERTEX, 0, sizeof_object_matrices, &object_matrices );
            _shader->bindUniformLayout( 1, &_atmosphere_data );
            _geometry->draw( command, 0, 1 );
        }
    }

    AtmosphereVolumeRenderTechnique::~AtmosphereVolumeRenderTechnique()
    {
        _entities = nullptr;
    }

    AtmosphereVolumeRenderTechnique::AtmosphereVolumeRenderTechnique()
    :   kege::RenderTechnique( "atmosphere-volume.shader" )
    ,   _entities( nullptr )
    {
        _entities = Entity::egm()->get( createComponentBitmask< component::Transform, component::Atmosphere >()._bitmask );

        AtmosphereMeshInfo m( 1, 1 , 64,64 );
        _geometry = m.build();


        kege::Ref< core::Buffer > buffer = KEGE::core()->graphics()->createBuffer
        (
            core::Buffer::STORAGE,
            6 * sizeof( kege::mat34 ),
            nullptr,
            core::Buffer::DYNAMIC
        );

        _atmosphere_data.setBinding
        (
            0, // store binding at index 0
            new kege::BufferBinding
            (
                "AtmosphereData",  // binding target name. note this must match the name of the target in the shader
                0, // the binding index in shader. note this must match the index of the target in the shader
                KEGE::core()->graphics()->createBuffer // the buffer to bind at the target index
                (
                    core::Buffer::UNIFORM,
                    sizeof( component::Atmosphere ),
                    nullptr, core::Buffer::DYNAMIC
                )
            )
        );
    }

}




namespace kege
{
    AtmosphereMeshInfo::AtmosphereMeshInfo( float x_radius, float y_radius, int cols, int rows )
    {
        // make sure the length of the x, y, radius is not zero.
        x_radius = ((x_radius<0.0f) ? 1.0f: x_radius);
        y_radius = ((y_radius<0.0f) ? 1.0f: y_radius);

        // calculate the size of the vertices
        cols = (cols<3) ?3 :cols;
        rows = (rows<2) ?2 :rows;

        genEllipsoidVertices( x_radius, y_radius, cols, rows );
        genEllipsoidIndices( cols, rows );
    }

    void AtmosphereMeshInfo::genEllipsoidVertices( float x_radius, float y_radius, int cols, int rows )
    {
        kege::vec2 angle;
        float ux, uy, cx, cy, sx, sy;
        int i,x,y;

        const float radianX = (6.28318530718 / (float)cols); // 360/col
        const float radianY = (3.14159265359 / (float)rows); // 180/row
        const float half_pi =  1.57079632679;
        
        _vertex_stride = sizeof( kege::VertexPT );
        _vertex_count = (rows + 1) * (cols + 1);
        _vertex_data.resize( _vertex_count * _vertex_stride );
        _attributes.push_back({ 0, 0, kege::Vertex::F16_3, 0 });
        _attributes.push_back({ 0, 1, kege::Vertex::F16_2, 0 });

        kege::VertexPT* vertices = (kege::VertexPT*) _vertex_data.data();

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

                vertices[i].position = kege::fvec3( ux * x_radius, sy * y_radius, uy * x_radius );
                vertices[i].texcoord = kege::fvec2( 1.0f - float(x)/float(cols), 1.0f - float(y)/float(rows) );

                i++;
            }
        }
    }

    void AtmosphereMeshInfo::genEllipsoidIndices( int cols, int rows )
    {
        _index_type = core::Index::UI32;
        _index_count = 6 * cols * rows;
        _index_data.resize( _index_count * sizeof(uint32_t) );
        uint32_t* indices = (uint32_t*) _index_data.data();
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
