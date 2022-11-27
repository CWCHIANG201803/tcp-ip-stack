#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "glthreads/glthread.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <stddef.h>

using ::testing::HasSubstr;
using ::testing::MatchesRegex;
using ::testing::IsEmpty;
using ::testing::StrEq;

typedef struct emp_ {
    char name[30];
    unsigned int salary;
    char designation[30];
    unsigned int emp_id;
    glthread_t glnode;
} emp_t; 

void print_emp_details(emp_t *emp){
    printf("Employee name = %s\n", emp->name);
    printf("salary = %u\n", emp->salary);
    printf("designation = %s\n", emp->designation);
    printf("emp_id = %u\n", emp->emp_id);
}

TEST(TestGlthread, TestSample1)
{
    emp_t *emp1  = (emp_t *)calloc(1, sizeof(emp_t));
    strncpy(emp1->name, "Neha", strlen("Neha"));
    emp1->salary = 50000;
    strncpy(emp1->designation, "HR", strlen("HR"));
    emp1->emp_id = 21;
    init_glthread(&emp1->glnode);

    ASSERT_FALSE(emp1->glnode.left != NULL);
    ASSERT_FALSE(emp1->glnode.right != NULL);
}

TEST(TestGlthread, TestSample2)
{
    /*Now Create a glthread*/
    emp_t *emp1  = (emp_t *)calloc(1, sizeof(emp_t));
    strncpy(emp1->name, "Neha", strlen("Neha"));
    emp1->salary = 50000;
    strncpy(emp1->designation, "HR", strlen("HR"));
    emp1->emp_id = 21;
    init_glthread(&emp1->glnode);

    glthread_t *emp_list = (glthread_t *)calloc(1, sizeof(glthread_t));
    init_glthread(emp_list);
    
    glthread_add_next(emp_list, &emp1->glnode);

    ASSERT_TRUE(emp_list->right!=NULL);
    ASSERT_FALSE(emp_list->left!=NULL);

    ASSERT_TRUE(emp1->glnode.left!=NULL);
    ASSERT_FALSE(emp1->glnode.right!=NULL);

}