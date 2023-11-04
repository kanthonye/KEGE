//
//  kege-xml.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 6/12/23.
//

#include "kege-xml.hpp"
namespace kege{namespace xml{

    const attribute* attributes::get( const string& name, const string& value )const
    {
        for (content::const_iterator i = _content.begin() ; i != _content.end(); i++ )
        {
            if ( (*i)->name == name )
            {
                if ( (*i)->value == value )
                {
                    return (*i).ref();
                }
            }
        }
        return nullptr;
    }
    attribute* attributes::get( const string& name, const string& value )
    {
        for (content::iterator i = _content.begin() ; i != _content.end(); i++ )
        {
            if ( (*i)->name == name )
            {
                if ( (*i)->value == value )
                {
                    return (*i).ref();
                }
            }
        }
        return nullptr;
    }

    const attribute* attributes::get( const string& tag )const
    {
        for (content::const_iterator i = _content.begin() ; i != _content.end(); i++ )
        {
            if ( (*i)->name == tag )
            {
                return (*i).ref();
            }
        }
        return nullptr;
    }
    attribute* attributes::get( const string& tag )
    {
        for (content::iterator i = _content.begin() ; i != _content.end(); i++ )
        {
            if ( (*i)->name == tag )
            {
                return (*i).ref();
            }
        }
        return nullptr;
    }
    void attributes::remove( const string& tag )
    {
        for (content::iterator i = _content.begin() ; i != _content.end(); i++ )
        {
            if ( (*i)->name == tag )
            {
                (*i) = nullptr;
                _content.erase( i );
                break;
            }
        }
    }
    void attributes::add( Ref< attribute > elem )
    {
        if ( elem != nullptr )
        {
            _content.push_back( elem );
        }
    }

    attributes::iterator attributes::begin()
    {
        return _content.begin();
    }

    attributes::iterator attributes::end()
    {
        return _content.end();
    }

    void attributes::print(bool all,int n)
    {
        for (content::iterator i = _content.begin() ; i != _content.end(); i++ )
        {
            printf( " %s='%s'", (*i)->name.c_str(), (*i)->value.c_str() );
        }
        printf("\n");
    }

}}



namespace kege{namespace xml{

    element::const_iterator element::begin()const
    {
        return const_iterator();
    }
    element::const_iterator element::end()const
    {
        return const_iterator();
    }
    element::iterator element::begin()
    {
        return iterator();
    }
    element::iterator element::end()
    {
        return iterator();
    }

    const element* element::get_by_attrib( const string& name, const string& value )const
    {
        return nullptr;
    }
    element* element::get_by_attrib( const string& name, const string& value )
    {
        return nullptr;
    }

    const attribute* element::get_attrib( const string& name, const string& value )const
    {
        if ( _attributes == nullptr ) return nullptr;
        return _attributes->get( name, value );
    }
    attribute* element::get_attrib( const string& name, const string& value )
    {
        if ( _attributes == nullptr ) return nullptr;
        return _attributes->get( name, value );
    }
    void element::add_attrib( const string& name, const string& value )
    {
        if ( _attributes == nullptr )
        {
            _attributes = new xml::attributes;
        }
        return _attributes->add( new attribute( name, value ) );
    }
    const attribute* element::get_attrib( const string& name )const
    {
        if ( _attributes == nullptr ) return nullptr;
        return _attributes->get( name );
    }
    attribute* element::element::get_attrib( const string& name )
    {
        if ( _attributes == nullptr ) return nullptr;
        return _attributes->get( name );
    }
    void element::remove_attrib( const string& name )
    {
        if ( _attributes == nullptr ) return;
        _attributes->remove( name );
    }

    void element::set_attributes( Ref< attributes > attributes )
    {
        _attributes = attributes;
    }
    attributes* element::get_attributes()
    {
        return _attributes.ref();
    }

    const element* element::get( const string& tag )const
    {
        return nullptr;
    }
    element* element::get( const string& tag )
    {
        return nullptr;
    }
    void element::remove( const string& tag )
    {}
    void element::add( Ref< element > elem )
    {}

    void element::set(string& content)
    {
        // move the data from content to _content
        _content.move(content);
    }
    const string& element::get()const
    {
        return _content;
    }
    string& element::get()
    {
        return _content;
    }

    void element::set_tag( const string& tag )
    {
        _tag = tag;
    }
    const string& element::get_tag()const
    {
        return _tag;
    }
    element::enum_type element::get_node_type()const
    {
        return _xml_type;
    }
    void element::print(bool all,int n)
    {
        for (int s=0; s<n; s++) printf("-");
        printf("%s :",_tag.c_str());

        if ( _attributes != nullptr )
        {
            _attributes->print( all, n );
        }

        else printf("\n");
    }
    element::element( enum_type t, const string& tag, Ref< attributes > attributes )
    :   _attributes( attributes )
    ,   _xml_type( t )
    ,   _tag( tag )
    {}
    element::element( enum_type t, const string& tag )
    :   _xml_type( t )
    ,   _tag( tag )
    {}
    element::element( enum_type t )
    :   _xml_type( t )
    {}

