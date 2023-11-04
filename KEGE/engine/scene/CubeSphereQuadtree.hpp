//
//  CubeSphereQuadtree.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/14/23.
//

#ifndef CubeSphereQuadtree_hpp
#define CubeSphereQuadtree_hpp

#include "CubeSphereQuadtreeFace.hpp"

namespace kege{

    /**
     * @class CubeSphereQuadtree
     *
     * CubeSphereQuadtree is a class that represents a spherical quadtree terrain
     * used for rendering landscapes in a 3D environment. It subdivides the terrain
     * into smaller tiles for efficient rendering and level-of-detail management. 
     *
     * @note This class is part of a larger system for managing and rendering spherical
     * terrain and assumes the existence of certain external classes and structures.
     *
     * @see SphericalQuadtreeLandscape, CubeSphereQuadtreeNode
     */
    class CubeSphereQuadtree// : public QuadtreeTerrain
    {
    public:
        
        //enum{ QUADTREE_BATCH_SIZE = 4096 };
        
        /**
         * @fn updateWorldPosition
         *
         * Updates the world position of a specific CubeSphereQuadtreeNode in the terrain quadtree.
         *
         * @param quadtree The CubeSphereQuadtreeNode to update.
         */
        void updateWorldPosition
        (
            CubeSphereQuadtreeNode& quadtree
        );

        /**
         * @fn getResolution
         *
         * Calculates and returns the resolution of a terrain quadtree based on the
         * camera's position.
         *
         * @param quadtree The terrain quadtree.
         * @param eye The camera's position.
         *
         * @return The resolution of the quadtree.
         */
        double getResolution
        (
            CubeSphereQuadtreeNode& quadtree,
            const kege::vec3& eye
        );

        /**
         * @fn isLeaf
         *
         * Checks if a given terrain quadtree node is a leaf node.
         *
         * @param quadtree The terrain quadtree node.
         * @return true if the node is a leaf, false otherwise.
         */
        bool isLeaf
        (
            CubeSphereQuadtreeNode& quadtree
        )const;

        /**
         * @fn split
         *
         * Splits a given terrain quadtree node into smaller terrain quadtree.
         *
         * @param quadtree The terrain quadtree node to be split.
         */
        void split
        (
            CubeSphereQuadtreeFace& root, 
            CubeSphereQuadtreeNode& quadtree
        );

        /**
         * @fn merge
         *
         * Merges a given terrain quadtree node with its neighboring nodes.
         *
         * @param quadtree The terrain quadtree node to be merged.
         */
        void merge
        (
            CubeSphereQuadtreeNode& quadtree
        );

        void update
        (
            const kege::vec3& eye
        );

        /**
         * @fn cullGeometry
         *
         * Culls geometry based on a frustum.
         *
         * @param frustum The frustum for culling.
         */
        void cullGeometry
        (
            const kege::Frustum<float>& frustum
        );

        /**
         * @fn cullGeometry
         *
         * Culls geometry based on a terrain quadtree and updates the count of instances.
         * Before you the draw function one of the cullGeometry function must be called inorder
         * for the batch buffer to be populate with data to render the quadtrre instances,
         *
         * @note This cullGeometry will render all the quadtree leaf nodes
         */
        void cullGeometry
        ();

        const CubeSphereQuadtreeFace* getFace( uint32_t i )const;
        const kege::mat34* getFaceRotationMatrices()const;

        void setRotation( const kege::mat33& rotation );
        void setPosition( const kege::vec3& position );

        /**
         * @fn getBatchBufferManager
         *
         * Get the getBatchBufferManager associated with this object
         *
         * @return The reference to the BatchBufferManager associated with this object
         */
        kege::BatchManager* getBatchBufferManager();

        /**
         * @fn getGeometry
         *
         * Get the Geometry associated with this object
         *
         * @return a reference to the Geometry associated with this object
         */
        const kege::Geometry* getGeometry()const;

        /**
         * @fn buildGeometry
         */
        void buildGeometry();


        /**
         * @fn CubeSphereQuadtree
         *
         * The constructor
         */
        CubeSphereQuadtree
        (
            double radius, double resolution, uint32_t depth, uint32_t binding_index
        );

        /**
         * @fn CubeSphereQuadtree
         *
         * The decstructor
         */
        ~CubeSphereQuadtree
        ();
        
        /**
         * @fn CubeSphereQuadtree
         *
         * The constructor
         */
        CubeSphereQuadtree
        ( uint32_t binding_index );

    private:
        
        void build
        (
            CubeSphereQuadtreeNode& quadtree,
            const int& depth,
            const double& radius,
            const kege::fvec4& cube
        );

        /**
         * @fn update
         *
         * Updates the spherical quadtree terrain, including LOD management and
         * visibility determination.
         *
         * @param eye The camera's position.
         * @param root The parent object of quadtree.
         * @param quadtree The cube sphere quadtree node to update.
         */
        void update
        (
            const kege::vec3& eye,
            CubeSphereQuadtreeFace& root,
            CubeSphereQuadtreeNode& quadtree
        );

        /**
         * @fn build
         *
         * Builds the spherical quadtree terrain for a specific face of the landscape.
         *
         * @param face_id The ID of the face to build.
         * @param points The points to construct the terrain.
         *
         * @public
         */
        void build
        (
            CubeSphereFaceType face_id,
            const kege::mat43& points
        );

        /**
         * @fn cullGeometry
         *
         * Culls geometry based on a frustum and updates the count of instances.
         *
         * @param frustum The frustum for culling.
         * @param quadtree The terrain quadtree.
         * @param instance An array to store quadtree leaf node data.
         * @param count The count of terrain quadtree nodes inside the frustum.
         */
        void cullGeometry
        (
            const kege::Frustum<float>& frustum,
            CubeSphereQuadtreeNode& quadtree,
            kege::mat24* instance,
            uint32_t& count
        );

        /**
         * @fn cullGeometry
         *
         * Culls geometry based on a terrain quadtree and updates the count of instances.
         * Before you the draw function one of the cullGeometry function must be called inorder
         * for the batch buffer to be populate with data to render the quadtrre instances,
         *
         * @note this cullGeometry will render all the quadtree leaf nodes
         *
         * @param quadtree The terrain quadtree.
         * @param instance An array of transformations.
         * @param count The count of instances after culling.
         */
        void cullGeometry
        (
            CubeSphereQuadtreeNode& quadtree,
            CubeSphereFaceType face_type,
            kege::mat24* instance,
            uint32_t& count
        );
        
    private:


        kege::Ref< kege::Geometry > _geometry;
        kege::CubeSphereQuadtreeFace _faces[ 6 ];
        kege::mat34 _face_rotations[ 6 ];
        kege::BatchManager _batch_mngr;
        kege::mat33 _rotation;
        kege::vec3 _position;
        double _max_resolution;
        double _max_depth;
        double _radius;
    };

}
#endif /* CubeSphereQuadtree_hpp */
