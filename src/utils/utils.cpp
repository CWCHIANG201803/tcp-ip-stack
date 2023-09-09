#include "utils.h"
#include <cstdlib>
#include <vector>
#include <bitset>
#include <sstream>
#include <iostream>

void apply_mask(char* prefix, char mask, char* str_prefix){
    int num_of_mask = int(mask);

    if(!prefix || strlen(prefix)==0)
        return;

    std::vector<std::bitset<8>> prefix_bitsets;
    std::istringstream iss(prefix);
    std::string val;
    while(std::getline(iss, val, '.'))
        prefix_bitsets.push_back(std::bitset<8>(stoi(val)));

    for(int i=0; i < prefix_bitsets.size(); ++i){
        for(int j=0; j < prefix_bitsets[i].size(); ++j){
            int index = 8*i + j;
            prefix_bitsets[i][j] = index < num_of_mask ? prefix_bitsets[i][j] : 0;
        }
    }

    std::stringstream out;
    for(int i=0; i < prefix_bitsets.size(); ++i){
        out << std::string(i==0 ? "" : ".") << (int)(prefix_bitsets[i].to_ulong());
    }

    strcpy(str_prefix, out.str().c_str());
}