    element:: ~element()
    {}

    string element::_dud;
}}



namespace kege{namespace xml{

    void content::set( string& data )
    {
        _content = data;
    }
    const string& content::get()const
    {
        return _content;
    }
    string& content::get()
    {
        return _content;
    }

    content::content( const string& tag, Ref< attributes > attribs, string& content )
    :   element( CONTENT, tag, attribs )
    ,   _content( content )
    {}
    content::content( const string& tag, Ref< attributes > attribs )
    :   element( CONTENT, tag, attribs )
    {}
    content::content( const string& tag )
    :   element( CONTENT, tag )
    {}
    content::content()
    :   element( CONTENT )
    {}
}}



namespace kege{namespace xml{

    const element* node::get_by_attrib( const string& name, const string& value )const
    {
        const xml::attribute* a;
        for ( const_iterator i=_elements.begin() ; i != _elements.end() ; i++ )
        {
            a = (*i)->get_attrib( name, value );
            if (a != nullptr) return (*i).ref();
        }
        return nullptr;
    }
    element* node::get_by_attrib( const string& name, const string& value )
    {
        const xml::attribute* a;
        for ( iterator i=_elements.begin() ; i != _elements.end() ; i++ )
        {
            a = (*i)->get_attrib( name, value );
            if (a != nullptr) return (*i).ref();
        }
        return nullptr;
    }

    const element* node::get( const string& tag )const
    {
        if (tag.empty()) return nullptr;

        if (tag[0] == '/')
        {
            string ename;
            string urlname = tag;
            const char* str = strtok(urlname.str(), "/\\\n\t\r");
            const element* root = this;
            while (str != nullptr && root != nullptr)
            {
                ename = str;
                root = ((node*)root)->get( ename );
                str = strtok(nullptr, "/\\\n\t\r");
            }
            if (root != nullptr)
            {
                if (root->get_tag() == ename )
                    return root;
            }
            return nullptr;
        }
        else
        {
            for ( const_iterator i=_elements.begin() ; i!=_elements.end() ; i++)
                if ( (*i)->get_tag() == tag )
                    return (*i).ref();
            return nullptr;
        }
        return nullptr;
    }
    element* node::get( const string& tag )
    {
        if (tag.empty()) return nullptr;

        if (tag[0] == '/')
        {
            string ename;
            string urlname = tag;
            const char* str = strtok(urlname.str(), "/\\\n\t\r");
            element* root = this;
            while (str != nullptr && root != nullptr)
            {
                ename = str;
                root = ((node*)root)->get( ename );
                str = strtok(nullptr, "/\\\n\t\r");
            }
            if (root != nullptr)
            {
                if (root->get_tag() == ename )
                    return root;
            }
            return nullptr;
        }
        else
        {
            for ( iterator i=_elements.begin() ; i!=_elements.end() ; i++)
                if ( (*i)->get_tag() == tag )
                    return (*i).ref();
            return nullptr;
        }
        return nullptr;
    }
    void node::remove( const string& tag )
    {
        if (tag[0] == '/')
        {
            string ename;
            string urlname = tag;
            const char* str = strtok(urlname.str(), "/\\\n\t\r");
            element* root = this;
            while (str != nullptr && root != nullptr)
            {
                ename = str;
                if ( root->get_node_type() == NODE )
                {
                    root = ((node*)root)->get( ename );
                }
                else
                {
                    break;
                }
                str = strtok(nullptr, "/\\\n\t\r");
            }
            if (root != nullptr)
            {
                for ( iterator i=_elements.begin() ; i!=_elements.end() ; i++)
                {
                    if ( (*i).ref() == root )
                    {
                        (*i) = nullptr;
                        _elements.erase(i);
                    }
                }
            }
        }
        else
        {
            for ( iterator i=_elements.begin() ; i!=_elements.end() ; i++)
            {
                if ( (*i)->get_tag() == tag )
                {
                    (*i) = nullptr;
                    _elements.erase(i);
                }
            }
        }
    }
    void node::add( Ref< element > elem )
    {
        if ( elem == nullptr ) return;
        _elements.push_back( elem );
    }

    void node::print(bool all,int n)
    {
        element::print( all, n );

        if ( !_elements.empty() )
        {
            for ( iterator i=_elements.begin() ; i!=_elements.end() ; i++)
            {
                (*i)->print( all, n+2 );
            }
        }
    }

