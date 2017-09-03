//
// Created by Michal Ziobro on 20/08/2017.
//

#ifndef REMOTECONTROLLERAPI_SERVER_APP_EVENTS_H
#define REMOTECONTROLLERAPI_SERVER_APP_EVENTS_H

#include <ntsid.h>
#include <types.h>

result_t handle_player_event(const char *request, size_t req_len);
result_t handle_browser_event(const char *request, size_t req_len);
result_t handle_slide_show_event(const char *request, size_t req_len);
result_t handle_system_event(const char *request, size_t req_len);
result_t handle_shortcut_event(const char *request, size_t req_len);

#endif //REMOTECONTROLLERAPI_SERVER_APP_EVENTS_H
