// PROJECT IDENTIFIER: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD
#include "Hunt.hpp"
#include <getopt.h>
#include <iostream>
#include <string>
#include "xcode_redirect.hpp"
#include <stack>
using namespace std;

void Hunt::read_in() {
    string comment;
    getline(cin, comment);
    while(comment[0]=='#'){
        getline(cin, comment);
    }
    string file_type;
    file_type=comment;
    uint32_t a_size;
    cin>>a_size;
    actual_map.map_size=a_size;
    
    string current_line;
    if(file_type=="M"){
        string junk;
        getline(cin, junk);
        datapoint pt;
        pt.p_type='.';
        actual_map.map.resize(actual_map.map_size,
                              vector<datapoint>(actual_map.map_size,pt));
        uint32_t line_n=0;
        
        datapoint temp;
        while(getline(cin,current_line)){
            if(current_line==""){
                continue;
            }
            for(uint32_t i=0;i<actual_map.map_size;++i){
                temp.p_type=current_line[i];
                if(current_line[i]=='@'){
                    temp.p_type='.'; //start must be water
                    actual_map.start.x_coordinate=line_n;
                    actual_map.start.y_coordinate=i;
                    actual_map.map[line_n][i].direction='s';
                }
                actual_map.map[line_n][i] = temp;
            }
            line_n++;
        }
    }
        
    if(file_type=="L"){
        datapoint pt;
        pt.p_type='.';
        actual_map.map.resize(actual_map.map_size,
                              vector<datapoint>(actual_map.map_size,pt));
        uint32_t row;
        uint32_t col;
        char type;
        while(cin>>row>>col>>type){
            pt.p_type=type;
            if(type=='@'){
                pt.p_type='.'; //start must be water
                actual_map.start.x_coordinate=row;
                actual_map.start.y_coordinate=col;
                actual_map.map[row][col].direction='s';
            }
            actual_map.map[row][col] = pt;
        }
    } // if L
}

void Hunt::captain_hunt(){
    deque<location> sail_container;
    sail_container.push_back(actual_map.start);
    if(verbose_mode){
        cout<<"Treasure hunt started at: "<<
        actual_map.start.x_coordinate<<","<<actual_map.start.y_coordinate<<"\n";
    }
    actual_map.map[sail_container.back().x_coordinate][sail_container.back().y_coordinate].direction='s';
    
    location sail_loc; //current sail location
    datapoint newpt; //current explore location
    location newpt_loc;
    while(!sail_container.empty() && !is_find){
        if(c_type=='s'){ //c_container -> STACK
            sail_loc.x_coordinate = sail_container.back().x_coordinate;
            sail_loc.y_coordinate = sail_container.back().y_coordinate;
            sail_container.pop_back();} //remove current loc from c_container
            
        if(c_type=='q'){ //c_container -> QUEUE
            sail_loc.x_coordinate = sail_container.front().x_coordinate;
            sail_loc.y_coordinate = sail_container.front().y_coordinate;
            sail_container.pop_front();
        }
        wat_loc++;
        
        for(uint32_t i=0;i<4;++i){ //check 4 directions
            if(is_find){break;}
            
            uint32_t temp_x;
            uint32_t temp_y;
            if(h_order[i]=='N'){
                temp_x=sail_loc.x_coordinate-1;
                temp_y=sail_loc.y_coordinate;
            } else if(h_order[i]=='E'){
                temp_x=sail_loc.x_coordinate;
                temp_y=sail_loc.y_coordinate+1;
            } else if(h_order[i]=='W'){
                temp_x=sail_loc.x_coordinate;
                temp_y=sail_loc.y_coordinate-1;
            } else{
                temp_x=sail_loc.x_coordinate+1;
                temp_y=sail_loc.y_coordinate;
            }
            if(temp_x>=actual_map.map_size||temp_y>=actual_map.map_size){continue;}
            newpt=actual_map.map[temp_x][temp_y]; //current datapoint
            
            if(newpt.direction!='d'){continue;}
            
            if(newpt.p_type=='.'){
                newpt_loc.x_coordinate=temp_x;
                newpt_loc.y_coordinate=temp_y;
                actual_map.map[temp_x][temp_y].direction=h_order[i];
                sail_container.push_back(newpt_loc);
            } else if(newpt.p_type=='o'||newpt.p_type=='$'){
                newpt_loc.x_coordinate=temp_x;
                newpt_loc.y_coordinate=temp_y;
                actual_map.map[temp_x][temp_y].direction=h_order[i];
                ++went_ashore;
                if(verbose_mode){
                    cout<<"Went ashore at: "<<temp_x<<","<<temp_y<<"\n";
                }
                fm_hunt(newpt_loc);
            } else{ //if impassable: do nothing
                continue;
            }
        }// for
    }//while
    if(!is_find&&verbose_mode){
        cout<<"Treasure hunt failed\n";
    }
}

