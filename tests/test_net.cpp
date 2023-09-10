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

    std::stringstream buffer{};
    // Save cout's buffer here
    std::streambuf *sbuf;
    sbuf = std::cout.rdbuf();
    // Redirect cout to our stringstream buffer or any other ostream
    std::cout.rdbuf(buffer.rdbuf());

    char topology_name[]="test_graph";
    graph_t* graph = create_new_graph(topology_name);

    char name_node1[] = "R1";
    node_t* node1 = create_graph_node(graph, name_node1);
    node1->node_nw_prop.is_lb_addr_config = true;
    strcpy(node1->node_nw_prop.lb_addr.ip_addr, "122.1.1.1");
    node1->intf[0] = create_node_interface(node1, "eth0/0", "20.1.1.2", 24);
    node1->intf[1] = create_node_interface(node1, "eth0/1", "10.1.1.1", 24);

    char name_node2[] = "R2";
    node_t* node2 = create_graph_node(graph, name_node2);
    node2->node_nw_prop.is_lb_addr_config = true;
    strcpy(node2->node_nw_prop.lb_addr.ip_addr, "122.1.1.2");
    node2->intf[0] = create_node_interface(node2, "eth0/2", "10.1.1.2",24);
    node2->intf[1] = create_node_interface(node2, "eth0/3", "20.1.1.1",24);

    link_t lnk30(*node2->intf[1], *node1->intf[0]);
    node2->intf[1]->link = &lnk30;
    node1->intf[0]->link = &lnk30;

    link_t lnk12(*node1->intf[1], *node2->intf[0]);
    node1->intf[1]->link = &lnk12;
    node2->intf[0]->link = &lnk12;

    dump_new_nw_graph(graph);
    std::string actual{buffer.str()};
    std::string expected = " Topology Name = test_graph\n\
\nNode Name = R2\n\
\t  lo addr : 122.1.1.2/32\n\
\n\t IP Addr = 10.1.1.2/\x18\n\
\t MAC : 0:0:0:0:0:0\n\
\t IP Addr = 20.1.1.1/\x18\n\
\t MAC : 0:0:0:0:0:0\n\
\nNode Name = R1\n\
\t  lo addr : 122.1.1.1/32\n\
\n\t IP Addr = 20.1.1.2/\x18\n\
\t MAC : 0:0:0:0:0:0\n\
\t IP Addr = 10.1.1.1/\x18\n\
\t MAC : 0:0:0:0:0:0\n";
    ASSERT_EQ(actual, expected);
}

TEST(TestNet, GiveIpAddrReturnUnsignedIntForm) {
    char ip_addr[] = "192.168.53.5";
    unsigned int res = convert_ip_from_str_to_int(ip_addr);
    unsigned int expected = 3232249093;
    ASSERT_EQ(res, expected);
}

TEST(TestNet, GiveUnsignedIntValReturnToIpAddr) {
    char* output_buffer = new char[20];
    unsigned int ip_addr = 335610113;
    convert_ip_from_int_to_str(ip_addr, output_buffer);
    ASSERT_STREQ(output_buffer, "20.1.1.1");
}