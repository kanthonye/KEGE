//
//  Shader.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 8/4/23.
//

#ifndef KEGE_CORE_SHADER_HPP
#define KEGE_CORE_SHADER_HPP
#include <vector>
#include <stdlib.h>
#include "kege-string.hpp"
#include "../../device/core/Abstraction.h"
#include "../../device/core/RenderPass.hpp"
#include "../../device/core/graphics-types.hpp"
#include "../../device/core/UniformLayout.hpp"
#include "../../geometry/Geometry.hpp"
namespace kege{namespace vk{
    class Shader;
}}

namespace kege{namespace core{

    class CommandBuffer;

    class Shader : public kege::RefCounter
    {
    public:

        struct InputRate
        {
            uint32_t binding;
            uint32_t stride;
            core::VertexInputRate::Enum rate;
        };

        enum Resource
        {
            BUFFER,
            IMAGE,
        };

        enum BindPoint
        {
            PIPELINE_BIND_POINT_GRAPHICS,
            PIPELINE_BIND_POINT_COMPUTE
        };

        enum StageType
        {
            VERTEX          = 0b00000001,
            FRAGMENT        = 0b00000010,
            TESS_CONTROL    = 0b00000100,
            TESS_EVALUATION = 0b00001000,
            GEOMETRY        = 0b00010000,
            COMPUTE         = 0b00100000
        };

        typedef std::pair< core::Shader::StageType, kege::string > Stage;
        typedef std::vector< core::Shader::InputRate > VertexInputRates;
        typedef std::vector< core::Shader::Stage > PipelineStages;

        struct Info
        {
            std::vector< std::pair< core::Shader::StageType, kege::string > > _shader_stages;
            core::Shader::VertexInputRates _vertex_input_rates;
            kege::core::Primitive::Type _primitive_topology;
            core::Polygon::Mode _polygon_mode;
            core::CullMode::Type _cull_mode;
            kege::string _pipeline_type;
            kege::string _name;
        };

    public:

        
        virtual void render
        (
            kege::Geometry* geometry,
            uint32_t first_instance,
            uint32_t instance_count
        )const = 0;

        virtual void bind
        (
            kege::Geometry* geometry
        )const = 0;

        virtual void pushConstants
        (   uint32_t stages
         ,  uint32_t offset
         ,  uint32_t size
         ,  const void* data  ) = 0;

        virtual void despatch( uint32_t num_groups_x, uint32_t num_groups_y, uint32_t num_groups_z ) = 0;
        virtual void bindUniformLayoutByName( const char* name, const core::UniformLayout* layout ) = 0;
        virtual void bindUniformLayout( uint32_t setindex, const core::UniformLayout* layout ) = 0;
        virtual int32_t getShaderResourceIndex( const char* name )const = 0;

        virtual void bind( core::CommandBuffer* command )const = 0;
        virtual void destruct() = 0;

        virtual const vk::Shader* vulkan()const{return nullptr;}
        virtual vk::Shader* vulkan(){return nullptr;}

        void setName( const kege::string& name ){ _name = name; }
        const kege::string& getName(){ return _name; }

        KEGE_CORE_ABSTRACTION( Shader );

        kege::string _name;
    };

}}
#endif /* Shader_hpp */
