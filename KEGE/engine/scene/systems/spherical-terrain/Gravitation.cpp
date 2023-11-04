//
//  Gravitation.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/18/23.
//

#include "../../systems/include/systems.hpp"

namespace kege{ namespace system{

    static const double GRAVITY_CONSTANT = 6.674e-11;

    void Gravitation::render( const double& dms )
    {}
    
    void Gravitation::update( const double& dms )
    {
        if ( !_entities ) return;

        for( std::set< EntityId >::iterator g = _entities->begin(); g != _entities->end(); g++ )
        {
            Entity e1 = (*g);
            component::Mass* m1 = e1.get< component::Mass >();
            component::Transform* t1 = e1.get< component::Transform >();
            component::LinearMotion* l1 = e1.get< component::LinearMotion >();

            std::set< EntityId>::iterator j = g;
            j++;
            
            for(; j != _entities->end(); j++ )
            {
                Entity e2 = (*j);
                component::Mass* m2 = e2.get< component::Mass >();
                component::Transform* t2 = e2.get< component::Transform >();
                component::LinearMotion* l2 = e2.get< component::LinearMotion >();

                kege::vec3 dir = t1->position() - t2->position();
                double r = magn( dir );
                dir = dir / r;

                double F = (GRAVITY_CONSTANT * m1->_mass * m2->_mass) / (r*r); // F = G * (m1 * m2) / r^2

                t1->worldUp() += dir;
                t2->worldUp() -= dir;

                l1->_netforce -= (dir * F);
                l2->_netforce += (dir * F);
                //std::cout <<r <<" F = " << F << " GRAVITY_CONSTANT = " << GRAVITY_CONSTANT <<'\n';
            }
        }

        for( kege::Entity e : *_entities )
        {
            component::Transform* t = e.get< component::Transform >();
            t->worldUp() = normalize( t->worldUp() );
        }
    }
    Gravitation::Gravitation( int priority )
    :   kege::EntitySystem( priority, "Gravitation" )
    {
        _components = createComponentBitmask< component::Transform, component::LinearMotion, component::Mass >();
    }

}}
