//
//  StdMesh.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 8/9/23.
//

#include "StdMesh.hpp"
namespace  kege
{
    StdMesh::StdMesh
    (
         kege::Ref< kege::Geometry > geometry,
         kege::Ref< kege::Material > material
    )
    :   _geometry( geometry )
    ,   _material( material )
    {}

    const kege::Geometry* StdMesh::geometry()const
    {
        return _geometry.ref();
    }

    const kege::Material* StdMesh::material()const
    {
        return _material.ref();
    }

    void StdMesh::draw( MeshRenderer& renderer )
    {

    }
    
    kege::Geometry* StdMesh::geometry()
    {
        return _geometry.ref();
    }

    kege::Material* StdMesh::material()
    {
        return _material.ref();
    }

    StdMesh::~StdMesh()
    {
        _geometry.clear();
        _material.clear();
    }

    StdMesh::StdMesh()
    {}
}
