//
// Created by Michal Ziobro on 15/10/2016.
//

#include "test_transfer.h"
#include <stdlib.h>
#include "../../common/bitwise.h"
#include "../../networking/transfer/integer_transfer.h"
#include "../../networking/transfer/str_transfer.h"
#include "../../networking/transfer/binary_transfer.h"
#include "../../unit_tests/test/assertion.h"
#include "../../unit_tests/common/terminal.h"

#ifdef __APPLE__
    #include "test_transfer_osx.c"
#endif

static result_t uint8_transfer_handler(sock_fd_t cs_fd) {

    result_t res = 0;
    uint8_t recvedUint8 = 0;

    res = recv_uint8(cs_fd, &recvedUint8);
    assert_equal_int(res, SUCCESS, "Received uint8 number from socket");

    printf(ANSI_COLOR_CYAN "uint8 received: %hhu\n" ANSI_COLOR_RESET, recvedUint8);

    res = send_uint8(cs_fd, recvedUint8);
    assert_equal_int(res, SUCCESS, "Sent uint8 number to socket");

    return CLOSED;
}

static void test_uint8_transfer(void) {
    test_create_stream_server(uint8_transfer_handler);
}

static result_t uint16_transfer_handler(sock_fd_t cs_fd) {

    result_t res = 0;
    uint16_t recvedUint16 = 0;

    res = recv_uint16(cs_fd, &recvedUint16);
    assert_equal_int(res, SUCCESS, "Received uint16 number from socket");

    printf(ANSI_COLOR_CYAN "uint16 received: %hu\n" ANSI_COLOR_RESET, recvedUint16);

    res = send_uint16(cs_fd, recvedUint16);
    assert_equal_int(res, SUCCESS, "Sent uint16 number to socket");

    return CLOSED;
}

static void test_uint16_transfer(void) {
    test_create_stream_server(uint16_transfer_handler);
}

static result_t uint32_transfer_handler(sock_fd_t cs_fd) {

    result_t res = 0;
    uint32_t recvedUint32 = 0;

    res = recv_uint32(cs_fd, &recvedUint32);
    assert_equal_int(res, SUCCESS, "Received uint32 number from socket");

    printf(ANSI_COLOR_CYAN "uint32 received: %u\n" ANSI_COLOR_RESET, recvedUint32);

    res = send_uint32(cs_fd, recvedUint32);
    assert_equal_int(res, SUCCESS, "Sent uint32 number to socket");

    return CLOSED;
}

static void test_uint32_transfer(void) {
    test_create_stream_server(uint32_transfer_handler);
}

static result_t uint64_transfer_handler(sock_fd_t cs_fd) {

    result_t res = 0;
    uint64_t recvedUint64 = 0;

    res = recv_uint64(cs_fd, &recvedUint64);
    assert_equal_int(res, SUCCESS, "Received uint64 number from socket");

    printf(ANSI_COLOR_CYAN "uint64 received: %llu\n" ANSI_COLOR_RESET, recvedUint64);

    res = send_uint64(cs_fd, recvedUint64);
    assert_equal_int(res, SUCCESS, "Sent uint64 number to socket");

    return CLOSED;
}

static void test_uint64_transfer(void) {
    test_create_stream_server(uint64_transfer_handler);
}

static result_t int8_transfer_handler(sock_fd_t cs_fd) {

    result_t res = 0;
    int8_t recvedInt8 = 0;

    res = recv_int8(cs_fd, &recvedInt8);
    assert_equal_int(res, SUCCESS, "Received int8 number from socket");

    printf(ANSI_COLOR_CYAN "int8 received: %hhi\n" ANSI_COLOR_RESET, recvedInt8);

    res = send_int8(cs_fd, recvedInt8);
    assert_equal_int(res, SUCCESS, "Sent int8 number to socket");

    return CLOSED;
}

static void test_int8_transfer(void) {
    test_create_stream_server(int8_transfer_handler);
}

static result_t int16_transfer_handler(sock_fd_t cs_fd) {

    result_t res = 0;
    int16_t recvedInt16 = 0;

    res = recv_int16(cs_fd, &recvedInt16);
    assert_equal_int(res, SUCCESS, "Received int16 number from socket");

    printf(ANSI_COLOR_CYAN "int16 received: %hi\n" ANSI_COLOR_RESET, recvedInt16);

    res = send_int16(cs_fd, recvedInt16);
    assert_equal_int(res, SUCCESS, "Sent int16 number to socket");

    return CLOSED;
}

