//
//  CubeFaceHeightmapGenerator.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/17/23.
//

#ifndef CubeFaceHeightmapGenerator_hpp
#define CubeFaceHeightmapGenerator_hpp

#include <vector>
#include "cgm.hpp"
#include "KEGE.hpp"
#include "Core.hpp"
#include "Graphics.hpp"
#include "UniformLayout.hpp"

namespace kege
{

    class CubeFaceHeightmapGenerator
    {
    public:

        struct Params
        {

            vec3  OFFSET  = kege::vec3( 32.f );
            int   OCTAVES = 6;
            float PERSISTENCE = 0.5;
            float LACUNARITY = 3;
            float SCALE = 0.2;
            float STEEPNESS = 2;
            float STRENGTH = 1;
            float SMAX = 0.0;
            float SMIN = 1.0;
            int   FACE_INDEX;
            float NORMAL_MAP_STRENGTH = 8;
        };

        void generatorHeightmap( const CubeFaceHeightmapGenerator::Params& params, std::vector< kege::Ref< core::Texture > > &textures );
        static std::vector< kege::Ref< core::Texture > > createCubeFaceTextures( uint32_t width, uint32_t height );
        void waitForExecutionToFinish();

        CubeFaceHeightmapGenerator();

        kege::Ref< core::CommandBuffer > _primary_command_buffer;
        kege::Ref< core::CommandBuffer > _command_buffers[ 6 ];
        kege::Ref< core::Shader > _heightmap_compute_shader;
        kege::Ref< core::Shader > _normalmap_compute_shader;

        kege::UniformLayout1 _cube_face_points;
    };
}
#endif /* CubeFaceHeightmapGenerator_hpp */

