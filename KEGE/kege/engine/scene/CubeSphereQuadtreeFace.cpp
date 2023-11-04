//
//  CubeSphereQuadtreeFace.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/14/23.
//

#include "CubeSphereQuadtreeFace.hpp"
namespace kege
{
    
    CubeSphereQuadtreeNode::~CubeSphereQuadtreeNode()
    {
        if ( _children )
        {
            delete _children;
            _children = nullptr;
        }
    }

    CubeSphereQuadtreeNode::CubeSphereQuadtreeNode()
    :   _children( nullptr )
    ,   _position( 0.0 )
    ,   _sphere( 0.0 )
    ,   _cube( 0.0 )
    ,   _depth( 0 )
    {}


}
