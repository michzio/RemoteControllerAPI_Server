//
// Created by Michal Ziobro on 15/10/2016.
//

#include <stdio.h>
#include <unistd.h>
#include <memory.h>
#include <errno.h>
#include <stdlib.h>
#include "test_transfer.h"
#include "../../common/types.h"
#include "../../common/bitwise.h"
#include "../../networking/common/network_types.h"
#include "../networking/generic_server.h"
#include "../../networking/transfer/integer_transfer.h"
#include "../../networking/transfer/str_transfer.h"
#include "../../networking/transfer/binary_transfer.h"
#include "../../unit_tests/test/assertion.h"
#include "../../unit_tests/common/terminal.h"
#include "../../system/OS_X/automation_scripts/system/display.h"
#include "../../system/OS_X/automation_scripts/system/display_stream.h"
#include "../../common/libraries/png/png-encoding.h"
#include "../../common/libraries/png/png-helper.h"
#include "../../common/bitmaps.h"
#include "../../common/libraries/lz4/lz4.h"

#define TEST_PORT "3333"

// custom server loop handler - without looping, just single connection handling for testing purposes
static result_t test_stream_server_handler(sock_fd_t ps_fd, connection_handler_t handle_connection) {

    int cs_fd = accept_new_connection(ps_fd);
    if(cs_fd == FAILURE || cs_fd == CONTINUE) {
        fprintf(stderr, "accept_new_connection: failed!\n");
        return FAILURE;
    }

    printf("Handle connection on the main thread...\n");

    switch (handle_connection(cs_fd)) {
        case FAILURE:
            fprintf(stderr, "handle_connection: failed!\n");
            break;
        case CLOSED:
            printf("handle_connection: closed!\n");
            break;
        default:
            break;
    }

    if(close(cs_fd) < 0){
        fprintf(stderr, "close: %s\n", strerror(errno));
        return FAILURE;
    }

    return SUCCESS;
}

// generic method creating stream server with custom connection handler
static void test_create_stream_server(connection_handler_t test_conn_handler) {

    if(create_stream_server(TEST_PORT, test_stream_server_handler, test_conn_handler)) {
        fprintf(stderr, "create_stream_server: failed!\n");
        return;
    }
}

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

static result_t png_transfer_handler(sock_fd_t cs_fd) {

    result_t res = 0;

    unsigned char *rgbaData = 0;
    size_t rgbaDataLength = 0;
    size_t width = 0, height = 0;

    display_screen_snapshot_to_buffer(&rgbaData, &rgbaDataLength, &width, &height);
    assert_not_null(rgbaData, "Screen shot taken, rgba data not null");

    unsigned char *pngData = 0;
    size_t pngDataLength = 0;

    res = (result_t) writeRGBAintoPNGBuffer(rgbaData, width, height, PNG_BIT_DEPTH_8, &pngData, &pngDataLength);
    assert_equal_int(res, SUCCESS, "RGBA encoded into PNG");

    send_uint32(cs_fd, width);
    send_uint32(cs_fd, height);
    send_uint32(cs_fd, pngDataLength);
    res = send_binary(cs_fd, PACKET_LENGTH, pngData, pngDataLength);
    assert_equal_int(res, SUCCESS, "send PNG data to socket");

    free(rgbaData);
    free(pngData);

    return SUCCESS;
}

static void test_png_transfer(void) {
    test_create_stream_server(png_transfer_handler);
}


static unsigned char *_prevFrameBuffer = 0;

static void transferPNGDisplayStreamHandler(const void *handlerArgs, const unsigned char *frameBuffer, const size_t frameBufferLength,
                                            const size_t frameWidth, const size_t frameHeight, const size_t bytesPerPixel) {

    result_t res = 0;
    sock_fd_t cs_fd = *((sock_fd_t *) handlerArgs);

    if(_prevFrameBuffer == 0) {
        _prevFrameBuffer = malloc(sizeof(unsigned char)*frameBufferLength*3/4);
        memset(_prevFrameBuffer, 0x00, frameBufferLength*3/4);
    }

    size_t newFrameBufferLength = 0;
    unsigned char *newFrameBuffer = RGBABytesArraySkipAlpha(frameBuffer, frameBufferLength, &newFrameBufferLength);

    unsigned char *xorFrameBuffer = bitwise_xor64(_prevFrameBuffer, newFrameBuffer, newFrameBufferLength);

    unsigned char *pngData = 0;
    size_t pngDataLength = 0;
    res = (result_t) writeRGBintoPNGBuffer(xorFrameBuffer, frameWidth, frameHeight, PNG_BIT_DEPTH_8, &pngData, &pngDataLength);
    assert_equal_int(res, SUCCESS, "XORed RGBA data encoded into PNG");

    send_uint32(cs_fd, frameWidth);
    send_uint32(cs_fd, frameHeight);
    send_uint32(cs_fd, pngDataLength);
    res = send_binary(cs_fd, PACKET_LENGTH, pngData, pngDataLength);
    assert_equal_int(res, SUCCESS, "sent PNG data to socket");

    free(pngData);
    free(xorFrameBuffer);

    memcpy(_prevFrameBuffer, newFrameBuffer, newFrameBufferLength);
}

static result_t display_stream_png_transfer_handler(sock_fd_t cs_fd) {

    size_t displayWidth = 0, displayHeight = 0;

    display_get_pixel_size(&displayWidth, &displayHeight);
    CGDisplayStreamRef displayStream = display_stream_init(displayWidth, displayHeight, transferPNGDisplayStreamHandler, &cs_fd);
    display_stream_start(displayStream);
    sleep(30);
    display_stream_stop(displayStream);
    display_stream_free(displayStream);
}

