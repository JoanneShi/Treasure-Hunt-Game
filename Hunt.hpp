// PROJECT IDENTIFIER: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD

#ifndef Hunt_hpp
#define Hunt_hpp

#include <string>
#include <vector>
#include <stdio.h>
#include <deque>
#include <map>
#include <queue>

struct location{
    std::uint32_t x_coordinate=0;
    std::uint32_t y_coordinate=0;
};

struct datapoint{
    char p_type='p'; //terrain o, water ., treasure $
    char direction='d';
};

struct Map{
    std::vector<std::vector<datapoint>> map;
    std::uint32_t map_size;
    location start;
    location end;
};

class Hunt{
    public:
        Hunt(){};
        void captain_hunt();
        void fm_hunt(location nloc);
        void getMode(int argc, char * argv[]);
        void read_in();
        void backtracing(std::string mode);
        bool stat_mode=false;
        bool verbose_mode=false;
        bool print_mode=false;
        std::string pmode;
        bool is_find=false;
        std::uint32_t lan_loc=0;
        std::uint32_t wat_loc=0;
        std::uint32_t p_length=0;
        Map actual_map;
        std::uint32_t went_ashore=0;
    
    private:
        bool order_defined=false;
        char h_order[4]={'N','E','S','W'};
        char c_type = 's';
        char f_type = 'q';
};

#endif /* Hunt_hpp */
