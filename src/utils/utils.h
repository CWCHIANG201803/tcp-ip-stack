#ifndef UTILS_H
#define UTILS_H

#define IS_MAC_BROADCAST_ADDR(mac) ( strlen(mac)==6 && strcmp(mac, "\xFF\xFF\xFF\xFF\xFF\xFF")==0 )

void apply_mask(char* prefix, char mask, char* str_prefix);

bool validate_prefix(const char* input);

void layer2_fill_with_broadcast_mac(char* mac_array);
#endif