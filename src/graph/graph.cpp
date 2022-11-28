#include "graph.h"
#include <stdio.h>


graph_t* create_new_graph(char* topology_name){

    if(!topology_name|| strlen(topology_name)<=0){
        return NULL;
    }

    graph_t* graph = (graph_t*)malloc(sizeof(graph_t));
    strncpy(graph->topology_name, topology_name, 32);
    graph->topology_name[32] = '\0';

    init_glthread(&graph->node_list);

    return graph;
}

node_t* create_graph_node(graph_t* graph,char* node_name){
    if(!graph)
        return NULL;

    if(!node_name || strlen(node_name)==0)
        return NULL;

    node_t* node = (node_t*)malloc(sizeof(node_t));
    strncpy(node->node_name, node_name, NODE_NAME_SIZE);
    node->node_name[NODE_NAME_SIZE] = '\0';

    init_glthread(&node->graph_glue);

    glthread_add_next(&graph->node_list, &node->graph_glue);

    return node;
}

void insert_link_between_two_nodes(node_t* node1, node_t* node2,char* from_if_name, char* to_if_name, unsigned int cost){
    link_t* lnk = (link_t*)malloc(sizeof(link_t));

    strncpy(lnk->intf1.if_name, from_if_name, IF_NAME_SIZE);
    lnk->intf1.if_name[IF_NAME_SIZE] = '\0';

    strncpy(lnk->intf2.if_name, to_if_name, IF_NAME_SIZE);
    lnk->intf2.if_name[IF_NAME_SIZE] = '\0';

    lnk->intf1.link = lnk;
    lnk->intf2.link = lnk;
    lnk->cost = cost;

    int empty_intf_slot;

    empty_intf_slot = get_node_intf_available_slot(node1);
    node1->intf[empty_intf_slot] = &lnk->intf1;

    empty_intf_slot = get_node_intf_available_slot(node2);
    node2->intf[empty_intf_slot] = &lnk->intf2;
}

