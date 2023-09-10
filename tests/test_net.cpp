#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "graph/graph.h"
#include "net/net.h"
#include <stdexcept>

TEST(TestNet, GiveNodeAndSubnetReturnInterface){
    // node_get_matching_subnet_interface
    node_t* node = new node_t();

    node->intf[0] = new interface_t();
    strncpy(node->intf[0]->if_name, "eth0/4", strlen("eth0/4"));
    node->intf[0]->if_name[strlen("eth0/4")]='\0';
    node_set_intf_address(node, node->intf[0]->if_name, "40.1.1.1",24);

    node->intf[1] = new interface_t();
    strncpy(node->intf[1]->if_name, "eth0/0", strlen("eth0/0"));
    node->intf[1]->if_name[strlen("eth0/0")]='\0';
    node_set_intf_address(node, node->intf[1]->if_name, "20.1.1.1",16);

    char* ip_addr = "20.1.20.131";
    interface_t* return_intf = node_get_matching_subnet_interface(node, ip_addr);
    ASSERT_EQ(return_intf, node->intf[1]);
}

TEST(TestNet, GiveInvalidInputArgThrowInvalidArgException){
    // node_get_matching_subnet_interface
    ASSERT_THROW(node_get_matching_subnet_interface(nullptr, "40.1.1.131"), std::invalid_argument);

    node_t* node = new node_t();
    ASSERT_THROW(node_get_matching_subnet_interface(node, nullptr), std::invalid_argument);
}

TEST(TestNet, GiveNodeWithoutInterfaceReturnNullptr){
    node_t* node = new node_t();
    char* ip_addr = "20.1.20.131";
    ASSERT_EQ(node_get_matching_subnet_interface(node, ip_addr), nullptr);
}

TEST(TestNet, TestOnInfoDump){
    char topology_name[]="test_graph";
    graph_t* graph = create_new_graph(topology_name);

    char name_node1[] = "R1";
    node_t* node1 = create_graph_node(graph, name_node1);
    node1->node_nw_prop.is_lb_addr_config = true;
    strcpy(node1->node_nw_prop.lb_addr.ip_addr, "122.1.1.1");
    node1->intf[0] = create_node_interface(node1, "eth0/0", "10.1.1.1", 24);    
    node1->intf[1] = create_node_interface(node1, "eth0/7", "40.1.1.2", 24);

    char name_node2[] = "R2";
    node_t* node2 = create_graph_node(graph, name_node2);
    node2->node_nw_prop.is_lb_addr_config = true;
    strcpy(node2->node_nw_prop.lb_addr.ip_addr, "122.1.1.2");
    node2->intf[0] = create_node_interface(node2, "eth0/1", "10.1.1.2",24);
    node2->intf[1] = create_node_interface(node2, "eth0/2", "20.1.1.1",24);

    char name_node3[] = "R3";
    node_t* node3 = create_graph_node(graph, name_node3);
    node3->node_nw_prop.is_lb_addr_config = true;
    strcpy(node3->node_nw_prop.lb_addr.ip_addr, "122.1.1.3");

    node3->intf[0] = create_node_interface(node3, "eth0/3", "20.1.1.2",24);
    node3->intf[1] = create_node_interface(node3, "eth0/4", "30.1.1.1",24);
    
    char name_node4[] = "R4";
    node_t* node4 = create_graph_node(graph, name_node4);
    node4->node_nw_prop.is_lb_addr_config = true;
    strcpy(node4->node_nw_prop.lb_addr.ip_addr, "122.1.1.4");

    node4->intf[0] = create_node_interface(node4, "eth0/5", "30.1.1.2",24);
    node4->intf[1] = create_node_interface(node4, "eth0/6", "40.1.1.2",24);

    link_t lnk70(*node4->intf[1], *node1->intf[0]);
    node4->intf[1]->link = &lnk70;
    node1->intf[0]->link = &lnk70;

    link_t lnk12(*node1->intf[1], *node2->intf[0]);
    node1->intf[1]->link = &lnk12;
    node2->intf[0]->link = &lnk12;

    link_t lnk34(*node2->intf[1], *node3->intf[0]);
    node2->intf[1]->link = &lnk34;
    node3->intf[0]->link = &lnk34;

    link_t lnk56(*node3->intf[1], *node4->intf[0]);
    node3->intf[1]->link = &lnk56;
    node4->intf[0]->link = &lnk56;

    dump_new_nw_graph(graph);

}