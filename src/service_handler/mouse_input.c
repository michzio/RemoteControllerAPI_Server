//
// Created by Michal Ziobro on 20/08/2017.
//

#include <stdlib.h>
#include <mouse_events.h>
#include <memory.h>
#include <scroll_wheel_events.h>
#include <hot_keys_events.h>
#include "service_handler/mouse_input.h"
#include "strings.h"

static const char MOUSE_BUTTON_LEFT[] = "left";
static const char MOUSE_BUTTON_RIGHT[] = "right";

static const char SCROLL_DIRECTION_VERTICAL[] = "vertical";
static const char SCROLL_DIRECTION_HORIZONTAL[] = "horizontal";
static const char SWIPE_DIRECTION_VERTICAL[] = "vertical";
static const char SWIPE_DIRECTION_HORIZONTAL[] = "horizontal";

result_t handle_mouse_click(const char *request, size_t req_len) {

    // tokenize request message
    size_t num_tokens = 0;
    char **tokens = str_split(request, ": ", &num_tokens);

    if(num_tokens < 2) {
        fprintf(stderr, "Could not find correct number of tokens in rpc mouse click request.");
        return FAILURE;
    }

    if(strncmp(tokens[1], MOUSE_BUTTON_LEFT, sizeof(MOUSE_BUTTON_LEFT)-1) == 0) {
        mouse_left_click();
    } else if(strncmp(tokens[1], MOUSE_BUTTON_RIGHT, sizeof(MOUSE_BUTTON_RIGHT)-1) == 0) {
        mouse_right_click();
    }

    // free tokens
    free_tokens(tokens, num_tokens);
}

result_t handle_mouse_double_click(const char *request, size_t req_length) {

    // tokenize request message
    size_t num_tokens = 0;
    char **tokens = str_split(request, ": ", &num_tokens);

    if(num_tokens < 2) {
        fprintf(stderr, "Could not find correct number of tokens in rpc mouse double click request.");
        return FAILURE;
    }

    if(strncmp(tokens[1], MOUSE_BUTTON_LEFT, sizeof(MOUSE_BUTTON_LEFT)-1) == 0) {
        mouse_left_double_click();
    } else if(strncmp(tokens[1], MOUSE_BUTTON_RIGHT, sizeof(MOUSE_BUTTON_RIGHT)-1) == 0) {
        mouse_right_double_click();
    }

    // free tokens
    free_tokens(tokens, num_tokens);
}

result_t handle_mouse_triple_click(const char *request, size_t req_length) {

    // tokenize request message
    size_t num_tokens = 0;
    char **tokens = str_split(request, ": ", &num_tokens);

    if(num_tokens < 2) {
        fprintf(stderr, "Could not find correct number of tokens in rpc mouse triple click request.");
        return FAILURE;
    }

    if(strncmp(tokens[1], MOUSE_BUTTON_LEFT, sizeof(MOUSE_BUTTON_LEFT)-1) == 0) {
        mouse_left_triple_click();
    } else if(strncmp(tokens[1], MOUSE_BUTTON_RIGHT, sizeof(MOUSE_BUTTON_RIGHT)-1) == 0) {
        mouse_right_triple_click();
    }

    // free tokens
    free_tokens(tokens, num_tokens);
}

result_t handle_mouse_move_by(const char *request, size_t req_len) {

    // tokenize request message
    size_t num_tokens = 0;
    char **tokens = str_split(request, ": ", &num_tokens);

    if(num_tokens < 2) {
        fprintf(stderr, "Could not find correct number of tokens in rpc mouse move by request.");
        return FAILURE;
    }

    size_t num_displacements = 0;
    char **displacements = str_split(tokens[1], " ", &num_displacements);

    if(num_displacements != 2) {
        fprintf(stderr, "Could not find correct number of displacement coordinates in rpc mouse move by request.");
        return FAILURE;
    }

    double dx = atof(displacements[0]);
    double dy = atof(displacements[1]);

    mouse_move_by(dx, dy);

    // free tokens, displacements
    free_tokens(tokens, num_tokens);
    free_tokens(displacements, num_displacements);
}

result_t handle_mouse_move_to(const char *request, size_t req_len) {

    // tokenize request message
    size_t num_tokens = 0;
    char **tokens = str_split(request, ": ", &num_tokens);

    if(num_tokens < 2) {
        fprintf(stderr, "Could not find correct number of tokens in rpc mouse move to request.");
        return FAILURE;
    }

    size_t num_displacements = 0;
    char **displacements = str_split(tokens[1], " ", &num_displacements);

    if(num_displacements != 2) {
        fprintf(stderr, "Could not find correct number of displacement coordinates in rpc mouse move to request.");
        return FAILURE;
    }

    double x = atof(displacements[0]);
    double y = atof(displacements[1]);

    mouse_move_to(x, y);

    // free tokens, displacements
    free_tokens(tokens, num_tokens);
    free_tokens(displacements, num_displacements);
}