void Hunt::fm_hunt(location nloc){
    if(actual_map.map[nloc.x_coordinate][nloc.y_coordinate].p_type=='$'){
        actual_map.end.x_coordinate=nloc.x_coordinate;
        actual_map.end.y_coordinate=nloc.y_coordinate;
        is_find=true;
        lan_loc++;
        if(verbose_mode&&is_find){
            cout<<"Searching island... party found treasure at "
            <<actual_map.end.x_coordinate<<","<<actual_map.end.y_coordinate<<".\n";
        }
        return;
    }
    deque <location> search_container;
    search_container.push_back(nloc);
    location search_loc; //current search location
    datapoint nowpt;
    location now_loc;
    while(search_container.size()!=0 && !is_find){
        if(f_type=='s'){ //f_container -> STACK
            search_loc.x_coordinate = search_container.back().x_coordinate;
            search_loc.y_coordinate = search_container.back().y_coordinate;
            search_container.pop_back();} //remove current loc from f_container
            
        if(f_type=='q'){ //f_container -> QUEUE
            search_loc.x_coordinate = search_container.front().x_coordinate;
            search_loc.y_coordinate = search_container.front().y_coordinate;
            search_container.pop_front();
        }
        lan_loc++;
        
        for(uint32_t i=0;i<4;++i){ //check 4 directions
            if(is_find){break;}
            
            uint32_t ftemp_x;
            uint32_t ftemp_y;
            if(h_order[i]=='N'){
                ftemp_x=search_loc.x_coordinate-1;
                ftemp_y=search_loc.y_coordinate;
            } else if(h_order[i]=='E'){
                ftemp_x=search_loc.x_coordinate;
                ftemp_y=search_loc.y_coordinate+1;
            } else if(h_order[i]=='W'){
                ftemp_x=search_loc.x_coordinate;
                ftemp_y=search_loc.y_coordinate-1;
            } else{
                ftemp_x=search_loc.x_coordinate+1;
                ftemp_y=search_loc.y_coordinate;
            }
            if(ftemp_x>=actual_map.map_size||ftemp_y>=actual_map.map_size){
                continue;}
            
            nowpt=actual_map.map[ftemp_x][ftemp_y]; //current datapoint
            
            if(nowpt.direction!='d'){continue;}
            
            if(nowpt.p_type=='.'){ //water: do nothing
                continue;
            } else if(nowpt.p_type=='o'){
                actual_map.map[ftemp_x][ftemp_y].direction=h_order[i];
                now_loc.x_coordinate=ftemp_x;
                now_loc.y_coordinate=ftemp_y;
                search_container.push_back(now_loc);
            } else if(nowpt.p_type=='$'){
                actual_map.end.x_coordinate=ftemp_x;
                actual_map.end.y_coordinate=ftemp_y;
                is_find=true;
                actual_map.map[ftemp_x][ftemp_y].direction=h_order[i];
                lan_loc++;
                break; //exit while loop!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            } else { //if impassable: do nothing
                continue;
            }
        }// for
    }//while
    if(verbose_mode&&!is_find){
        cout<<"Searching island... party returned with no treasure.\n";
    } else if(verbose_mode&&is_find){
        cout<<"Searching island... party found treasure at "
        <<actual_map.end.x_coordinate<<","<<actual_map.end.y_coordinate<<".\n";
    }
}

