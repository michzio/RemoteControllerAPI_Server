//
// Created by Michal Ziobro on 29.11.2016.
//

/**
 * This code is implemented only for Mac OS X.
 */
#ifdef __APPLE__

#include "test_transfer.h"
#include "png/png-encoding.h"
#include "png/png-helper.h"
#include "bitmaps.h"
#include "bitwise.h"
#include "lz4/lz4.h"
#include "transfer/integer_transfer.h"
#include "transfer/binary_transfer.h"
#include "test/assertion.h"
#include "system/display.h"
#include "system/display_stream.h"

/**
 * PNG transfer
 * test_png_transfer()
 */
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

/**
 * Display Stream PNG transfer
 * test_display_stream_png_transfer()
 */
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

/**
 * Display Stream LZ4 transfer
 * test_display_stream_lz4_transfer()
 */
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

/*
 * Display Stream updates LZ4 transfer
 * test_display_stream_updates_lz4_transfer()
 */
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

#endif // __APPLE__