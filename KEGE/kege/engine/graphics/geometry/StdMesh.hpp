//
//  StdMesh.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 8/9/23.
//

#ifndef KEGE_STD_MESH_HPP
#define KEGE_STD_MESH_HPP

#include "../geometry/Mesh.hpp"
#include "../geometry/Geometry.hpp"
#include "../material/Material.hpp"

namespace kege
{
    class StdMesh : public kege::Mesh
    {
    public:

        StdMesh
        (
             kege::Ref< kege::Geometry > geometry,
             kege::Ref< kege::Material > material
        );

        const kege::Geometry* geometry()const;
        const kege::Material* material()const;

        void draw( MeshRenderer& renderer );
        
        kege::Geometry* geometry();
        kege::Material* material();

        ~StdMesh();
        StdMesh();

    protected:

        kege::Ref< kege::Geometry > _geometry;
        kege::Ref< kege::Material > _material;
    };
}
#endif /* KEGE_STD_MESH_HPP */
