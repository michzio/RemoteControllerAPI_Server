//
// Created by Michal Ziobro on 20/08/2017.
//

#ifndef REMOTECONTROLLERAPI_SERVER_MOUSE_CLICK_H
#define REMOTECONTROLLERAPI_SERVER_MOUSE_CLICK_H

#include <ntsid.h>
#include <types.h>

result_t handle_mouse_click(const char *request, size_t req_len);
result_t handle_mouse_double_click(const char *request, size_t req_length);
result_t handle_mouse_triple_click(const char *request, size_t req_length);
result_t handle_mouse_move_by(const char *request, size_t req_len);
result_t handle_mouse_move_to(const char *request, size_t req_len);
result_t handle_mouse_drag_by(const char *request, size_t req_length);
result_t handle_mouse_drag_to(const char *request, size_t req_length);
result_t handle_mouse_scroll(const char *request, size_t req_len);
result_t handle_mouse_pinch(const char *request, size_t req_length);
result_t handle_mouse_triple_swipe(const char *request, size_t req_length);
result_t handle_mouse_rotate(const char *request, size_t req_length);

#endif //REMOTECONTROLLERAPI_SERVER_MOUSE_CLICK_H
