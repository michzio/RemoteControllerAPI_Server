//
// Created by Michal Ziobro on 28/07/2016.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test_doubly_linked_list.h"
#include "assertion.h"

static doubly_linked_list_t *list;

static void test_create(void) {

    list_init(&list);

    for(int i=0; i<10; i++) {
        char *str = malloc(256);
        sprintf(str, "test %d", i);
        push_front(list, str, sizeof(str));
    }
}
static void test_clean(void) {
    free_doubly_linked_list(list);
}

static void test_travers_forward(void) {
    test_create();
    printf("%s: ", __func__); travers_forward(list, print_string_data_handler); printf("\n");
    test_clean();
}
static void test_travers_backward(void) {
    test_create();
    printf("%s: ", __func__); travers_backward(list, print_string_data_handler); printf("\n");
    test_clean();
}
static void test_find_first(void) {
    test_create();
    doubly_linked_node_t *found_node = find_first(list, "test 5", str_cmp_fun);
    assert_not_null(found_node, __func__);
    test_clean();
}
static void test_insert_at_pos(void) {
    test_create();
    doubly_linked_node_t *found_node = find_first(list, "test 5", str_cmp_fun);
    char *data =  malloc(256);
    strcpy(data, "new test");
    insert_at_pos(list, found_node, data, sizeof(data));
    printf("%s: ", __func__); travers_forward(list, print_string_data_handler); printf("\n");
    test_clean();
}
static void test_insert_node_at_pos(void) {
    test_create();
    doubly_linked_node_t *found_node = find_first(list, "test 5", str_cmp_fun);
    doubly_linked_node_t *new_node;
    char *data =  malloc(256);
    strcpy(data, "new node");
    node_init(&new_node, data, sizeof(data));
    insert_node_at_pos(list, found_node, new_node);
    printf("%s: ", __func__); travers_forward(list, print_string_data_handler); printf("\n");
    test_clean();
}
static void test_push_front(void) {
    test_create();
    char *data =  malloc(256);
    strcpy(data, "front test");
    push_front(list, data, sizeof(data));
    printf("%s: ", __func__); travers_forward(list, print_string_data_handler); printf("\n");
    test_clean();
}
static void test_push_node_front(void) {
    test_create();
    doubly_linked_node_t *front_node;
    char *data =  malloc(256);
    strcpy(data, "front node");
    node_init(&front_node, data, sizeof(data));
    push_node_front(list, front_node);
    printf("%s: ", __func__); travers_forward(list, print_string_data_handler); printf("\n");
    test_clean();
}
static void test_push_back(void) {
    test_create();
    char *data =  malloc(256);
    strcpy(data, "back test");
    push_back(list, data, sizeof(data));
    printf("%s: ", __func__); travers_forward(list, print_string_data_handler); printf("\n");
    test_clean();
}
static void test_push_node_back(void) {
    test_create();
    doubly_linked_node_t *back_node;
    char *data =  malloc(256);
    strcpy(data, "back node");
    node_init(&back_node, data, sizeof(data));
    push_node_back(list, back_node);
    printf("%s: ", __func__); travers_forward(list, print_string_data_handler); printf("\n");
    test_clean();
}
static void test_remove_node(void) {
    test_create();
    doubly_linked_node_t *old_node1 = find_first(list, "test 5", str_cmp_fun);
    doubly_linked_node_t *old_node2 = find_first(list, "test 4", str_cmp_fun);
    doubly_linked_node_t *old_node3 = find_first(list, "test 9", str_cmp_fun);
    remove_node(list, old_node1);
    remove_node(list, old_node2);
    remove_node(list, old_node3);
    printf("%s: ", __func__); travers_forward(list, print_string_data_handler); printf("\n");
    test_clean();
}
static void test_front(void) {
    test_create();
    doubly_linked_node_t *front_node = front(list);
    assert_equal(unwrap_data(front_node, NULL), "test 9", (compare_func_t) strcmp, __func__);
    test_clean();
}
static void test_back(void) {
    test_create();
    doubly_linked_node_t *back_node = back(list);
    assert_equal(unwrap_data(back_node, NULL), "test 0", (compare_func_t) strcmp, __func__);
    test_clean();
}
static void test_pop_front(void) {
    test_create();
    pop_front(list);
    printf("%s: ", __func__); travers_forward(list, print_string_data_handler); printf("\n");
    test_clean();
}
static void test_pop_back(void) {
    test_create();
    pop_back(list);
    printf("%s: ", __func__); travers_forward(list, print_string_data_handler); printf("\n");
    test_clean();
}

static void run_tests(void) {
    test_travers_forward();
    test_travers_backward();
    test_find_first();
    test_insert_at_pos();
    test_insert_node_at_pos();
    test_push_front();
    test_push_node_front();
    test_push_back();
    test_push_node_back();
    test_remove_node();
    test_front();
    test_back();
    test_pop_front();
    test_pop_back();
}

test_doubly_linked_list_t test_doubly_linked_list = { .run_tests = run_tests };