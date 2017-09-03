//
// Created by Michal Ziobro on 20/08/2017.
//

#include "service_handler/keyboard_input.h"
#include "hash_map/str_hash_map.h"
#include "keyboard_events.h"
#include "virtual_key_codes.h"
#include "symbolic_hot_keys.h"
#include "strings.h"

void virtual_keys_map_init(str_hash_map_t **virtual_keys_map) {

    // uint16_t value allocator
    allocator_t *uint16_value_allocator;
    allocator_init(&uint16_value_allocator, uint16_allocate_handler, uint16_deallocate_handler);

    // virtual key name (string) -> virtual key code (integer)
    str_hash_map_init_default(virtual_keys_map, uint16_value_allocator);

    // fill virtual_keys_map with key-value pairs
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_A", kVK_ANSI_A);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_S", kVK_ANSI_S);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_D", kVK_ANSI_D);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_F", kVK_ANSI_F);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_H", kVK_ANSI_H);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_G", kVK_ANSI_G);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_Z", kVK_ANSI_Z);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_X", kVK_ANSI_X);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_C", kVK_ANSI_C);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_V", kVK_ANSI_V);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_B", kVK_ANSI_B);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_Q", kVK_ANSI_Q);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_W", kVK_ANSI_W);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_E", kVK_ANSI_E);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_R", kVK_ANSI_R);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_Y", kVK_ANSI_Y);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_T", kVK_ANSI_T);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_1", kVK_ANSI_1);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_2", kVK_ANSI_2);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_3", kVK_ANSI_3);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_4", kVK_ANSI_4);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_5", kVK_ANSI_5);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_6", kVK_ANSI_6);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_Equal", kVK_ANSI_Equal);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_9", kVK_ANSI_9);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_7", kVK_ANSI_7);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_Minus", kVK_ANSI_Minus);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_8", kVK_ANSI_8);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_0", kVK_ANSI_0);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_RightBracket", kVK_ANSI_RightBracket);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_O", kVK_ANSI_O);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_U", kVK_ANSI_U);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_LeftBracket", kVK_ANSI_LeftBracket);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_I", kVK_ANSI_I);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_P", kVK_ANSI_P);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_L", kVK_ANSI_L);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_J", kVK_ANSI_J);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_Quote", kVK_ANSI_Quote);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_K", kVK_ANSI_K);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_Semicolon", kVK_ANSI_Semicolon);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_Backslash", kVK_ANSI_Backslash);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_Comma", kVK_ANSI_Comma);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_Slash", kVK_ANSI_Slash);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_N", kVK_ANSI_N);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_M", kVK_ANSI_M);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_Period", kVK_ANSI_Period);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_Grave", kVK_ANSI_Grave);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_KeypadDecimal", kVK_ANSI_KeypadDecimal);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_KeypadMultiply", kVK_ANSI_KeypadMultiply);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_KeypadPlus", kVK_ANSI_KeypadPlus);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_KeypadClear", kVK_ANSI_KeypadClear);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_KeypadDivide", kVK_ANSI_KeypadDivide);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_KeypadEnter", kVK_ANSI_KeypadEnter);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_KeypadMinus", kVK_ANSI_KeypadMinus);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_KeypadEquals", kVK_ANSI_KeypadEquals);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_Keypad0", kVK_ANSI_Keypad0);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_Keypad1", kVK_ANSI_Keypad1);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_Keypad2", kVK_ANSI_Keypad2);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_Keypad3", kVK_ANSI_Keypad3);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_Keypad4", kVK_ANSI_Keypad4);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_Keypad5", kVK_ANSI_Keypad5);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_Keypad6", kVK_ANSI_Keypad6);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_Keypad7", kVK_ANSI_Keypad7);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_Keypad8", kVK_ANSI_Keypad8);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ANSI_Keypad9", kVK_ANSI_Keypad9);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_Return", kVK_Return);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_Tab", kVK_Tab);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_Space", kVK_Space);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_Backspace", kVK_Backspace);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_Delete", kVK_Delete);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_Escape", kVK_Escape);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_Command", kVK_Command);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_Shift", kVK_Shift);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_CapsLock", kVK_CapsLock);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_Option", kVK_Option);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_Control", kVK_Control);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_RightShift", kVK_RightShift);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_RightOption", kVK_RightOption);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_RightControl", kVK_RightControl);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_Function", kVK_Function);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_F17", kVK_F17);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_VolumeUp", kVK_VolumeUp);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_VolumeDown", kVK_VolumeDown);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_Mute", kVK_Mute);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_F18", kVK_F18);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_F19", kVK_F19);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_F20", kVK_F20);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_F5", kVK_F5);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_F6", kVK_F6);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_F7", kVK_F7);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_F3", kVK_F3);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_F8", kVK_F8);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_F9", kVK_F9);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_F11", kVK_F11);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_F13", kVK_F13);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_F16", kVK_F16);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_F14", kVK_F14);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_F10", kVK_F10);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_F12", kVK_F12);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_F15", kVK_F15);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_Help", kVK_Help);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_Home", kVK_Home);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_PageUp", kVK_PageUp);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_ForwardDelete", kVK_ForwardDelete);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_F4", kVK_F4);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_End", kVK_End);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_F2", kVK_F2);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_PageDown", kVK_PageDown);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_F1", kVK_F1);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_LeftArrow", kVK_LeftArrow);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_RightArrow", kVK_RightArrow);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_DownArrow", kVK_DownArrow);
    str_hash_map_put_uint16(*virtual_keys_map, "kVK_UpArrow", kVK_UpArrow);
}

