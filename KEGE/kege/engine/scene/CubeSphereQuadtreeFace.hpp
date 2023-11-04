//
//  CubeSphereQuadtreeFace.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/14/23.
//

#ifndef CubeSphereQuadtreeFace_hpp
#define CubeSphereQuadtreeFace_hpp

#include "BatchManager.hpp"

namespace kege{

    class CubeSphereQuadtree;
    struct CubeSphereQuadtreeChildren;

    /**
     * @enum FaceId
     *
     * FaceId is an enumeration representing the faces of a spherical terrain. It
     * includes FRONT, BACK, LEFT, RIGHT, ABOVE, BELOW, and MAX_FACE_COUNT for
     * reference.
     */
    enum CubeSphereFaceType{ FRONT, BACK, LEFT, RIGHT, ABOVE, BELOW, MAX_FACE_COUNT };

    struct CubeSphereQuadtreeNode
    {
        ~CubeSphereQuadtreeNode();
        CubeSphereQuadtreeNode();

        CubeSphereQuadtreeChildren* _children;
        kege::fvec3 _position;
        kege::fvec4 _sphere;
        kege::fvec4 _cube;
        uint16_t _depth;
    };

    struct CubeSphereQuadtreeChildren
    {
        CubeSphereQuadtreeNode _nodes[4];
    };

    struct CubeSphereQuadtreeFace
    {
        CubeSphereQuadtreeNode _quadtree;
        CubeSphereFaceType _face_type;
        kege::vec3 _xaxis;
        kege::vec3 _yaxis;
    };

}
#endif /* CubeSphereQuadtreeFace_hpp */
