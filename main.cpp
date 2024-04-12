#include "Game.hpp"
#include "ThreadPool.hpp"
#include <getopt.h>

constexpr static struct option long_options[] = {
        { "help",    no_argument,       nullptr, 'h' },
        { "serial",    required_argument,      nullptr, 's' },
        { "parallel", required_argument, nullptr, 'p'},
        { nullptr,  0,                 nullptr, '\0' }
};

int main(int argc, char*argv[]){

    int longidx, NumSimulations, mode;
    
    try{
        longidx = getopt_long(argc, argv, "hs:p:", long_options, &longidx);
        if(longidx == -1 || !optarg) throw InvalidArgument();
        NumSimulations = (char) longidx == 'h'? 0: std::stoi(optarg),
        mode = (char) longidx == 's'? 0: 1;

        if((char) longidx == 'h'){
            std::cerr << "This is a help message\n";
            exit(0);
        }
        else if((char) longidx != 's' && (char) longidx != 'p') throw InvalidArgument();
        //else if(NumSimulations < 0 || NumSimulations > 200000) throw InvalidArgument();
    }
    catch( const InvalidArgument& err ){
        std::cerr << "Invalid arguements provided\n";
        exit(1);
    }
    
    
    if(mode) std::cout << "parallel";
    else std::cout << "serial";

    std::cout << std::endl << NumSimulations << std::endl;


    return 0;
}