static void test_int16_transfer(void) {
    test_create_stream_server(int16_transfer_handler);
}

static result_t int32_transfer_handler(sock_fd_t cs_fd) {

    result_t res = 0;
    int32_t recvedInt32 = 0;

    res = recv_int32(cs_fd, &recvedInt32);
    assert_equal_int(res, SUCCESS, "Received int32 number from socket");

    printf(ANSI_COLOR_CYAN "int32 received: %i\n" ANSI_COLOR_RESET, recvedInt32);

    res = send_int32(cs_fd, recvedInt32);
    assert_equal_int(res, SUCCESS, "Sent int32 number to socket");

    return CLOSED;
}

static void test_int32_transfer(void) {
    test_create_stream_server(int32_transfer_handler);
}

static result_t int64_transfer_handler(sock_fd_t cs_fd) {

    result_t res = 0;
    int64_t recvedInt64 = 0;

    res = recv_int64(cs_fd, &recvedInt64);
    assert_equal_int(res, SUCCESS, "Received int64 number from socket");

    printf(ANSI_COLOR_CYAN "int64 received: %lli\n" ANSI_COLOR_RESET, recvedInt64);

    res = send_int64(cs_fd, recvedInt64);
    assert_equal_int(res, SUCCESS, "Sent int64 number to socket");

    return CLOSED;
}

static void test_int64_transfer(void) {
    test_create_stream_server(int64_transfer_handler);
}

#define BINARY_DATA_LENGTH 32

static result_t binary_transfer_handler(sock_fd_t cs_fd) {

    result_t res = 0;
    unsigned char recvedBinaryData[BINARY_DATA_LENGTH];

    res = recv_binary(cs_fd, 8, recvedBinaryData, BINARY_DATA_LENGTH);
    assert_equal_int(res, SUCCESS, "Received binary data from socket");

    printf(ANSI_COLOR_CYAN);
    bytes_array_dump(recvedBinaryData, BINARY_DATA_LENGTH, 8);
    printf(ANSI_COLOR_RESET);

    res = send_binary(cs_fd, 8, recvedBinaryData, BINARY_DATA_LENGTH);
    assert_equal_int(res, SUCCESS, "Sent binary data to socket");

    return CLOSED;
}

static void test_binary_transfer(void) {
    test_create_stream_server(binary_transfer_handler);
}

static result_t cstring_transfer_handler(sock_fd_t cs_fd) {

    result_t res = 0;
    char *recvedCString = 0;
    size_t recvedCStringLen = 0;

    res = recv_cstr(cs_fd, &recvedCString, &recvedCStringLen);
    assert_equal_int(res, SUCCESS, "Received cstring from socket");

    printf(ANSI_COLOR_CYAN "cstring received: %s\n" ANSI_COLOR_RESET, recvedCString);
    printf(ANSI_COLOR_CYAN "cstring length: %lu\n" ANSI_COLOR_RESET, recvedCStringLen);

    res = send_cstr(cs_fd, recvedCString, recvedCStringLen);
    assert_equal_int(res, SUCCESS, "Sent cstring to socket");

    free(recvedCString);

    return CLOSED;
}

static void test_cstring_transfer(void) {
    test_create_stream_server(cstring_transfer_handler);
}

static void run_tests(void) {

    printf( ANSI_COLOR_BLUE "Integration Test - requires to run: 'client' program only with 'test_client_transfer.run_tests()' \n" ANSI_COLOR_RESET);

    //test_uint8_transfer();
    //test_uint16_transfer();
    //test_uint32_transfer();
    //test_uint64_transfer();
    //test_int8_transfer();
    //test_int16_transfer();
    //test_int32_transfer();
    //test_int64_transfer();
    //test_binary_transfer();
    //test_cstring_transfer();

    //test_png_transfer();
    //test_display_stream_png_transfer();
    //test_display_stream_lz4_transfer();
    //test_display_stream_updates_lz4_transfer();
}

test_server_transfer_t test_server_transfer = { .run_tests = run_tests };