void Hunt::backtracing(string mode){
    uint32_t trace_x=actual_map.end.x_coordinate;
    uint32_t trace_y=actual_map.end.y_coordinate;
    uint32_t next_x;
    uint32_t next_y;
    if(mode=="L"){
        stack<char> path;
        while(trace_x!=actual_map.start.x_coordinate||trace_y!=actual_map.start.y_coordinate){
            path.push(actual_map.map[trace_x][trace_y].direction);
            if(actual_map.map[trace_x][trace_y].direction=='N'){
                trace_x+=1;
            } else if(actual_map.map[trace_x][trace_y].direction=='E'){
                trace_y-=1;
            } else if(actual_map.map[trace_x][trace_y].direction=='W'){
                trace_y+=1;
            } else{
                trace_x-=1;
            }
            p_length+=1;
        }
        if(stat_mode){
            cout<<"Path length: "<<p_length<<"\n";
            cout<<"Treasure location: "<<actual_map.end.x_coordinate<<","<<actual_map.end.y_coordinate<<"\n";
            cout << "--- STATS ---" << "\n";
        }
        if(print_mode){
            cout<<"Sail:\n";
            cout<<trace_x<<","<<trace_y<<"\n";
            while(!path.empty()){
                if(path.top()=='N'){
                    next_x=trace_x-1;
                    next_y=trace_y;
                } else if(path.top()=='E'){
                    next_x=trace_x;
                    next_y=trace_y+1;
                } else if(path.top()=='W'){
                    next_x=trace_x;
                    next_y=trace_y-1;
                } else{
                    next_x=trace_x+1;
                    next_y=trace_y;
                }
                if((actual_map.map[next_x][next_y].p_type=='o'||actual_map.map[next_x][next_y].p_type=='$')
                   &&actual_map.map[trace_x][trace_y].p_type=='.'){
                    cout<<"Search:\n";
                }
                cout<<next_x<<","<<next_y<<"\n";
                trace_x=next_x;
                trace_y=next_y;
                path.pop();
            }
        }
    } else {
        while(trace_x!=actual_map.start.x_coordinate||trace_y!=actual_map.start.y_coordinate){
            if(actual_map.map[trace_x][trace_y].direction=='N'){
                next_x=trace_x+1;
                next_y=trace_y;
            } else if(actual_map.map[trace_x][trace_y].direction=='E'){
                next_x=trace_x;
                next_y=trace_y-1;
            } else if(actual_map.map[trace_x][trace_y].direction=='W'){
                next_x=trace_x;
                next_y=trace_y+1;
            } else{
                next_x=trace_x-1;
                next_y=trace_y;
            }
            if(print_mode){
                if(trace_x==actual_map.end.x_coordinate&&trace_y==actual_map.end.y_coordinate){
                    actual_map.map[trace_x][trace_y].p_type='X';
                }
                if((actual_map.map[trace_x][trace_y].direction=='N'&&((actual_map.map[next_x][next_y].direction=='W')
                                                ||(actual_map.map[next_x][next_y].direction=='E')))
                       ||(actual_map.map[trace_x][trace_y].direction=='S'&&((actual_map.map[next_x][next_y].direction=='W')||(actual_map.map[next_x][next_y].direction=='E')))
                       ||(actual_map.map[trace_x][trace_y].direction=='E'&&((actual_map.map[next_x][next_y].direction=='N')||(actual_map.map[next_x][next_y].direction=='S')))
                       ||(actual_map.map[trace_x][trace_y].direction=='W'&&((actual_map.map[next_x][next_y].direction=='N')||(actual_map.map[next_x][next_y].direction=='S')))){
                    actual_map.map[next_x][next_y].p_type='+';
                } else if(actual_map.map[trace_x][trace_y].direction=='N'||actual_map.map[trace_x][trace_y].direction=='S'){
                    actual_map.map[next_x][next_y].p_type='|';
                } else if(actual_map.map[trace_x][trace_y].direction=='E'||actual_map.map[trace_x][trace_y].direction=='W'){
                    actual_map.map[next_x][next_y].p_type='-';
                }
            }
            trace_x=next_x;
            trace_y=next_y;
            p_length+=1;
        }
        if(stat_mode){
            cout<<"Path length: "<<p_length<<"\n";
            cout<<"Treasure location: "<<actual_map.end.x_coordinate<<","<<actual_map.end.y_coordinate<<"\n";
            cout << "--- STATS ---" << "\n";
        }
        if(print_mode){
            if(trace_x==actual_map.start.x_coordinate&&trace_y==actual_map.start.y_coordinate){
                actual_map.map[trace_x][trace_y].p_type='@';
            }
            for(uint32_t i=0;i<actual_map.map.size();++i){
                for(uint32_t j=0;j<actual_map.map[i].size();++j){
                    cout<<actual_map.map[i][j].p_type;
                }
                cout<<"\n";
            }
        }
    }
}

