//
//  GUIElem.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/11/23.
//

#ifndef GUIElem_hpp
#define GUIElem_hpp

#include "cgm.hpp"
#include <vector>
namespace kege{

    struct GUIChild;

    struct GUIEvent
    {
        enum
        {
            MOUSE_ENTER,
            MOUSE_EXIT,
            UNSELECTED,
            SELECTED,
            PRESSING,
            RELEASE,
            DROPOFF,
            MOVING,
        };
    };

    
    struct GUIElem
    {
        const std::vector< GUIChild >* children()const;
        std::vector< GUIChild >* children();

        enum
        {
            INACTIVE,
            SELECTABLE,
            MOVEABLE,
            RESIZABLE,
            VISIBLE,
            DRAWABLE,
        };

        enum State
        {
            MOUSE_ENTER,
            SELECTED,
            PRESSING,
            MOVING,
        };

        kege::vec4 _transform;
        kege::vec4 _color;
        kege::vec4 _texel;
        kege::real _height;
        kege::real _width;
        uint64_t   _flags;
        uint64_t   _state;
    };

    struct GUIChild
    {
        kege::vec4 _transform;
        GUIElem*  _elem;
    };

    struct GUINode : public GUIElem
    {
        std::vector< GUIChild > _children;
    };

}
#endif /* GUIElem_hpp */
