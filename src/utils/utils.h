#ifndef UTILS_H
#define UTILS_H

void apply_mask(char* prefix, char mask, char* str_prefix);

bool validate_prefix(const char* input);

void layer2_fill_with_broadcast_mac(char* mac_array);
#endif