void Hunt::getMode(int argc, char * argv[]) {
    string mode;
    opterr = false;
    int choice;
    int option_index = 0;
    option long_options[] = {
        {"help",       no_argument,       nullptr, 'h'},
        {"captain",    required_argument, nullptr, 'c'},
        {"first-mate", required_argument, nullptr, 'f'},
        {"hunt-order", required_argument, nullptr, 'o'},
        {"verbose",    no_argument,       nullptr, 'v'},
        {"stats",      no_argument,       nullptr, 's'},
        {"show-path",  required_argument, nullptr, 'p'},
        { nullptr,     0,         nullptr, '\0' }
    };

    while ((choice = getopt_long(argc, argv, "hc:f:o:vsp:", long_options, &option_index)) != -1) {
        switch (choice) {
        case 'h': // print useful help message and exit, ignores all other options
                cout<<"This is a treasure game, please read EECS281 project1 spec.\n";
            exit(0);

        case 'c': //captain container
            mode = string(optarg);
            if (mode != "QUEUE" && mode != "STACK") {
                cerr << "Invalid argument to --captain" << endl;
                exit(1);
            }
            if(mode=="QUEUE"){c_type='q';}
            if(mode=="STACK"){c_type='s';}
            break;
                
        case 'f': //first-mate container
            mode = string(optarg);
            if (mode != "QUEUE" && mode != "STACK") {
                cerr << "Invalid argument to --first-mate" << endl;
                exit(1);
            }
            if(mode=="QUEUE"){f_type='q';}
            if(mode=="STACK"){f_type='s';}
            if(!f_type){
                f_type='q';
            }
            break;
            
        case 'o': // h_order
            mode = string(optarg);
            if(mode.length()!=4||mode.find("N")==string::npos||mode.find("E")==string::npos||
               mode.find("W")==string::npos||mode.find("S")==string::npos) {
                cerr << "Invalid argument to --hunt-order" << endl;
                exit(1);
            }
            for(uint32_t i=0;i<4;++i){
                h_order[i]=mode[i];
            }
            break;
                
        case 'v':
            verbose_mode=true;
            break;
            
        case 's': //Display statistics after the search is complete
            stat_mode=true;
            break;
            
        case 'p': //show path
            print_mode=true;
            pmode=optarg;
            if((pmode!="M")&&(pmode!="L")){
                cerr << "Invalid argument to --show-path" << endl;
                exit(1);
            } else if(order_defined){
                cerr << "Specify --show-path only once" << endl;
                exit(1);
            }
            order_defined=true;
            break;
                
        default:
            cerr << "Unknown option" << endl;
            exit(1);
        } // switch
    } // while
}


int main(int argc, char *argv[]) {
    xcode_redirect(argc,argv);
    Hunt attempt;
    attempt.getMode(argc, argv);
    attempt.read_in();
    attempt.captain_hunt();
    if(attempt.stat_mode){
        cout << "--- STATS ---" << "\n";
        cout<<"Starting location: "<<attempt.actual_map.start.x_coordinate
        <<","<<attempt.actual_map.start.y_coordinate<<"\n";
        cout<<"Water locations investigated: "<<attempt.wat_loc<<"\n";
        cout<<"Land locations investigated: "<<attempt.lan_loc<<"\n";
        cout<<"Went ashore: "<<attempt.went_ashore<<"\n";
        if(!attempt.is_find){
            cout << "--- STATS ---" << "\n";
        }
    }
    if(attempt.is_find){
        attempt.backtracing(attempt.pmode);
        cout<<"Treasure found at "<<attempt.actual_map.end.x_coordinate<<","
        <<attempt.actual_map.end.y_coordinate<<" with path length "<<attempt.p_length<<".\n";
    } else{
        cout<<"No treasure found after investigating "<<attempt.wat_loc+attempt.lan_loc<<" locations.\n";
    }
    return 0;
}
