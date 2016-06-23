#ifndef BORISAS_JSON_PRETIFY_H
#define BORISAS_JSON_PRETIFY_H

#include <string>
#include <regex>
#include <vector>
#include <algorithm>

namespace boris {
    
    namespace JSONPrettify_specifics{
        enum class Colons{
            TIGHT,
            SPACED
        };
        enum class Position{
            TAB = 0,
            COMMA = 1,
            OBJ_START = 2,
            OBJ_END = 3,
            ARRAY_START = 4,
            ARRAY_END = 5
        };
        struct RegexPos{
            size_t pos;
            long length;
        };
            
        std::string generateSpaces( int l ){
            return std::string(l*4, ' ');
        }
        long lowestOf( std::vector<size_t> of ){
            std::vector<size_t>::iterator result = std::min_element(std::begin(of), std::end(of));
            return std::distance(std::begin(of), result);
        }
        
        void insertColonSpaces( std::string & j ){
            std::regex colon = std::regex(R"(\s*?\:\s*?(?=\S))");
            j.assign(std::regex_replace(j, colon, " : "));
        }
        RegexPos findRegexFirstPosition( const std::string & json, const long start_pos, const std::regex rx ){
            size_t at = -1;
            long l = 0;
            
            std::string ss( json.begin()+start_pos, json.end() );
            std::smatch m;
            
            std::regex_search( ss, m, rx );
            
            if ( m.size() > 0 ) {
                at = m.position(0);
                l = m[0].str().size();
            }
            if( at < json.size() ) at += start_pos;
            return {at,l};
        }
    }
    
    std::string JSONPrettify( const std::string & json, boris::JSONPrettify_specifics::Colons spacing=boris::JSONPrettify_specifics::Colons::TIGHT ){
        
        using namespace boris::JSONPrettify_specifics;
        
        std::string pretty      = json;
        const std::regex var    = std::regex(R"((\".+?\".*?(?=\{|\[|\,|\]|\}))|(\d+?))");
        
        long it                 = 0;
        int depth               = 0;
        
        while( it < pretty.size() ){
            
            RegexPos pos_tab        = findRegexFirstPosition( pretty,it, var );
            auto pos_comma          = pretty.find( ",", it );
            auto pos_obj_start      = pretty.find( "{", it );
            auto pos_obj_end        = pretty.find( "}", it );
            auto pos_array_start    = pretty.find( "[", it );
            auto pos_array_end      = pretty.find( "]", it );
            
            long old_it             = it;
            
            Position work_with;
            
            {
                std::vector<size_t> _temp = { pos_tab.pos, pos_comma, pos_obj_start, pos_obj_end,pos_array_start,pos_array_end };
                
                auto at = lowestOf(_temp);
                
            
                if(_temp[at] > pretty.size())   break;
                
                
                work_with = static_cast<Position>(at);
            }
            
            switch( work_with ){
                    
                case(Position::TAB):{
                    std::string insert = generateSpaces(depth);
                    
                    pretty.insert( pos_tab.pos, insert );
                    
                    it = pos_tab.pos+insert.size()+pos_tab.length;
                    break;
                }
                    
                case(Position::COMMA):{
                    std::string insert = "\n";
                    
                    pretty.insert( pos_comma+1, insert );
                    
                    it = pos_comma+1;
                    break;
                }
                    
                case(Position::OBJ_START):{
                    std::string insert = "\n";
                    pretty.insert( pos_obj_start+1, insert );
                    
                    it = pos_obj_start+insert.size();
                    
                    depth+=1;
                    
                    if(pos_obj_start-1 > pretty.size()) continue;
                    
                    if(pretty.at(pos_obj_start-1) != ':'){
                        std::string extra = generateSpaces( depth-1 );
                        pretty.insert( pos_obj_start, extra );
                        it+=extra.size();
                    }
                    
                    break;
                }
                    
                case(Position::OBJ_END):{
                    std::string insert = "\n"+generateSpaces( depth-1 );
                    pretty.insert( pos_obj_end, insert );
                    
                    it = pos_obj_end+insert.size()+1;
                    
                    depth-=1;
                    break;
                }
                    
                case(Position::ARRAY_START):{
                    std::string insert = "\n";
                    pretty.insert( pos_array_start+1,insert );
                    it=pos_array_start+insert.size();
                    
                    depth+=1;
                    break;
                }
                    
                case(Position::ARRAY_END):{
                    std::string insert = "\n"+generateSpaces( depth-1 );
                    pretty.insert( pos_array_end,insert );
                    it=pos_array_end+insert.size()+1;
                    
                    depth-=1;
                    break;
                }
                    
                default:{
                    break;
                }
            };
            
            if(it == old_it)
                break;
        }
        
        if(spacing == Colons::TIGHT)
            insertColonSpaces(pretty);
        
        return pretty;
    }
};

#endif