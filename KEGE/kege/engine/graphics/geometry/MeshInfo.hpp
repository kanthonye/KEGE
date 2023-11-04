//
//  MeshInfo.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 6/22/23.
//

#ifndef KEGE_MESH_DATA_HPP
#define KEGE_MESH_DATA_HPP

#include <stdlib.h>
#include <vector>
#include <string>
#include "Ref.hpp"
#include "kege-string.hpp"
#include "../geometry/Geometry.hpp"
#include "../device/core/Buffer.hpp"

namespace kege
{
    struct MaterialPropertyData : public kege::RefCounter
    {
        std::vector< std::pair< kege::string, kege::string > > _textures;
        std::vector< std::pair< kege::string, kege::string > > _parameters;
        std::vector< std::pair< kege::string, kege::string > > _shader_params;
    };

    struct MaterialData : public kege::RefCounter
    {
        std::vector< kege::Ref< MaterialPropertyData > > _properties;
    };

    struct MeshInfo : public kege::RefCounter
    {
        kege::Ref< kege::Geometry > build()const;
        std::vector< Vertex::Attribute > _attributes;
        std::vector< char > _vertex_data;
        std::vector< char > _index_data;
        core::Index::Type _index_type;
        uint32_t _vertex_stride;
        uint32_t _vertex_count;
        uint32_t _index_count;
    };

}
#endif /* KEGE_MESH_DATA_HPP */
