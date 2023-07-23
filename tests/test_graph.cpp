#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "glthreads/glthread.h"
#include "graph/graph.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <memory.h>
#include <stddef.h>


TEST(TestGraph, TestCorrectConfigReturnNeighborNodeName){
    interface_t* intf_n0_0 = (interface_t*)malloc(sizeof(interface_t));
    intf_n0_0->att_node = (node_t*)malloc(sizeof(node_t));
    strcpy(intf_n0_0->att_node->node_name, "node0");
    strcpy(intf_n0_0->if_name, "interface0_n0");
    intf_n0_0->att_node->intf[0] = intf_n0_0;

    interface_t* intf_n1_0 = (interface_t*)malloc(sizeof(interface_t));
    intf_n1_0->att_node = (node_t*)malloc(sizeof(node_t));

    strcpy(intf_n1_0->att_node->node_name, "node1");
    strcpy(intf_n1_0->if_name, "interface0_n1");
    intf_n1_0->att_node->intf[0] = intf_n1_0;

    link_t lnk = { *intf_n0_0, *intf_n1_0 };
    intf_n0_0->link = &lnk;
    intf_n1_0->link = &lnk;

    node_t* rtn_node = get_nbr_node(intf_n0_0);

    EXPECT_STREQ(rtn_node->node_name, "node1");
}

TEST(TestGraph, TestNULLInterfaceReturnNULL){
    interface_t* intf_n_0 = NULL;
    node_t* rtn_node = get_nbr_node(intf_n_0);
    EXPECT_EQ(rtn_node, nullptr);
}

TEST(TestGraph, TestGiveInterfaceNoLinkReturnNULL){
    interface_t* intf_n_0 = (interface_t*)malloc(sizeof(interface_t));
    intf_n_0->link = NULL;

    node_t* rtn_node = get_nbr_node(intf_n_0);

    EXPECT_EQ(rtn_node, nullptr);
}

TEST(TestGraph, TestGiveInterfaceNoAttachNodeReturnNULL){
    interface_t* intf_n_0 = (interface_t*)malloc(sizeof(interface_t));
    intf_n_0->att_node = NULL;

    node_t* rtn_node = get_nbr_node(intf_n_0);

    EXPECT_EQ(rtn_node, nullptr);
}

TEST(TestGraph, TestGiveInterfaceLinkNoNeighborNodeReturnNULL){

    interface_t* intf_n0_0 = (interface_t*)malloc(sizeof(interface_t));
    intf_n0_0->att_node = (node_t*)malloc(sizeof(node_t));

    interface_t* intf_n1_0 = (interface_t*)malloc(sizeof(interface_t));
    intf_n1_0->att_node = NULL;
    
    link_t lnk = { *intf_n0_0, *intf_n1_0 };
    intf_n0_0->link = &lnk;

    node_t* rtn_node = get_nbr_node(intf_n0_0);

    EXPECT_EQ(rtn_node, nullptr);
}

TEST(TestGraph, TestGiveNULLNodeReturnIdxNegative){
    node_t *node = NULL;
    int idx = get_node_intf_available_slot(node);
    EXPECT_EQ(idx, -1);
}

TEST(TestGraph, TestGiveNodeAllNULLInterfaceReturnIdxZero){
    node_t *node = (node_t*)malloc(sizeof(node_t));
    for(int i=0; i < MAX_INTF_PER_NODE; ++i)
        node->intf[i] = NULL;

    int idx = get_node_intf_available_slot(node);

    EXPECT_EQ(idx, 0);
}

TEST(TestGraph, TestGiveNodeInterfaceReturnIdxNegative){
    node_t *node = (node_t*)malloc(sizeof(node_t));
    for(int i=0; i < MAX_INTF_PER_NODE; ++i)
        node->intf[i] = (interface_t*)malloc(sizeof(interface_t));
    
    int idx = get_node_intf_available_slot(node);

    EXPECT_EQ(idx, -1);
}

