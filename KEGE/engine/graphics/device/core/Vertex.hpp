//
//  Vertex.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 8/9/23.
//

#ifndef KEGE_VERTEX_HPP
#define KEGE_VERTEX_HPP

#include <vector>
#include "cgm.hpp"
#include "../../device/core/Buffer.hpp"

namespace kege
{
    struct Vertex
    {
        typedef enum Format
        {
            U8_1, U16_1, U32_1, F16_1, F32_1, F64_1,
            U8_2, U16_2, U32_2, F16_2, F32_2, F64_2,
            U8_3, U16_3, U32_3, F16_3, F32_3, F64_3,
            U8_4, U16_4, U32_4, F16_4, F32_4, F64_4,
        } Format;

        typedef enum Location
        {
            POSITION = 0,
            NORMAL   = 1,
            TEXCOORD = 2,
            TANGENT = 3,
            COLOR = 4
        } Location;

        struct Attribute
        {
            uint32_t _binding;
            uint32_t _location;
            Vertex::Format _format;
            uint32_t _offset;
        };

        struct Binding
        {
            kege::Ref< core::Buffer > _buffer;
            uint32_t _stride;
            core::VertexInputRate::Enum _input_rate;
        };

        static uint32_t sizeOf( Format format )
        {
            switch ( format )
            {
            case U8_1:  return sizeof(char);
            case U8_2:  return sizeof(char) * 2;
            case U8_3:  return sizeof(char) * 3;
            case U8_4:  return sizeof(char) * 4;

            case U16_1: return sizeof(short);
            case U16_2: return sizeof(short) * 2;
            case U16_3: return sizeof(short) * 3;
            case U16_4: return sizeof(short) * 4;

            case U32_1: return sizeof(int);
            case U32_2: return sizeof(int) * 2;
            case U32_3: return sizeof(int) * 3;
            case U32_4: return sizeof(int) * 4;

            case F16_1: return sizeof(float);
            case F16_2: return sizeof(float) * 2;
            case F16_3: return sizeof(float) * 3;
            case F16_4: return sizeof(float) * 4;

            case F32_1: return sizeof(double);
            case F32_2: return sizeof(double) * 2;
            case F32_3: return sizeof(double) * 3;
            case F32_4: return sizeof(double) * 4;

            default: throw std::runtime_error( "Unspported vertex format" );
            }
        }

        static std::vector< Vertex::Attribute > attributes()
        {
            std::vector< Vertex::Attribute > attributes;
            if ( attributes.empty() )
            {
                attributes.push_back({ 0, POSITION, F16_3, 0 });
                attributes.push_back({ 0, NORMAL,   F16_3, sizeOf( F16_3 ) });
                attributes.push_back({ 0, TEXCOORD, F16_2, sizeOf( F16_3 ) + attributes[1]._offset });
                attributes.push_back({ 0, TANGENT,  F16_3, sizeOf( F16_2 ) + attributes[2]._offset });
                attributes.push_back({ 0, COLOR,    F16_4, sizeOf( F16_3 ) + attributes[3]._offset });
            }
            return attributes;
        }

        kege::fvec3 _position;
        kege::fvec3 _normal;
        kege::fvec2 _texcoord;
        kege::fvec3 _tangent;
        kege::fvec4 _color;
    };

}
#endif /* Vertex_hpp */
