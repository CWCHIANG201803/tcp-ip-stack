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