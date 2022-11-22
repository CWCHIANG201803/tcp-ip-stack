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

TEST(TestGraph, TestGiveNULLNodeReturnNegativeCount){
    node_t *node = NULL;
    int count = get_node_intf_available_slot(node);
    EXPECT_EQ(count, -1);
}

TEST(TestGraph, TestGiveNodeNULLInterfaceReturnCountEqMax){
    node_t *node = (node_t*)malloc(sizeof(node_t));
    for(int i=0; i < MAX_INTF_PER_NODE; ++i)
        node->intf[i] = NULL;

    int count = get_node_intf_available_slot(node);

    EXPECT_EQ(count, MAX_INTF_PER_NODE);
}

TEST(TestGraph, TestGiveNodeInterfaceReturnCountLTMax){
    node_t *node = (node_t*)malloc(sizeof(node_t));
    for(int i=0; i < MAX_INTF_PER_NODE; ++i)
        node->intf[i] = NULL;
    node->intf[0] = (interface_t*)malloc(sizeof(interface_t));
    
    int count = get_node_intf_available_slot(node);

    EXPECT_LT(count, MAX_INTF_PER_NODE);
}