    element::const_iterator node::begin()const
    {
        return _elements.cbegin();
    }
    element::const_iterator node::end()const
    {
        return _elements.cend();
    }

    node::iterator node::begin()
    {
        return _elements.begin();
    }
    node::iterator node::end()
    {
        return _elements.end();
    }
    void node::clear()
    {
        _elements.clear();
    }

    node::node( const string& tag, Ref< attributes > attribs, Ref< element > elem )
    :   element( NODE, tag, attribs )
    {
        add( elem );
    }
    node::node( const string& tag, Ref< attributes > attribs )
    :   element( NODE, tag, attribs )
    {}
    node::node( const string& tag )
    :   element( NODE, tag )
    {}

    node::~node()
    {
        _elements.clear();
    }
    node::node()
    :   element( NODE )
    {}
}}


namespace kege{namespace xml{

    void document::state_none_endtag()
    {
        if (_state == NONE_ENDTAG)
        {
            get_token();

            Ref< xml::element > xml = _stack.top();
            _stack.pop();

            if (!_stack.empty())
            {
                _stack.top()->add( xml.ref() );
            }
            else
            {
                add( xml );
            }
        }
        else
        {
            write_error("xml state_start_element() : expecting character <.");
        }
    }

    void document::state_attribute()
    {
        if ( _state == ATTRIBUTE)
        {
            get_token();
            string name = _token;
            if (_state == ATTRIBUTE_VALUE)
            {
                get_token();
                string value = _token;
                _stack.top()->add_attrib(name, value);
            }
            else
            {
                write_error("xml expecting attribute value for " + name);
            }
        }
        else
        {
            write_error("xml incorrect state destination : ATTRIBUTE /.");
        }
    }

    void document::state_content()
    {
        if ( _state == CONTENT)
        {
            get_content();
            get_token();
            state_endtag();
        }
        else
        {
            write_error("xml state_content() : expecting string");
        }
    }

    void document::state_endtag()
    {
        if ( _state == END_TAG)
        {
            get_token();

            Ref< xml::element > xml = _stack.top();
            _stack.pop();

            if (!_stack.empty())
            {
                if (xml->get_tag() == _token)
                {
                    _stack.top()->add( xml.ref() );
                }
                else
                {
                    write_error("xml end tag miss match -> " + xml->get_tag());
                }
            }
            else
            {
                add( xml );
            }
        }
        else
        {
            write_error("xml state_start_element() : expecting character <.");
        }
    }

    void document::state_newtag()
    {
        if ( _state == NEW_TAG)
        {
            get_token();
            _stack.push(new  xml::node(_token));
        }
        else
        {
            write_error("xml state_start_element() : expecting character <.");
        }
    }

    void document::write_error( const string& erro )
    {
        printf("%3i: %s\n", _line, erro.c_str());
        _state = ERROR;
    }

    bool document::is_letters(int c, State state)
    {
        switch ( state )
        {
            case ATTRIBUTE_VALUE: // attribute value can take any characters except for, "
                return ((c >= 'a' && c <= 'z') ||
                        (c >= 'A' && c <= 'Z') ||
                        (c >= '0' && c <= '9') ||
                        (c >= ' ' && c <= '!') ||
                        (c >= '#' && c <= '/') ||
                        (c >= ':' && c <= '@') ||
                        (c >= '[' && c <= '`') ||
                        (c >= '{' && c <= '~'));

            case CONTENT: // content area can take any characters except for, '>' and '<'
                return ((c >= 'a' && c <= 'z') ||
                        (c >= 'A' && c <= 'Z') ||
                        (c >= '0' && c <= '9') ||
                        (c >= ' ' && c <= '/') ||
                        (c >= ':' && c <= ';') ||
                        (c >= '[' && c <= '`') ||
                        (c >= '{' && c <= '~') ||
                        (c >= '?' && c <= '@') ||
                        (c == '='));;

            default: // only these characters are allowed for element and attribute naming
                return ((c >= 'a' && c <= 'z') ||
                        (c >= 'A' && c <= 'Z') ||
                        (c >= '0' && c <= '9') ||
                        (c == '-') ||
                        (c == '_') ||
                        (c == '#'));
        }
    }

    long document::seek(const char* flags)
    {
        const char* f;
        const char* s = _curr;
        while (*s != 0)
        {
            f = flags;
            while (*f != 0)
            {
                if (*s == *f )
                {
                    long n = s - _curr;
                    _curr += n;
                    return n;
                }
                f++;
            }
            if (*s == '\n')
            {
                _line++;
            }
            s++;
        }
        if (*s == 0)
        {
            return -1;
        }
        return 0;
    }

