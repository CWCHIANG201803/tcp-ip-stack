#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "glthreads/glthread.h"
#include "utils/utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <memory.h>
#include <stddef.h>
#include <tuple>

TEST(TestUtils, GiveValidPrefixReturnPrefixAfterMask){
    // arrange
    char prefix[] = "122.1.1.1";
    char mask = 24;
    char* str_prefix = (char*)malloc(sizeof(prefix));
    // act
    apply_mask(prefix, mask, str_prefix);
    // assert
    ASSERT_STREQ(str_prefix, "122.1.1.0");
}

TEST(TestUtils, GiveNullPrefixThrowInvalidArgumentException){
    // arrange
    char* prefix = nullptr;
    char mask = 24;
    char* str_prefix = (char*)malloc(sizeof(prefix));
    // act+assert
    ASSERT_THROW(apply_mask(prefix, mask, str_prefix), std::invalid_argument);
}

TEST(TestUtils, GiveNullOutputPrefixThrowInvalidArgumentException){
    // arrange
    char prefix[] = "122.1.1.1";
    char mask = 24;
    char* str_prefix = nullptr;
    // act+assert
    ASSERT_THROW(apply_mask(prefix, mask, str_prefix), std::invalid_argument);
}

TEST(TestUtils, GiveMaskBoundaryValueOutofRangeThrowInvalidArgumentException){
    // arrange
    char prefix[] = "122.1.1.1";
    char mask_0 = -1;
    char mask_1 = 33;
    char* str_prefix = (char*)malloc(sizeof(prefix));
    // act+assert
    ASSERT_THROW(apply_mask(prefix, mask_0, str_prefix), std::out_of_range);
    ASSERT_THROW(apply_mask(prefix, mask_1, str_prefix), std::out_of_range);
}

TEST(TestUtils, GiveMaskBoundaryValueValidRangeReturnExpectedValue){
    // arrange
    char prefix[] = "122.1.1.1";
    char mask_0 = 0;
    char* str_prefix_0 = (char*)malloc(sizeof(prefix));
    apply_mask(prefix, mask_0, str_prefix_0);
    ASSERT_STREQ(str_prefix_0, "0.0.0.0");

    char mask_1 = 32;
    char* str_prefix_1 = (char*)malloc(sizeof(prefix));
    apply_mask(prefix, mask_1, str_prefix_1);
    ASSERT_STREQ(str_prefix_1, "122.1.1.1");
}

class TestUtils : public ::testing::TestWithParam<std::tuple<const char*, bool>> {};

TEST_P(TestUtils, GiveInputStrReturnValidateIpPrefix){
	const char* target = std::get<0>(GetParam());
	bool expected = std::get<1>(GetParam());
	ASSERT_EQ(validate_prefix(target),expected);
}

INSTANTIATE_TEST_SUITE_P(IpPrefixInputsValidation, TestUtils, ::testing::Values(
        std::make_tuple("127.0.0.1", true),
        std::make_tuple("192.168.1.1", true),
        std::make_tuple("192.168.1.255", true),
        std::make_tuple("255.255.255.255", true),
        std::make_tuple("0.0.0.0", true),
        std::make_tuple("1.1.1.01", false),
        std::make_tuple("30.168.1.255.1", false),
        std::make_tuple("127.1", false),
        std::make_tuple("192.168.1.256", false),
        std::make_tuple("-1.2.3.4", false),
        std::make_tuple("3...3", false)
	));
