#ifndef NET_H
#define NET_H

#include <stdlib.h>
#include <string.h>
#include "assert.h"


// forward declaration
typedef struct graph_ graph_t;
typedef struct node_ node_t;
typedef struct interface_ interface_t;

typedef struct ip_add_ {
    ip_add_(){
        strcpy(ip_addr, "");
    }
    char ip_addr[16];
} ip_add_t;

typedef struct mac_add_ {
    mac_add_(){
        strcpy(mac, "");
    }
    char mac[8];
} mac_add_t;

typedef struct node_nw_prop_ {
    node_nw_prop_(){
        is_lb_addr_config = false;
        lb_addr = ip_add_t();
    }
    bool is_lb_addr_config;
    ip_add_t lb_addr;
} node_nw_prop_t;

static inline void init_node_nw_prop(node_nw_prop_t* node_nw_prop) {
    node_nw_prop->is_lb_addr_config = false;
    memset(node_nw_prop->lb_addr.ip_addr, 0, 16);
}

typedef struct intf_nw_props_ {
    intf_nw_props_() {
        strcpy(mac_add.mac, "");
        is_ipadd_config = false;
        strcpy(ip_add.ip_addr, "");
        mask = 0x0;
    }
    mac_add_t mac_add;
    bool is_ipadd_config;
    ip_add_t ip_add;
    char mask;
} intf_nw_props_t;

static inline void init_intf_nw_prop(intf_nw_props_t* intf_nw_props){
    memset(intf_nw_props->mac_add.mac, 0, 48);
    intf_nw_props->is_ipadd_config = false;
    memset(intf_nw_props->ip_add.ip_addr, 0, 16);
    intf_nw_props->mask = 0;
}

#define IF_MAC(intf_ptr)    ((intf_ptr)->intf_nw_props.mac_add.mac)
#define IF_IP(intf_ptr)    ((intf_ptr)->intf_nw_props.ip_add.ip_addr)

#define NODE_LO_ADDR(node_ptr)    (node_ptr->node_nw_prop.lb_addr.ip_addr)

bool node_set_loopback_address(node_t* node, char *ip_addr);
bool node_set_intf_address(node_t* node, char* local_if, char* ip_addr, char mask);
bool node_unset_intf_ip_address(node_t* node, char* local_if);

void dump_nw_graph(graph_t* graph);
void dump_node_nw_props(node_t *node);
void dump_intf_props(interface_t *interface);

interface_t* node_get_matching_subnet_interface(node_t* node, char* ip_addr);

#endif