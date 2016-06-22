#include <string>
#include <regex>
#include <vector>

class JSONPretify : public std::string{

public:
    
    JSONPretify(std::string j){this->assign(j); pretify(); };
private:
    void pretify(){
        std::regex var = std::regex(R"((\".+?\".*?(?=\{|\[|\,|\]|\}))|(\d+?))");
        long it = 0;
        int depth = 0;
        while(it < this->size() && it != -1){
            regex_pos pos_tab = findRegexFirstPosition(it, var);
            long pos_comma = this->find(",", it);
            long pos_obj_start = this->find("{", it);
            long pos_obj_end = this->find("}", it);
            long pos_array_start = this->find("[", it);
            long pos_array_end = this->find("]", it);
            long old_it = it;
            
            unsigned long work_with = find_lowest(std::vector<long>{pos_tab.pos, pos_comma, pos_obj_start, pos_obj_end,pos_array_start,pos_array_end});
            
            switch(work_with){
                case(TAB):{
                    std::string insert = generateSpaces(depth);
                    this->insert(pos_tab.pos, insert);
                    
                    it = pos_tab.pos+insert.size()+pos_tab.length;
                    break;
                }
                case(COMMA):{
                    std::string insert = "\n";
                    this->insert(pos_comma+1, insert);
                    
                    it = pos_comma+1;
                    break;
                }
                case(OBJ_START):{
                    std::string insert = "\n";
                    this->insert(pos_obj_start+1, insert);
                    it = pos_obj_start+insert.size();
                    if(this->at(pos_obj_start-1) != ':'){
                        std::string extra = generateSpaces(depth);
                        this->insert(pos_obj_start, extra);
                        it+=extra.size();
                    }
                    depth+=1;
                    break;
                }
                case(OBJ_END):{
                    std::string insert = "\n"+generateSpaces(depth-1);
                    this->insert(pos_obj_end, insert);
                    depth-=1;
                    it = pos_obj_end+insert.size()+1;
                    break;
                }
                case(ARRAY_START):{
                    depth+=1;
                    std::string insert = "\n";
                    this->insert(pos_array_start+1,insert);
                    it=pos_array_start+insert.size();
                    break;
                }
                case(ARRAY_END):{
                    depth-=1;
                    std::string insert = "\n"+generateSpaces(depth);
                    this->insert(pos_array_end,insert);
                    it=pos_array_end+insert.size()+1;
                    break;
                }
                default:{
                    break;
                }
            };
            
            
            if(it == old_it)
                break;
        }
    };
    
    struct regex_pos{
        long pos;
        long length;
    };
    std::string generateSpaces(int l){
        std::string r="";
        for(int i = 0; i < l; i++){
            r+= "    ";
        }
        return r;
    }
    regex_pos findRegexFirstPosition(long start_pos, std::regex rx){
        long at = -1;
        long l = 0;
        
        std::string ss(this->begin()+start_pos, this->end());
        std::smatch m;
        
        std::regex_search ( ss, m, rx );
        
        for (unsigned i=0; i<m.size(); ++i) {
            at = m.position(i);
            l = m[i].str().size();
            break;
        }
        if(at != -1) at += start_pos;
        return {at,l};
    }
    template<typename T>
    unsigned long find_lowest(std::vector<T> outof){
        unsigned long lowest_it = 0;
        for(unsigned i = 0; i < outof.size(); i++){
            if((outof[i] < outof[lowest_it] && outof[i] != -1) || (outof[lowest_it] == -1 && outof[i] != -1)){
                lowest_it = i;
            }
        }
        if(outof[lowest_it] == -1)
            lowest_it = outof.size()+1;
        return lowest_it;
    }
    
    enum positions{
        TAB = 0,
        COMMA = 1,
        OBJ_START = 2,
        OBJ_END = 3,
        ARRAY_START = 4,
        ARRAY_END = 5
    };
};