    long document::skip(const char* flags)
    {
        const char* s = _curr;
        const char* f;

        while (*s != 0)
        {
            f = flags;
            while (*f != 0 && *s != *f) f++;
            if (*f == 0) break;
            if (*s == '\n')
            {
                _line++;
            }
            s++;
        }

        long n = s - _curr;
        _curr = s;
        return n;
    }

    long document::skip_whitespace()
    {
        return skip(" \n\t\r");
    }

    void document::get_content()
    {
        const char* p = _curr;
        long size = seek("<");
        string &target = _stack.top()->get();
        target.copyFrom( size, p );
    }

    void document::get_token()
    {
        if ( _state == ERROR )
            return;

        skip_whitespace();
        _prev = _curr;
        _token.setLength(0);

        int c;
        State curr_state = _state;
        while ( get_char() != 0 && curr_state == _state )
        {
            c = get_next();

            if (is_letters(c, _state))
            {
                _token += c;
            }
            else if (c == '<')
            {
                c = get_char();
                if (c == '/')
                {
                    c = get_next();
                    curr_state = END_TAG;
                }
                else if (c == '?')
                {
                    c = get_next();
                    curr_state = NEW_TAG;
                }
                else if (is_letters(c, _state))
                {
                    curr_state = NEW_TAG;
                }
                else
                {
                    //kege::string sm = kege::string("expecting '<' or '/', unknown character -> ") + c;
                    write_error( kege::string("expecting '<' or '/', unknown character -> ") + c );
                }
            }
            else if (c == '>')
            {
                curr_state = GT;
                _prev = _curr;
                skip_whitespace();
                c = get_char();
                if (is_letters(c, curr_state))
                {
                    _curr = _prev;
                    curr_state = CONTENT;
                }
            }
            else if (c == '/')
            {
                c = get_char();
                if (c == '>')
                {
                    c = get_next();
                    curr_state = NONE_ENDTAG;
                }
            }
            else if (c == ' ')
            {
                skip_whitespace();
                c = get_char();
                if (is_letters(c, _state))
                {
                    curr_state = ATTRIBUTE;
                }
            }
            else if (c == '=')
            {
                c = get_char();
                if (c == '\"')
                {
                    c = get_next();
                    curr_state = ATTRIBUTE_VALUE;
                }
                else
                {
                    write_error(string("expecting '\"' after '=', for attribute '") + _token + '\'');
                }
            }
            else if (c == '\"')
            {
                if(_state == ATTRIBUTE)
                {
                    write_error(string("missing '=' for attribute '") + _token + '\'');
                    break;
                }
                curr_state = END_QUOTE;
                skip_whitespace();
                c = get_char();
                if (is_letters(c, ATTRIBUTE))
                {
                    curr_state = ATTRIBUTE;
                }
            }
            else if (c == '?')
            {
                c = get_next();
                if (c == '>')
                {
                    curr_state = NONE_ENDTAG;
                }
                else
                {
                    write_error(string("expecting '>' after '?', unknown character -> ") + c);
                }
            }
        }
        _state = curr_state;
    }

    int document::get_char()
    {
        if (_curr == nullptr) return '\0';
        return *_curr;
    }
    int document::get_next()
    {
        if (_curr == nullptr) return '\0';
        if (*_curr == '\0') return '\0';
        if (*_curr == '\n') _line++;
        return *_curr++;
    }
    int document::get_prev()
    {
        if (_prev == nullptr) return '\0';
        return *_prev;
    }

    void document::parse()
    {
        _state = START;
        while (get_char() != 0 && _state != ERROR)
        {
            switch (_state)
            {
                case NEW_TAG: state_newtag();
                    break;

                case ATTRIBUTE: state_attribute();
                    break;

                case CONTENT: state_content();
                    break;

                case END_TAG: state_endtag();
                    break;

                case NONE_ENDTAG: state_none_endtag();
                    break;

                default:
                    get_token();
                    if(_state == START)
                        return;
                    break;
            }
        }
    }

    bool document::load(string const& filename)
    {
        FILE* file = fopen( filename.c_str(), "r" );
        if ( file == nullptr )
        {
            return false;
        }

        fseek(file, 0, SEEK_END);
        size_t size = ftell(file);
        rewind(file);

        if (size == 0) return false;
        _data = new char[ size + 1 ];
        fread(_data, size, sizeof(char), file);
        fclose(file);
        _data[ size ] = '\0';
        _curr = _data;

        parse();
        return true;
    }
    document::~document()
    {
        if (_data != nullptr)
        {
            delete [] _data;
            _data = nullptr;
        }
    }
    document::document()
    :   _curr(nullptr)
    ,   _prev(nullptr)
    ,   _data(nullptr)
    ,   _line(1)
    {}
}}