void modifier_flags_map_init(str_hash_map_t **modifier_flags_map) {

    // uint64_t value allocator
    allocator_t *uint64_value_allocator;
    allocator_init(&uint64_value_allocator, uint64_allocate_handler, uint64_deallocate_handler);

    // modifier flag name (string) -> modifier flag code  (integer)
    str_hash_map_init_default(modifier_flags_map, uint64_value_allocator);

    // fill modifier_flags_map with key-value pairs
    str_hash_map_put_uint64(*modifier_flags_map, "kMFCapsLock", kMFShift); // TODO
    str_hash_map_put_uint64(*modifier_flags_map, "kMFLeftShift", kMFLeftShift);
    str_hash_map_put_uint64(*modifier_flags_map, "kMFControl", kMFControl);
    str_hash_map_put_uint64(*modifier_flags_map, "kMFLeftOption", kMFLeftOption);
    str_hash_map_put_uint64(*modifier_flags_map, "kMFCommand", kMFCommand);
    str_hash_map_put_uint64(*modifier_flags_map, "kMFRightOption", kMFRightOption);
    str_hash_map_put_uint64(*modifier_flags_map, "kMFRightShift", kMFRightShift);
}

result_t handle_keyboard_input(const char *request, size_t req_len) {

    // map of virtual keys and modifier flags
    static str_hash_map_t *virtual_keys_map = NULL;
    static str_hash_map_t *modifier_flags_map = NULL;

    if(virtual_keys_map == NULL) {
        virtual_keys_map_init(&virtual_keys_map);
    }
    if(modifier_flags_map == NULL) {
        modifier_flags_map_init(&modifier_flags_map);
    }

    // tokenize request message
    size_t num_tokens = 0;
    char **tokens = str_split(request, ": ", &num_tokens);

    if(num_tokens < 3) {
        fprintf(stderr, "Could not find correct number of tokens in rpc keyboard input request.");
        return FAILURE;
    }

    // get virtual key name and modifier flags string
    char *virtual_key_name = strtok(tokens[1], " ");
    size_t num_flags = 0;
    char **modifier_flags = str_split(tokens[2], ", ", &num_flags);

    // map string virtual key name and modifier flag names into numeric codes
    uint16_t virtual_key_code = str_hash_map_get_uint16(virtual_keys_map, virtual_key_name);
    if(virtual_key_code == HASH_MAP_UINT16_NOT_FOUND) {
        fprintf(stderr, "Could not find virtual key code for given virtual key name.");
        return FAILURE;
    }

    uint64_t modifier_flags_code = 0;
    for(int i=0; i<num_flags; i++) {
        uint64_t modifier_flag_code = str_hash_map_get_uint64(modifier_flags_map, modifier_flags[i]);
        if(modifier_flag_code == HASH_MAP_UINT64_NOT_FOUND)
            continue;
        else
            modifier_flags_code |= modifier_flag_code;
    }

    // execute keyboard input event based on retrieved values
    if (modifier_flags_code > 0) {
        fprintf(stderr,  "Virtual key: %d modifier flags: %d\n",  virtual_key_code, modifier_flags_code);
        key_input_modified(virtual_key_code, modifier_flags_code);
    } else {
        fprintf(stderr,  "Virtual key: %d\n",  virtual_key_code);
        key_input(virtual_key_code);
    }

    // free tokens, modifier_flags
    free_tokens(tokens, num_tokens);
    free_tokens(modifier_flags, num_flags);

    return SUCCESS;
}

