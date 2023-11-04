//
//  core.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 1/27/23.
//

#ifndef kege_core_hpp
#define kege_core_hpp
#include <stdio.h>
#include <vector>
#include <string>
#include "cgm.hpp"
#include "Ref.hpp"
#include "kege-assert.hpp"
#include "kege-logger.hpp"
#include "PoolSysm.hpp"

#define KEGE_INVALID_ID (uint32_t)(4294967296)

namespace kege{namespace core{

    struct Primitive
    {
        enum Type
        {
            POINT_LIST,
            LINE_LIST,
            LINE_STRIP,
            TRIANGLE_LIST,
            TRIANGLE_STRIP,
            TRIANGLE_FAN,
            LINE_LIST_WITH_ADJACENCY,
            LINE_STRIP_WITH_ADJACENCY,
            TRIANGLE_LIST_WITH_ADJACENCY,
            TRIANGLE_STRIP_WITH_ADJACENCY,
            PATCH_LIST,
        };
    };

    struct Polygon
    {
        enum Mode
        {
            FILL,
            LINE,
            POINT,
        };
    };


    struct CullMode
    {
        enum Type
        {
            CW,
            CCW,
            CW_CCW,
        };
    };

    struct VertexInputRate
    {
        enum Enum
        {
            PER_VERTEX,
            PER_INSTANCE,
            MAX_ENUM,
        };
    };

}}
#endif /* kege_core_hpp */
