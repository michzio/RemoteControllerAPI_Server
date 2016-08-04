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

    list_init(&list, NULL);

    for(int i=0; i<10; i++) {
        char *str = malloc(256);
        sprintf(str, "test %d", i);
        push_front(list, str, strlen(str));
    }
}

static void test_create_with_allocator(void) {

    allocator_t *allocator;
    allocator_init(&allocator, string_allocate_handler, string_deallocate_handler);

    list_init(&list, allocator);

    for(int i=0; i<10; i++) {
        char *str = malloc(256);
        sprintf(str, "test %d", i);
        push_front(list, str, strlen(str));
    }
}


static void test_clean(void) {

    travers_forward(list, free); // deallocate all date stored in the list
    list_free(list);
}

static void test_clean_with_allocator(void) {

    list_free(list);
}

static void test_travers_forward(void) {
    test_create();
    printf("%s: ", __func__); travers_forward(list, print_string_data_handler); printf("\n");
    test_clean();
}
static void test_travers_forward_with_allocator(void) {
    test_create_with_allocator();
    printf("%s: ", __func__); travers_forward(list, print_string_data_handler); printf("\n");
    test_clean_with_allocator();
}
static void test_travers_backward(void) {
    test_create();
    printf("%s: ", __func__); travers_backward(list, print_string_data_handler); printf("\n");
    test_clean();
}
static void test_travers_backward_with_allocator(void) {
    test_create_with_allocator();
    printf("%s: ", __func__); travers_backward(list, print_string_data_handler); printf("\n");
    test_clean_with_allocator();
}
static void test_find_first(void) {
    test_create();
    doubly_linked_node_t *found_node = find_first(list, "test 5", str_cmp_func);
    assert_not_null(found_node, __func__);
    test_clean();
}
static void test_find_first_with_allocator(void) {
    test_create_with_allocator();
    doubly_linked_node_t *found_node = find_first(list, "test 5", str_cmp_func);
    assert_not_null(found_node, __func__);
    test_clean_with_allocator();
}
static void test_insert_at_pos(void) {
    test_create();
    doubly_linked_node_t *found_node = find_first(list, "test 5", str_cmp_func);
    char *data =  malloc(256);
    strcpy(data, "new test");
    insert_at_pos(list, found_node, data, strlen(data));
    printf("%s: ", __func__); travers_forward(list, print_string_data_handler); printf("\n");
    test_clean();
}
static void test_insert_at_pos_with_allocator(void) {
    test_create_with_allocator();
    doubly_linked_node_t *found_node = find_first(list, "test 5", str_cmp_func);
    char *data = malloc(256);
    strcpy(data, "new test");
    insert_at_pos(list, found_node, data, strlen(data));
    printf("%s: ", __func__); travers_forward(list, print_string_data_handler); printf("\n");
    test_clean_with_allocator();
}
static void test_insert_node_at_pos(void) {
    test_create();
    doubly_linked_node_t *found_node = find_first(list, "test 5", str_cmp_func);
    doubly_linked_node_t *new_node;
    char *data =  malloc(256);
    strcpy(data, "new node");
    node_init(&new_node);
    wrap_data(list, new_node, data, strlen(data));
    insert_node_at_pos(list, found_node, new_node);
    printf("%s: ", __func__); travers_forward(list, print_string_data_handler); printf("\n");
    test_clean();
}
static void test_insert_node_at_pos_with_allocator(void) {
    test_create_with_allocator();
    doubly_linked_node_t *found_node = find_first(list, "test 5", str_cmp_func);
    doubly_linked_node_t *new_node;
    char *data = malloc(256);
    strcpy(data, "new node");
    node_init(&new_node);
    wrap_data(list, new_node, data, strlen(data));
    insert_node_at_pos(list, found_node, new_node);
    printf("%s: ", __func__); travers_forward(list, print_string_data_handler); printf("\n");
    test_clean_with_allocator();
}
static void test_push_front(void) {
    test_create();
    char *data =  malloc(256);
    strcpy(data, "front test");
    push_front(list, data, strlen(data));
    printf("%s: ", __func__); travers_forward(list, print_string_data_handler); printf("\n");
    test_clean();
}
static void test_push_front_with_allocator(void) {
    test_create_with_allocator();
    char *data =  malloc(256);
    strcpy(data, "front test");
    push_front(list, data, strlen(data));
    printf("%s: ", __func__); travers_forward(list, print_string_data_handler); printf("\n");
    test_clean_with_allocator();
}
static void test_push_node_front(void) {
    test_create();
    doubly_linked_node_t *front_node;
    char *data =  malloc(256);
    strcpy(data, "front node");
    node_init(&front_node);
    wrap_data(list, front_node,  data, strlen(data));
    push_node_front(list, front_node);
    printf("%s: ", __func__); travers_forward(list, print_string_data_handler); printf("\n");
    test_clean();
}
static void test_push_node_front_with_allocator(void) {
    test_create_with_allocator();
    doubly_linked_node_t *front_node;
    char *data = malloc(256);
    strcpy(data, "front node");
    node_init(&front_node);
    wrap_data(list, front_node, data, strlen(data));
    push_node_front(list, front_node);
    printf("%s: ", __func__); travers_forward(list, print_string_data_handler); printf("\n");
    test_clean_with_allocator();
}
static void test_push_back(void) {
    test_create();
    char *data =  malloc(256);
    strcpy(data, "back test");
    push_back(list, data, strlen(data));
    printf("%s: ", __func__); travers_forward(list, print_string_data_handler); printf("\n");
    test_clean();
}
static void test_push_back_with_allocator(void) {
    test_create_with_allocator();
    char *data = malloc(256);
    strcpy(data, "back test");
    push_back(list, data, strlen(data));
    printf("%s: ", __func__); travers_forward(list, print_string_data_handler); printf("\n");
    test_clean_with_allocator();
}
static void test_push_node_back(void) {
    test_create();
    doubly_linked_node_t *back_node;
    char *data = malloc(256);
    strcpy(data, "back node");
    node_init(&back_node);
    wrap_data(list, back_node, data, strlen(data));
    push_node_back(list, back_node);
    printf("%s: ", __func__); travers_forward(list, print_string_data_handler); printf("\n");
    test_clean();
}
static void test_push_node_back_with_allocator(void) {
    test_create_with_allocator();
    doubly_linked_node_t *back_node;
    char *data = malloc(256);
    strcpy(data, "back node");
    node_init(&back_node);
    wrap_data(list, back_node, data, strlen(data));
    push_node_back(list, back_node);
    printf("%s: ", __func__); travers_forward(list, print_string_data_handler); printf("\n");
    test_clean_with_allocator();
}
static void test_remove_node(void) {
    test_create();
    doubly_linked_node_t *old_node1 = find_first(list, "test 5", str_cmp_func);
    doubly_linked_node_t *old_node2 = find_first(list, "test 4", str_cmp_func);
    doubly_linked_node_t *old_node3 = find_first(list, "test 9", str_cmp_func);
    free(unwrap_data(old_node1, NULL));
    free(unwrap_data(old_node2, NULL));
    free(unwrap_data(old_node3, NULL));
    remove_node(list, old_node1);
    remove_node(list, old_node2);
    remove_node(list, old_node3);
    printf("%s: ", __func__); travers_forward(list, print_string_data_handler); printf("\n");
    test_clean();
}
static void test_remove_node_with_allocator(void) {
    test_create_with_allocator();
    doubly_linked_node_t *old_node1 = find_first(list, "test 5", str_cmp_func);
    doubly_linked_node_t *old_node2 = find_first(list, "test 4", str_cmp_func);
    doubly_linked_node_t *old_node3 = find_first(list, "test 9", str_cmp_func);
    remove_node(list, old_node1);
    remove_node(list, old_node2);
    remove_node(list, old_node3);
    printf("%s: ", __func__); travers_forward(list, print_string_data_handler); printf("\n");
    test_clean_with_allocator();
}
static void test_front(void) {
    test_create();
    doubly_linked_node_t *front_node = front(list);
    assert_equal(unwrap_data(front_node, NULL), "test 9", (compare_func_t) strcmp, __func__);
    test_clean();
}
static void test_front_with_allocator(void) {
    test_create_with_allocator();
    doubly_linked_node_t *front_node = front(list);
    assert_equal(unwrap_data(front_node, NULL), "test 9", (compare_func_t) strcmp, __func__);
    test_clean_with_allocator();
}
static void test_back(void) {
    test_create();
    doubly_linked_node_t *back_node = back(list);
    assert_equal(unwrap_data(back_node, NULL), "test 0", (compare_func_t) strcmp, __func__);
    test_clean();
}
static void test_back_with_allocator(void) {
    test_create_with_allocator();
    doubly_linked_node_t *back_node = back(list);
    assert_equal(unwrap_data(back_node, NULL), "test 0", (compare_func_t) strcmp, __func__);
    test_clean_with_allocator();
}
static void test_pop_front(void) {
    test_create();
    free(unwrap_data(front(list), NULL));
    pop_front(list);
    printf("%s: ", __func__); travers_forward(list, print_string_data_handler); printf("\n");
    test_clean();
}
static void test_pop_front_with_allocator(void) {
    test_create_with_allocator();
    pop_front(list);
    printf("%s: ", __func__); travers_forward(list, print_string_data_handler); printf("\n");
    test_clean_with_allocator();
}
static void test_pop_back(void) {
    test_create();
    free(unwrap_data(back(list), NULL));
    pop_back(list);
    printf("%s: ", __func__); travers_forward(list, print_string_data_handler); printf("\n");
    test_clean();
}
static void test_pop_back_with_allocator(void) {
    test_create_with_allocator();
    pop_back(list);
    printf("%s: ", __func__); travers_forward(list, print_string_data_handler); printf("\n");
    test_clean_with_allocator();
}
static void run_tests(void) {
    test_travers_forward();
    test_travers_forward_with_allocator();
    test_travers_backward();
    test_travers_backward_with_allocator();
    test_find_first();
    test_find_first_with_allocator();
    test_insert_at_pos();
    test_insert_at_pos_with_allocator();
    test_insert_node_at_pos();
    test_insert_node_at_pos_with_allocator();
    test_push_front();
    test_push_front_with_allocator();
    test_push_node_front();
    test_push_node_front_with_allocator();
    test_push_back();
    test_push_back_with_allocator();
    test_push_node_back();
    test_push_node_back_with_allocator();
    test_remove_node();
    test_remove_node_with_allocator();
    test_front();
    test_front_with_allocator();
    test_back();
    test_back_with_allocator();
    test_pop_front();
    test_pop_front_with_allocator();
    test_pop_back();
    test_pop_back_with_allocator();
}

test_doubly_linked_list_t test_doubly_linked_list = { .run_tests = run_tests };