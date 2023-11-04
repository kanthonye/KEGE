//
//  System.hpp
//  PoolSysm
//
//  Created by Kenneth Esdaile on 3/11/23.
//

#ifndef System_hpp
#define System_hpp
#include <stdio.h>
#include <set>
#include <map>
#include <mutex>
#include <string>
#include "kege-string.hpp"
#define POOL_INDEX_8_BIT_INTEGER
#include "cgm.hpp"
#include "ECO.hpp"
#include "ECM.hpp"
#include "Ref.hpp"
namespace kege{

    class Message;

    class System : public kege::RefCounter
    {
    public:

        virtual void update( const double& dms ){}
        virtual void render( const double& dms ){}
        virtual void uninit(){}
        virtual bool init();

        const kege::string& name()const;

        System( const kege::string& name  );
        virtual ~System();

    protected:

        kege::string _name;
    };
}
#endif /* System_hpp */
