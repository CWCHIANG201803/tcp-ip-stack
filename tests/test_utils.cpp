#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "glthreads/glthread.h"
#include "utils/utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <memory.h>
#include <stddef.h>


TEST(TestUtils, TestOnApplyMask){
    // arrange
    char prefix[] = "122.1.1.1";
    char mask = 24;
    char* str_prefix = (char*)malloc(sizeof(prefix));
    // act
    apply_mask(prefix, mask, str_prefix);
    // assert
    ASSERT_STREQ(str_prefix, "122.1.1.0");
}
