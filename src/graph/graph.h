#ifndef GRAPH_H
#define GRAPH_H

#include "../glthreads/glthread.h"
#include "stddef.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "../net/net.h"

#define NODE_NAME_SIZE      16
#define IF_NAME_SIZE        16
#define MAX_INTF_PER_NODE   10


typedef struct node_ node_t;
typedef struct link_ link_t;


typedef struct interface_ { // included in a node. a node may have many interfaces
    char if_name[IF_NAME_SIZE]; // interface name
    struct node_ *att_node; // attached node
    struct link_ *link; // link with other interfaces
    intf_nw_props_t intf_nw_props;
} interface_t;

struct link_ {
    interface_t intf1;
    interface_t intf2;
    unsigned int cost;
};

struct node_ {
    char node_name[NODE_NAME_SIZE];
    interface_t *intf[MAX_INTF_PER_NODE];
    glthread_t graph_glue;
    node_nw_prop_t node_nw_prop;
};

struct graph_ {
    char topology_name[32];
    glthread_t node_list;
};



graph_t* create_new_graph(char* topology_name);
node_t* create_graph_node(graph_t* graph, char* node_name);
void insert_link_between_two_nodes(node_t* node1, node_t* node2, char* from_if_name,char* to_if_name, unsigned int cost);


// The function must return the pointer to the nbr node which is connected to the interface passes as an argument.
static inline node_t* get_nbr_node(interface_t* interface){

    if(!interface || !interface->att_node)
        return NULL;

    if(!interface->link || !interface->link->intf2.att_node)
        return NULL;

    struct link_* lnk = interface->link;
    if(interface->att_node != lnk->intf1.att_node)
        return lnk->intf1.att_node;
    
    if(interface->att_node != lnk->intf2.att_node)
        return lnk->intf2.att_node;

    return NULL;
}

// write a below function which returns an empty available slot (the index of the array) 
// into which interface address could be stored
static inline int get_node_intf_available_slot(node_t *node){

    if(!node)
        return -1;

    for(int i =0 ; i < MAX_INTF_PER_NODE; ++i){
        if(!node->intf[i])
            return i;
    }

    return -1;
}

// Write a function in graph.h file which returns pointer to the local interface of a node, searched searched by if_name.
static inline interface_t* get_node_if_by_name(node_t *node, char *if_name){

    for(int idx=0; idx < MAX_INTF_PER_NODE; ++idx){
        if(strcmp(node->intf[idx]->if_name, if_name)==0)
            return node->intf[idx];
    }
    return NULL;
}

// Write a function in graph.h file which returns pointer node present in a graph list, searched by node name.
static inline node_t* get_node_by_node_name(graph_t *topo, char *node_name){

    glthread_t* cur = topo->node_list.right;
    int offset = offsetof(node_t, graph_glue);  // use offsetof provided by gnu-gcc
    while(cur!=NULL){
        node_t* ret_node = (node_t*)((char*)(cur)-offset);
        if(strcmp(ret_node->node_name, node_name)==0)
            return ret_node;
    }

    return NULL;
}

#endif