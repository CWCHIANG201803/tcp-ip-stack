#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "net/net.h"

TEST(TestNet, GiveNodeAndSubnetReturnInterface){
    // node_get_matching_subnet_interface
    node_t* node = nullptr;
    char* ip_addr = nullptr;
    interface_t* intf = node_get_matching_subnet_interface(node, ip_addr);
}