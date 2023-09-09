#include "utils.h"
#include <cstdlib>
#include <vector>
#include <bitset>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <regex>
#include <cassert>

void apply_mask(char* prefix, char mask, char* str_prefix){
    int num_of_mask = int(mask);

    if(!prefix || strlen(prefix)==0 || !validate_prefix(prefix)){
        throw std::invalid_argument("invalid input prefix");
        return;
    }

    if(!str_prefix){
        throw std::invalid_argument("invalid str_prefix");
        return;
    }

    if(int(mask) < 0 || int(mask) > 32){
        throw std::out_of_range("mask is out of range");
        return;
    }

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
    assert(strlen(str_prefix) > 0 && validate_prefix(str_prefix));
}

bool validate_prefix(const char* input){
    std::regex pattern("(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])");
    return std::regex_match(input, pattern);
}


void layer2_fill_with_broadcast_mac(char* mac_array){
    const int len = 6;
    if(!mac_array || strlen(mac_array) != len){
        throw std::invalid_argument("invalid input prefix");
        return;
    }

    for(int i = 0; i < len; ++i)
        mac_array[i] = 0xFF;
}