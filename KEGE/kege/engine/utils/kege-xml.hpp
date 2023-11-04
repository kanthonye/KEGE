//
//  kege-xml.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 6/12/23.
//

#ifndef kege_xml_hpp
#define kege_xml_hpp
#include <stack>
#include <vector>
#include "Ref.hpp"
#include "kege-string.hpp"
namespace kege{namespace xml{

    struct attribute : public kege::RefCounter
    {
        attribute(const string& n, const string& v)
        :   name( n )
        ,   value( v )
        {}
        string name;
        string value;
    };

    struct attributes : public kege::RefCounter
    {
        typedef std::vector< Ref< attribute > > content;

        const attribute* get( const string& name, const string& value )const;
        attribute* get( const string& name, const string& value );

        const attribute* get( const string& tag )const;
        attribute* get( const string& tag );
        void remove( const string& tag );
        void add( Ref< attribute > elem );

        typedef content::iterator iterator;
        iterator begin();
        iterator end();

        void print(bool all,int n);

    private:

        content _content;
    };
}}




namespace kege{namespace xml{

    class element : public kege::RefCounter
    {
    public:

        typedef std::vector< Ref< element > > list;
        typedef list::const_iterator const_iterator;
        typedef list::iterator iterator;

        virtual const_iterator begin()const;
        virtual const_iterator end()const;
        virtual iterator begin();
        virtual iterator end();

        enum enum_type { CONTENT, NODE };

        virtual const element* get_by_attrib( const string& name, const string& value )const;
        virtual element* get_by_attrib( const string& name, const string& value );

        virtual const element* get( const string& tag )const;
        virtual element* get( const string& tag );
        virtual void remove( const string& tag );
        virtual void add( Ref< element > elem );

        virtual void set( string& data );
        virtual const string& get()const;
        virtual string& get();

        const attribute* get_attrib( const string& name, const string& value )const;
        attribute* get_attrib( const string& name, const string& value );
        void add_attrib( const string& name, const string& value );
        const attribute* get_attrib( const string& name )const;
        attribute* get_attrib( const string& name );
        void remove_attrib( const string& name );

        void set_attributes( Ref< attributes > attributes );
        attributes* get_attributes();

        void set_tag( const string& tag );
        const string& get_tag()const;

        enum_type get_node_type()const;

        virtual void print(bool all,int n);
        virtual ~element();

    protected:

        element( enum_type t, const string& tag, Ref< attributes > attributes );
        element( enum_type t, const string& tag );
        element( enum_type t );

    protected:

        Ref< attributes > _attributes;
        enum_type _xml_type;
        string _tag;

        string _content;
        static string _dud;
    };
}}


namespace kege{namespace xml{

    class content : public element
    {
    public:

        void set( string& data );
        const string& get()const;
        string& get();

        content( const string& tag, Ref< attributes > attribs, string& content );
        content( const string& tag, Ref< attributes > attribs );
        content( const string& tag );
        content();

    protected:

        string _content;
    };
}}

namespace kege{namespace xml{

    class node : public element
    {
    public:

        const element* get_by_attrib( const string& name, const string& value )const;
        element* get_by_attrib( const string& name, const string& value );

        const element* get( const string& tag )const;
        element* get( const string& tag );
        void remove( const string& tag );
        void add( Ref< element > elem );

        const_iterator begin()const;
        const_iterator end()const;
        iterator begin();
        iterator end();
        void clear();


        void print(bool all,int n);

        node( const string& tag, Ref< attributes > attribs, Ref< element > content );
        node( const string& tag, Ref< attributes > attribs );
        node( const string& tag );

        ~node();
        node();

    private:

        list _elements;
    };
}}



namespace kege{namespace xml{

    class document : public xml::node
    {
    public:

        bool load(string const& filename);
        ~document();
        document();

    private:

        enum State
        {
            ERROR,
            START,
            NEW_TAG,
            END_TAG,
            NONE_ENDTAG,
            CONTENT,

            ATTRIBUTE,
            ATTRIBUTE_VALUE,
            LT_SLASH,

            GT,
            END_QUOTE,
            FINAL,
        };

        void state_none_endtag();
        void state_attribute();
        void state_content();
        void state_endtag();
        void state_newtag();

        void write_error( const string& erro );
        bool is_letters(int c, State state);
        long seek(const char* flags);
        long skip(const char* flags);
        long skip_whitespace();
        void get_content();
        void get_token();
        int get_char();
        int get_next();
        int get_prev();
        void parse();

    private:

        std::stack< Ref< xml::element > > _stack;
        const char* _prev;
        const char* _curr;
        char* _data;
        int _line;

        string _token;
        State _state;
    };
}}
#endif /* kege_xml_hpp */
