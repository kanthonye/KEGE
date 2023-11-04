//
//  Binding.hpp
//  kege-rendering-engine
//
//  Created by Kenneth A Esdaile on 6/7/22.
//

#ifndef Binding_hpp
#define Binding_hpp
#include <unordered_map>
#include "Ref.hpp"
#include "kege-string.hpp"
#include "Keycodes.hpp"
namespace kege{

    class Keybind
    {
    public:
        
        Keybind( const kege::string& name, const int& event, const int& key, bool toggle );

        std::unordered_map< int, double > _keys;
        kege::string _name;

        int _active_keys;
        int _event;
        
        bool _key_down;
        bool _toggle;
        bool _fired;
    };
    
}
#endif /* Binding_hpp */
