#include "net.h"
#include "graph/graph.h"
#include "utils/utils.h"
#include <stdexcept>
#include <iostream>

/*Just some Random number generator*/
// ref: https://github.com/sachinites/tcpip_stack/blob/master/net.c
static unsigned int hash_code(void *ptr, unsigned int size)
{
    unsigned int value = 0, i = 0;
    char *str = (char *)ptr;
    while (i < size)
    {
        value += *str;
        value *= 97;
        str++;
        i++;
    }
    return value;
}

/*Heuristics, Assign a unique mac address to interface*/
// ref: https://github.com/sachinites/tcpip_stack/blob/master/net.c
void interface_assign_mac_address(interface_t *interface)
{
    node_t *node = interface->att_node;

    if (!node)
        return;

    unsigned int hash_code_val = 0;
    hash_code_val = hash_code(node->node_name, NODE_NAME_SIZE);
    hash_code_val *= hash_code(interface->if_name, IF_NAME_SIZE);
    memset(IF_MAC(interface), 0, sizeof(IF_MAC(interface)));
    memcpy(IF_MAC(interface), (char *)&hash_code_val, sizeof(unsigned int));
}

bool node_set_loopback_address(node_t *node, char *ip_addr)
{

    assert(ip_addr);
    assert(node);

    node->node_nw_prop.is_lb_addr_config = true;
    strncpy(node->node_nw_prop.lb_addr.ip_addr, ip_addr, 16);
    NODE_LO_ADDR(node)
    [15] = '\0';

    return true;
}

bool node_set_intf_address(node_t *node, char *local_if, char *ip_addr, char mask)
{
    if (!node || strlen(local_if) == 0 || strlen(ip_addr) == 0)
        return false;

    interface_t *interface = get_node_if_by_name(node, local_if);

    if (!interface)
        assert(0);

    strncpy(IF_IP(interface), ip_addr, 16);
    IF_IP(interface)
    [15] = '\0';
    interface->intf_nw_props.mask = mask;
    interface->intf_nw_props.is_ipadd_config = true;
    return true;
}

bool node_unset_intf_ip_address(node_t *node, char *local_if)
{

    return false;
}

// display the entire network topology with networking properties
void dump_nw_graph(graph_t *graph)
{

    node_t *node;
    glthread_t *curr;
    interface_t *interface;
    unsigned int i;

    std::cout << "Topology Name = " << graph->topology_name << std::endl;

    ITERATE_GLTHREAD_BEGIN(&graph->node_list, curr)
    {

        node = graph_glue_to_node(curr);
        dump_node_nw_props(node);
        for (i = 0; i < MAX_INTF_PER_NODE; i++)
        {
            interface = node->intf[i];
            if (!interface)
                break;
            dump_intf_props(interface);
        }
    }
    ITERATE_GLTHREAD_END(&graph->node_list, curr);
}

void dump_node_nw_props(node_t *node)
{
    std::cout << "\nNode Name = " << node->node_name << std::endl;
    if (node->node_nw_prop.is_lb_addr_config)
    {
        std::cout << "\t  lo addr : " << NODE_LO_ADDR(node) << "/32" << std::endl;
    }
    std::cout << std::endl;
}

void dump_intf_props(interface_t *interface)
{

    dump_interface(interface);

    if (interface->intf_nw_props.is_ipadd_config)
    {
        std::cout << "\t IP Addr = " << IF_IP(interface) << "/" << interface->intf_nw_props.mask << std::endl;
        std::cout <<"\t MAC : ";
        for(int i=0; i < 6; ++i)
            std::cout <<  std::to_string(int(IF_MAC(interface)[i])) << std::string(i < 5  ? ":" : "");
        std::cout << std::endl;
    }
}

interface_t* node_get_matching_subnet_interface(node_t* node, char* ip_addr){

    if(!node){
        throw std::invalid_argument("invalid input node");
        return nullptr;
    }

    if(!ip_addr || !validate_prefix(ip_addr)){
        throw std::invalid_argument("illegal input ip_addr");
        return nullptr;
    }

    int n = sizeof(node->intf)/sizeof(interface_t*);
    for(int i=0; i < n; ++i){
        if(!node->intf[i]) break;
        interface_t* cur_intf = node->intf[i];
        char* intf_addr = cur_intf->intf_nw_props.ip_add.ip_addr;
        char* str_prefix_intf = (char*)malloc(sizeof(intf_addr));
        char mask = cur_intf->intf_nw_props.mask;
        apply_mask(intf_addr, mask, str_prefix_intf);

        char* str_prefix_input_ip = (char*)malloc(sizeof(ip_addr));
        apply_mask(ip_addr, mask, str_prefix_input_ip);
        if(strcmp(str_prefix_intf, str_prefix_input_ip)==0)
            return node->intf[i];
    }
    return nullptr;
}

void dump_new_nw_graph(graph_t* graph){
    node_t *node;
    glthread_t *curr;
    interface_t* cur_intf;

    std::cout <<" Topology Name = " << graph->topology_name << std::endl;

    ITERATE_GLTHREAD_BEGIN(&graph->node_list, curr)
    {
        node = graph_glue_to_node(curr);
        dump_node_nw_props(node);
        for(int i=0; i < MAX_INTF_PER_NODE; ++i){
            cur_intf = node->intf[i];
            if(!cur_intf) break;
            dump_intf_props(cur_intf);
        }
    }
    ITERATE_GLTHREAD_END(&graph->node_list, curr);
}