static void test_display_stream_png_transfer(void) {
    _prevFrameBuffer = 0;
    test_create_stream_server(display_stream_png_transfer_handler);
}

#define FRAME_BUFFER_SIZE WIDTH_5K_16_9*HEIGHT_5K_16_9*RGBA_COMPONENTS_PER_PIXEL
#define LZ4_BUFFER_SIZE LZ4_COMPRESSBOUND(FRAME_BUFFER_SIZE)
static unsigned char xorFrameBuffer[FRAME_BUFFER_SIZE];
static unsigned char *prevFrameBuffer = 0;
static unsigned char lz4Data[LZ4_BUFFER_SIZE];

static void transferLZ4DisplayStreamHandler(const void *handlerArgs, const unsigned char *frameBuffer, const size_t frameBufferLength,
                                            const size_t frameWidth, const size_t frameHeight, const size_t bytesPerPixel) {

    result_t res = 0;
    sock_fd_t cs_fd = *((sock_fd_t *) handlerArgs);

    // XOR frame bitmap with previous frame bitmap
    if(prevFrameBuffer != 0)
        bitwise_xor64_static(prevFrameBuffer, frameBuffer, frameBufferLength, xorFrameBuffer);
    else
        memcpy(xorFrameBuffer, frameBuffer, frameBufferLength);

    // compress XORed data with LZ4
    int lz4DataLength = 0;
    if( (lz4DataLength = LZ4_compress_fast(xorFrameBuffer, lz4Data, frameBufferLength, LZ4_BUFFER_SIZE+1, 1)) == 0 ) {
        fprintf(stderr, "%s: Error at compression of data with LZ4\n", __func__);
        return;
    }
    assert_greater_than(lz4DataLength, 0, "XORed RGB data compressed with LZ4");

    send_uint32(cs_fd, lz4DataLength);
    res = send_binary(cs_fd, PACKET_LENGTH, lz4Data, lz4DataLength);
    assert_equal_int(res, SUCCESS, "sent LZ4 compressed data to socket");

    if(prevFrameBuffer) free(prevFrameBuffer);
    prevFrameBuffer = frameBuffer;
    //memcpy(prevFrameBuffer, frameBuffer, frameBufferLength);
}

static result_t display_stream_lz4_transfer_handler(sock_fd_t cs_fd) {

    size_t displayWidth = 0, displayHeight = 0;
    display_get_pixel_size(&displayWidth, &displayHeight);

    send_uint32(cs_fd, displayWidth);
    send_uint32(cs_fd, displayHeight);

    CGDisplayStreamRef displayStream = display_stream_init(displayWidth, displayHeight, transferLZ4DisplayStreamHandler, &cs_fd);
    display_stream_start(displayStream);
    sleep(30);
    display_stream_stop(displayStream);
    display_stream_free(displayStream);
}

static void test_display_stream_lz4_transfer(void) {
    test_create_stream_server(display_stream_lz4_transfer_handler);
}

static void transferLZ4DisplayStreamUpdatesHandler(const void *handlerArgs, const unsigned char *updateBuffer, const size_t updateBufferLength,
                                                    const size_t offsetX, const size_t offsetY, const size_t updateWidth,
                                                    const size_t updateHeight, const size_t bytesPerPixel) {

    result_t res = 0;
    sock_fd_t cs_fd = *((sock_fd_t *) handlerArgs);

    // send update frame metadata
    send_uint32(cs_fd, offsetX);
    send_uint32(cs_fd, offsetY);
    send_uint32(cs_fd, updateWidth);
    send_uint32(cs_fd, updateHeight);

    // compress update frame with LZ4
    int lz4DataLength = 0;
    if( (lz4DataLength = LZ4_compress_fast(updateBuffer, lz4Data, updateBufferLength, LZ4_BUFFER_SIZE+1, 1)) == 0 ) {
        fprintf(stderr, "%s: Error at compression of data with LZ4\n", __func__);
        return;
    }
    assert_greater_than(lz4DataLength, 0, "update frame compressed with LZ4");

    send_uint32(cs_fd, lz4DataLength);
    res = send_binary(cs_fd, PACKET_LENGTH, lz4Data, lz4DataLength);
    assert_equal_int(res, SUCCESS, "sent LZ4 compressed data to socket");
}

static result_t display_stream_updates_lz4_transfer_handler(sock_fd_t cs_fd) {

    size_t displayWidth = 0, displayHeight = 0;
    display_get_pixel_size(&displayWidth, &displayHeight);

    send_uint32(cs_fd, displayWidth);
    send_uint32(cs_fd, displayHeight);

    CGDisplayStreamRef displayStream = display_stream_updates_init(displayWidth, displayHeight, transferLZ4DisplayStreamUpdatesHandler, &cs_fd);
    display_stream_start(displayStream);
    sleep(30);
    display_stream_stop(displayStream);
    display_stream_free(displayStream);
}

static void test_display_stream_updates_lz4_transfer(void) {
    test_create_stream_server(display_stream_updates_lz4_transfer_handler);
}

static void run_tests(void) {

    printf( ANSI_COLOR_BLUE "Integration Test - requires to run: 'client' program only with 'test_client_transfer.run_tests()' \n" ANSI_COLOR_RESET);
   /* test_uint8_transfer();
    test_uint16_transfer();
    test_uint32_transfer();
    test_uint64_transfer();
    test_int8_transfer();
    test_int16_transfer();
    test_int32_transfer();
    test_int64_transfer();
    test_binary_transfer();
    test_cstring_transfer();
    test_png_transfer(); */
    //test_display_stream_png_transfer();
    //test_display_stream_lz4_transfer();
    test_display_stream_updates_lz4_transfer();

}

test_server_transfer_t test_server_transfer = { .run_tests = run_tests };