TEST(TestGraph, TestCreateGraphEmptyStringReturnNull){
    char name[] = "";
    graph_t* graph = create_new_graph(name);
    EXPECT_FALSE(graph!=NULL);
}

TEST(TestGraph, TestCreateGraphNonEmptyNameReturnObject){
    char name[]="test_graph";
    graph_t* graph = create_new_graph(name);
    EXPECT_TRUE(graph!=NULL);

    EXPECT_EQ(strlen(graph->topology_name), strlen(name));
    EXPECT_STREQ(graph->topology_name, name);
    
    EXPECT_FALSE(graph->node_list.left!=NULL);
    EXPECT_FALSE(graph->node_list.right!=NULL);
}

TEST(TestGraph, TestCreateGraphNodeNonEmptyNameReturnObject){

    char topology_name[]="test_graph";
    graph_t* graph = create_new_graph(topology_name);

    char node_name[] = "test_node";
    node_t* node = create_graph_node(graph, node_name);

    int offset = offsetof(node_t, graph_glue);
    node_t* ret_node = (node_t*)((char*)(graph->node_list.right)-offset);

    EXPECT_EQ(ret_node->node_name, node->node_name);
    EXPECT_FALSE(graph->node_list.left!=NULL);
}

TEST(TestGraph, TestCreateGraphNodeNullGraphReturnNull){

    graph_t* graph = NULL;

    char node_name[] = "test_node";
    node_t* node = create_graph_node(graph, node_name);

    EXPECT_FALSE(node!=NULL);
}

TEST(TestGraph, TestCreateGraphEmptyNodeNameReturnNull){

    char topology_name[]="test_graph";
    graph_t* graph = create_new_graph(topology_name);

    char node_name[] = "";
    node_t* node = create_graph_node(graph, node_name);

    EXPECT_FALSE(node!=NULL);
}

TEST(TestGraph, TestCreateNodeLinkReturnNormalObject){
    char topology_name[]="test_graph";
    graph_t* graph = create_new_graph(topology_name);

    char name_node1[] = "node1";
    node_t* node1 = create_graph_node(graph, name_node1);
    
    char name_node2[] = "node2";
    node_t* node2 = create_graph_node(graph, name_node2);

    char intf_name_from[] = "eth0/0";
    char intf_name_to[] = "eth0/1";
    insert_link_between_two_nodes(node1, node2, intf_name_from, intf_name_to, 1);

    EXPECT_TRUE(node1->intf[0]!=NULL);
    EXPECT_TRUE(node2->intf[0]!=NULL);

}

TEST(TestGraph, TestGiveNodeWithTwoIntfWithNameGetTargetNameIntf){
    node_t* node = (node_t*)(malloc(sizeof(node_t)));
    node->intf[0] = (interface_t*)(malloc(sizeof(interface_t)));
    strncpy(node->intf[0]->if_name, "eth0", strlen("eth0"));
    node->intf[0]->if_name[strlen("eth0")]='\0';

    node->intf[1] = (interface_t*)(malloc(sizeof(interface_t)));
    strncpy(node->intf[1]->if_name, "eth1", strlen("eth1"));
    node->intf[1]->if_name[strlen("eth1")]='\0';

    char target_intf_name[] = "eth1";   // contains '\0' implicitly
    interface_t* ret_intf = get_node_if_by_name(node, target_intf_name);

    EXPECT_STREQ(ret_intf->if_name, "eth1");
}

TEST(TestGraph, TestGiveGraphAndNodeNameReturnNodeObject){
    char topology_name[]="test_graph";
    graph_t* graph = create_new_graph(topology_name);

    char name_node1[] = "node1";
    node_t* node1 = create_graph_node(graph, name_node1);
    
    char name_node2[] = "node2";
    node_t* node2 = create_graph_node(graph, name_node2);

    char target_node_name[] = "node2";
    node_t* ret_node = get_node_by_node_name(graph, target_node_name);

    EXPECT_STREQ(ret_node->node_name, "node2");
}

