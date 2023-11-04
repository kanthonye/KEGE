//
//  VirDir.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 5/27/16.
//  Copyright © 2016 Kenneth Anthony Esdaile. All rights reserved.
//

#ifndef VirDir_hpp
#define VirDir_hpp
#include <vector>
#include <unordered_map>
#include "kege-string.hpp"
#include "FileReader.hpp"
#include "Ref.hpp"
namespace kege{

    class VirDir : public kege::RefCounter
    {
    public:

        void dir( const kege::string& name, const kege::string& path );
        kege::FileReader open( const kege::string& name );
        kege::string fetch( const kege::string& name );
        bool save( const kege::string& filename );
        bool load( const kege::string& filename );
        VirDir();

    private:

        std::unordered_map< unsigned long, std::vector< std::pair<kege::string, uint32_t> > > _virmap;
        std::vector< kege::string > _directories;
    };


    kege::string readfile( const kege::string& filename );
}
#endif /* vsf_hpp */