result_t handle_mouse_drag_by(const char *request, size_t req_length) {

    // tokenize request message
    size_t num_tokens = 0;
    char **tokens = str_split(request, ": ", &num_tokens);

    if(num_tokens < 2) {
        fprintf(stderr, "Could not find correct number of tokens in rpc mouse drag by request.");
        return FAILURE;
    }

    size_t num_displacements = 0;
    char **displacements = str_split(tokens[1], " ", &num_displacements);

    if(num_displacements != 2) {
        fprintf(stderr, "Could not find correct number of displacement coordinates in rpc mouse drag by request.");
        return FAILURE;
    }

    double dx = atof(displacements[0]);
    double dy = atof(displacements[1]);

    mouse_left_drag_by(dx, dy);

    // free tokens, displacements
    free_tokens(tokens, num_tokens);
    free_tokens(displacements, num_displacements);
}

result_t handle_mouse_drag_to(const char *request, size_t req_length) {

    // tokenize request message
    size_t num_tokens = 0;
    char **tokens = str_split(request, ": ", &num_tokens);

    if(num_tokens < 2) {
        fprintf(stderr, "Could not find correct number of tokens in rpc mouse darg to request.");
        return FAILURE;
    }

    size_t num_displacements = 0;
    char **displacements = str_split(tokens[1], " ", &num_displacements);

    if(num_displacements != 2) {
        fprintf(stderr, "Could not find correct number of displacement coordinates in rpc mouse drag to request.");
        return FAILURE;
    }

    double x = atof(displacements[0]);
    double y = atof(displacements[1]);

    mouse_left_drag_to(x, y);

    // free tokens, displacements
    free_tokens(tokens, num_tokens);
    free_tokens(displacements, num_displacements);
}

result_t handle_mouse_scroll(const char *request, size_t req_len) {

    // tokenize request message
    size_t num_tokens = 0;
    char **tokens = str_split(request, ": ", &num_tokens);

    if(num_tokens < 3) {
        fprintf(stderr, "Could not find correct number of tokens in rpc mouse scroll request.");
        return FAILURE;
    }

    double scroll_value = atof(strtok(tokens[1], " "));
    char *scroll_direction = tokens[2];

    if(strcmp(scroll_direction, SCROLL_DIRECTION_VERTICAL) == 0) {
        scroll_wheel_scroll( (int) scroll_value);
    }

    // free tokens
    free_tokens(tokens, num_tokens);
}

result_t handle_mouse_pinch(const char *request, size_t req_length) {

    // tokenize request message
    size_t num_tokens = 0;
    char **tokens = str_split(request, ": ", &num_tokens);

    if(num_tokens < 2) {
        fprintf(stderr, "Could not find correct number of tokens in rpc mouse pinch request.");
        return FAILURE;
    }

    double scaleFactor = atof(tokens[1]);

    if(scaleFactor > 1.0) {
        zoom_hot_keys_event(kZoomIn);
    } else {
        zoom_hot_keys_event(kZoomOut);
    }

    // free tokens
    free_tokens(tokens, num_tokens);
}

result_t handle_mouse_triple_swipe(const char *request, size_t req_length) {

    // tokenize request message
    size_t num_tokens = 0;
    char **tokens = str_split(request, ": ", &num_tokens);

    if(num_tokens < 3) {
        fprintf(stderr, "Could not find correct number of tokens in rpc mouse triple swipe request.");
        return FAILURE;
    }

    double swipe_value = atof(strtok(tokens[1], " "));
    char *swipe_direction = tokens[2];

    if(strcmp(swipe_direction, SWIPE_DIRECTION_VERTICAL) == 0) {
        if(swipe_value > 0) {
            swipe_hot_keys_event(kSwipeDown);
        } else {
            swipe_hot_keys_event(kSwipeUp);
        }
    } else if(strcmp(swipe_direction, SWIPE_DIRECTION_HORIZONTAL) == 0) {
        if(swipe_value > 0) {
            swipe_hot_keys_event(kSwipeRight);
        } else {
            swipe_hot_keys_event(kSwipeLeft);
        }
    }

    // free tokens
    free_tokens(tokens, num_tokens);
}

result_t handle_mouse_rotate(const char *request, size_t req_length) {

    // tokenize request message
    size_t num_tokens = 0;
    char **tokens = str_split(request, ": ", &num_tokens);

    if(num_tokens < 2) {
        fprintf(stderr, "Could not find correct number of tokens in rpc mouse rotate request.");
        return FAILURE;
    }

    double rotation_angle = atof(tokens[1]);

    if(rotation_angle > 0) {
        rotate_hot_keys_event(kRotateCounterClockWise);
    } else {
        rotate_hot_keys_event(kRotateClockWise);
    }

    // free tokens
    free_tokens(tokens, num_tokens);
}