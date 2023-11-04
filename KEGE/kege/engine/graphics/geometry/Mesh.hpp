//
//  Mesh.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 4/9/23.
//

#ifndef KEGE_MESH_HPP
#define KEGE_MESH_HPP

#include "Ref.hpp"

namespace kege
{
    class MeshRenderer;

    class Mesh : public kege::RefCounter
    {
    public:

        virtual void draw( MeshRenderer& renderer ) = 0;
        virtual ~Mesh(){}
    };

}
#endif /* KEGE_MESH_HPP */
