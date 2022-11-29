#include "graph/graph.h"
#include "net.h"

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

    printf("Topology Name = %s\n", graph->topology_name);

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

    printf("\nNode Name = %s\n", node->node_name);
    if (node->node_nw_prop.is_lb_addr_config)
    {
        printf("\t  lo addr : %s/32", NODE_LO_ADDR(node));
    }
    printf("\n");
}

void dump_intf_props(interface_t *interface)
{

    dump_interface(interface);

    if (interface->intf_nw_props.is_ipadd_config)
    {
        printf("\t IP Addr = %s/%u", IF_IP(interface), interface->intf_nw_props.mask);
        printf("\t MAC : %u:%u:%u:%u:%u:%u\n",
                IF_MAC(interface)[0], IF_MAC(interface)[1],
                IF_MAC(interface)[2], IF_MAC(interface)[3],
                IF_MAC(interface)[4], IF_MAC(interface)[5]);
    }
}