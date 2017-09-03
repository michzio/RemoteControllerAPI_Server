//
// Created by Michal Ziobro on 20/08/2017.
//

#include <memory.h>
#include <hot_keys_events.h>
#include <system/audio.h>
#include <stdlib.h>
#include <system/events.h>
#include "service_handler/app_event.h"
#include "strings.h"

// player actions
static const char ACTION_VOLUME_MUTE[] = "volume mute";
static const char ACTION_VOLUME_DOWN[] = "volume down";
static const char ACTION_VOLUME_UP[] = "volume up";
static const char ACTION_VOLUME_LEVEL[] = "volume level";
static const char ACTION_PLAY_PAUSE[] = "play";
static const char ACTION_STOP[] = "stop";
static const char ACTION_STEP_FORWARD[] = "step forward";
static const char ACTION_STEP_BACKWARD[] = "step backward";
static const char ACTION_SKIP_NEXT[] = "skip next";
static const char ACTION_SKIP_PREVIOUS[] = "skip previous";
static const char ACTION_LOOP[] = "loop";
static const char ACTION_SHUFFLE[] = "shuffle";
static const char ACTION_FULLSCREEN[] = "fullscreen";
static const char ACTION_SUBTITLES[] = "subtitles";

// slide show actions
static const char ACTION_START[] = "start";
static const char ACTION_END[] = "end";
static const char ACTION_PREVIOUS[] = "previous";
static const char ACTION_NEXT[] = "next";
static const char ACTION_POINTER[] = "pointer";
static const char ACTION_BLANK_SLIDE[] = "blank slide";
static const char ACTION_FIRST_SLIDE[] = "first slide";
static const char ACTION_LAST_SLIDE[] = "last slide";

// browser actions
static const char ACTION_NEW_TAB[] = "new tab";
static const char ACTION_PREVIOUS_TAB[] = "previous tab";
static const char ACTION_NEXT_TAB[] = "next tab";
static const char ACTION_CLOSE_TAB[] = "close tab";
static const char ACTION_OPEN_FILE[] = "open file";
static const char ACTION_NEW_PRIVATE_WINDOW[] = "new private window";
static const char ACTION_REOPEN_CLOSED_TAB[] = "reopen_closed_tab";
static const char ACTION_CLOSE_WINDOW[] = "close window";
static const char ACTION_SHOW_DOWNLOADS[] = "show downloads";
static const char ACTION_SHOW_HISTORY[] = "show history";
static const char ACTION_SHOW_SIDEBAR[] = "show sidebar";
static const char ACTION_SHOW_PAGE_SOURCE[] = "show page source";
static const char ACTION_HOME_PAGE[] = "home page";
static const char ACTION_RELOAD_PAGE[] = "reload page";
static const char ACTION_BOOKMARK_PAGE[] = "bookmark page";
static const char ACTION_ENTER_FULL_SCREEN[] = "enter full screen";
static const char ACTION_ZOOM_OUT[] = "zoom out";
static const char ACTION_ZOOM_ACTUAL_SIZE[] = "zoom actual size";
static const char ACTION_ZOOM_IN[] = "zoom in";
static const char ACTION_ENTER_LOCATION[] = "enter location";

// system actions
static const char ACTION_SHUT_DOWN[] = "shut down";
static const char ACTION_RESTART[] = "restart";
static const char ACTION_SLEEP[] = "sleep";
static const char ACTION_LOG_OUT[] = "log out";

// shortcut actions
static const char ACTION_SELECT_ALL[] = "select all";
static const char ACTION_CUT[] = "cut";
static const char ACTION_COPY[] = "copy";
static const char ACTION_PASTE[] = "paste";
//static const char ACTION_OPEN_FILE[] = "open file";
static const char ACTION_SAVE[] = "save";
static const char ACTION_FIND[] = "find";
static const char ACTION_PRINT[] = "print";
static const char ACTION_NEW_WINDOW[] = "new window";
static const char ACTION_MINIMIZE_WINDOW[] = "minimize window";
//static const char ACTION_CLOSE_WINDOW[] = "close window";
static const char ACTION_SWITCH_APPS[] = "switch apps";
static const char ACTION_UNDO[] = "undo";
static const char ACTION_REDO[] = "redo";
static const char ACTION_SYSTEM_SEARCH[] = "system search";
static const char ACTION_FORCE_QUIT[] = "force quit";
static const char ACTION_SHOW_DESKTOP[] = "show desktop";
static const char ACTION_LEFT_DESKTOP[] = "left desktop";
static const char ACTION_RIGHT_DESKTOP[] = "right desktop";

result_t handle_player_event(const char *request, size_t req_len) {

    // tokenize request message
    size_t num_tokens = 0;
    char **tokens = str_split(request, ": ", &num_tokens);

    if(num_tokens < 2) {
        fprintf(stderr, "Could not find correct number of tokens in rpc player event request.");
        return FAILURE;
    }

    if( strncmp(tokens[1], ACTION_VOLUME_MUTE, sizeof(ACTION_VOLUME_MUTE)-1) == 0 ){
        player_mute_hot_keys_event();
    } else if( strncmp(tokens[1], ACTION_VOLUME_DOWN, sizeof(ACTION_VOLUME_DOWN)-1) == 0) {
        player_volume_down_hot_keys_event();
    } else if( strncmp(tokens[1], ACTION_VOLUME_UP, sizeof(ACTION_VOLUME_UP)-1) == 0) {
        player_volume_up_hot_keys_event();
    } else if( strncmp(tokens[1], ACTION_PLAY_PAUSE, sizeof(ACTION_PLAY_PAUSE)-1) == 0) {
        player_play_hot_keys_event();
    } else if( strncmp(tokens[1], ACTION_STOP, sizeof(ACTION_STOP)-1) == 0) {
        player_stop_hot_keys_event();
    } else if( strncmp(tokens[1], ACTION_STEP_FORWARD, sizeof(ACTION_STEP_FORWARD)-1) == 0) {
        player_step_forward_hot_keys_event();
    } else if( strncmp(tokens[1], ACTION_STEP_BACKWARD, sizeof(ACTION_STEP_BACKWARD)-1) == 0) {
        player_step_backward_hot_keys_event();
    } else if( strncmp(tokens[1], ACTION_SKIP_NEXT, sizeof(ACTION_SKIP_NEXT)-1) == 0) {
        player_next_hot_keys_event();
    } else if( strncmp(tokens[1], ACTION_SKIP_PREVIOUS, sizeof(ACTION_SKIP_PREVIOUS)-1) == 0) {
        player_previous_hot_keys_event();
    } else if( strncmp(tokens[1], ACTION_LOOP, sizeof(ACTION_LOOP)-1) == 0) {
        player_loop_hot_keys_event();
    } else if( strncmp(tokens[1], ACTION_SHUFFLE, sizeof(ACTION_SHUFFLE)-1) == 0) {
        player_shuffle_hot_keys_event();
    } else if( strncmp(tokens[1], ACTION_FULLSCREEN, sizeof(ACTION_FULLSCREEN)-1) == 0) {
        player_full_screen_hot_keys_event();
    } else if( strncmp(tokens[1], ACTION_SUBTITLES, sizeof(ACTION_SUBTITLES)-1) == 0) {
        player_subtitles_hot_keys_event();
    }

    // free tokens
    free_tokens(tokens, num_tokens);
}

result_t handle_slide_show_event(const char *request, size_t req_len) {

    // tokenize request message
    size_t num_tokens = 0;
    char **tokens = str_split(request, ": ", &num_tokens);

    if(num_tokens < 2) {
        fprintf(stderr, "Could not find correct number of tokens in rpc slide show event request.");
        return FAILURE;
    }

    if( strncmp(tokens[1], ACTION_START, sizeof(ACTION_START)-1) == 0 ){
        slide_show_start_hot_keys_event();
    } else if( strncmp(tokens[1], ACTION_END, sizeof(ACTION_END)-1) == 0) {
        slide_show_end_hot_keys_event();
    } else if( strncmp(tokens[1], ACTION_PREVIOUS, sizeof(ACTION_PREVIOUS)-1) == 0) {
        if(num_tokens == 3 && ( strncmp(tokens[2], "yes", 3) == 0) ) {
            slide_show_previous_hot_keys_event();
        } else {
            slide_show_previous_no_animation_hot_keys_event();
        }
    } else if( strncmp(tokens[1], ACTION_NEXT, sizeof(ACTION_NEXT)-1) == 0) {
        if(num_tokens == 3 && (strncmp(tokens[2], "yes", 3) == 0) ) {
            slide_show_next_hot_keys_event();
        } else {
            slide_show_next_no_animation_hot_keys_event();
        }
    } else if( strncmp(tokens[1], ACTION_POINTER, sizeof(ACTION_POINTER)-1) == 0) {
        if(num_tokens < 3) {
            fprintf(stderr, "Could not find correct number of tokens for rpc slide show request's action pointer");
            return FAILURE;
        }

        if( strncmp(tokens[2], "arrow", 5) == 0 ) {
            slide_show_arrow_pointer_hot_keys_event();
        } else if( strncmp(tokens[2], "pen", 3) == 0) {
            slide_show_pen_pointer_hot_keys_event();
        }
    } else if( strncmp(tokens[1], ACTION_BLANK_SLIDE, sizeof(ACTION_BLANK_SLIDE)-1) == 0) {
        if(num_tokens < 3) {
            fprintf(stderr, "Could not find correct number of tokens for rpc slide show request's action blank slide");
            return FAILURE;
        }

        if( strncmp(tokens[2], "black", 5) == 0 ) {
            slide_show_blank_slide_black_hot_keys_event();
        } else if( strncmp(tokens[2], "pen", 3) == 0) {
            slide_show_blank_slide_white_hot_keys_event();
        }
    } else if( strncmp(tokens[1], ACTION_FIRST_SLIDE, sizeof(ACTION_FIRST_SLIDE)-1) == 0) {
        slide_show_first_slide_hot_keys_event();
    } else if( strncmp(tokens[1], ACTION_LAST_SLIDE, sizeof(ACTION_LAST_SLIDE)-1) == 0) {
        slide_show_last_slide_hot_keys_event();
    }
}

result_t handle_browser_event(const char *request, size_t req_len) {

    // tokenize request message
    size_t num_tokens = 0;
    char **tokens = str_split(request, ": ", &num_tokens);

    if(num_tokens < 2) {
        fprintf(stderr, "Could not find correct number of tokens in rpc browser event request.");
        return FAILURE;
    }

    if( strncmp(tokens[1], ACTION_NEW_TAB, sizeof(ACTION_NEW_TAB)-1) == 0 ){
        browser_new_tab_hot_keys_event();
    } else if( strncmp(tokens[1], ACTION_PREVIOUS_TAB, sizeof(ACTION_PREVIOUS_TAB)-1) == 0) {
       browser_prev_tab_hot_keys_event();
    } else if( strncmp(tokens[1], ACTION_NEXT_TAB, sizeof(ACTION_NEXT_TAB)-1) == 0) {
        browser_next_tab_hot_keys_event();
    } else if( strncmp(tokens[1], ACTION_CLOSE_TAB, sizeof(ACTION_CLOSE_TAB)-1) == 0) {
        browser_close_tab_hot_keys_event();
    } else if( strncmp(tokens[1], ACTION_OPEN_FILE, sizeof(ACTION_OPEN_FILE)-1) == 0) {
        browser_open_file_hot_keys_event();
    } else if( strncmp(tokens[1], ACTION_NEW_PRIVATE_WINDOW, sizeof(ACTION_NEW_PRIVATE_WINDOW)-1) == 0) {
        browser_new_private_window_hot_keys_event();
    } else if( strncmp(tokens[1], ACTION_REOPEN_CLOSED_TAB, sizeof(ACTION_REOPEN_CLOSED_TAB)-1) == 0) {
        browser_reopen_closed_tab_hot_keys_event();
    } else if( strncmp(tokens[1], ACTION_CLOSE_WINDOW, sizeof(ACTION_CLOSE_WINDOW)-1) == 0) {
        browser_close_window_hot_keys_event();
    } else if( strncmp(tokens[1], ACTION_SHOW_DOWNLOADS, sizeof(ACTION_SHOW_DOWNLOADS)-1) == 0) {
        browser_show_downloads_hot_keys_event();
    } else if( strncmp(tokens[1], ACTION_SHOW_HISTORY, sizeof(ACTION_SHOW_HISTORY)-1) == 0) {
        browser_show_history_hot_keys_event();
    }  else if( strncmp(tokens[1], ACTION_SHOW_SIDEBAR, sizeof(ACTION_SHOW_SIDEBAR)-1) == 0) {
        browser_show_sidebar_hot_keys_event();
    } else if( strncmp(tokens[1], ACTION_SHOW_PAGE_SOURCE, sizeof(ACTION_SHOW_PAGE_SOURCE)-1) == 0) {
        browser_show_page_source_hot_keys_event();
    } else if( strncmp(tokens[1], ACTION_HOME_PAGE, sizeof(ACTION_HOME_PAGE)-1) == 0) {
        browser_home_page_hot_keys_event();
    } else if( strncmp(tokens[1], ACTION_RELOAD_PAGE, sizeof(ACTION_RELOAD_PAGE)-1) == 0) {
        browser_reload_page_hot_keys_event();
    } else if( strncmp(tokens[1], ACTION_BOOKMARK_PAGE, sizeof(ACTION_BOOKMARK_PAGE)-1) == 0) {
        browser_add_bookmark_hot_keys_event();
    } else if( strncmp(tokens[1], ACTION_ENTER_FULL_SCREEN, sizeof(ACTION_ENTER_FULL_SCREEN)-1) == 0) {
        enter_full_screen_hot_keys_event();
    } else if( strncmp(tokens[1], ACTION_ZOOM_OUT, sizeof(ACTION_ZOOM_OUT)-1) == 0) {
        zoom_hot_keys_event(kZoomOut);
    } else if( strncmp(tokens[1], ACTION_ZOOM_ACTUAL_SIZE, sizeof(ACTION_ZOOM_ACTUAL_SIZE)-1) == 0) {
        zoom_actual_size_hot_keys_event();
    } else if( strncmp(tokens[1], ACTION_ZOOM_IN, sizeof(ACTION_ZOOM_IN)-1) == 0) {
        zoom_hot_keys_event(kZoomIn);
    } else if( strncmp(tokens[1], ACTION_ENTER_LOCATION, sizeof(ACTION_ENTER_LOCATION)-1) == 0) {
        browser_open_location_hot_keys_event();
    }
}

result_t handle_system_event(const char *request, size_t req_len) {

    // tokenize request message
    size_t num_tokens = 0;
    char **tokens = str_split(request, ": ", &num_tokens);

    if(num_tokens < 2) {
        fprintf(stderr, "Could not find correct number of tokens in rpc system request.");
        return FAILURE;
    }

    if(strncmp(tokens[1], ACTION_VOLUME_MUTE, sizeof(ACTION_VOLUME_MUTE)-1) == 0) {
        audio_output_mute(!audio_input_is_muted());
    } else if(strncmp(tokens[1], ACTION_VOLUME_LEVEL, sizeof(ACTION_VOLUME_LEVEL)-1) == 0) {
        if(num_tokens < 3) {
            fprintf(stderr, "Could not find correct number of tokens for rpc system request's action volume level");
            return FAILURE;
        }
            float volume_level = atoi(tokens[2])/100.0;
            volume_level = (volume_level < 0.0f) ? 0.0f : (volume_level > 1.0f) ? 1.0f : volume_level;
            audio_output_set_volume(volume_level);
    } else if(strncmp(tokens[1], ACTION_SHUT_DOWN, sizeof(ACTION_SHUT_DOWN)-1) == 0) {
        system_shutdown();
    } else if(strncmp(tokens[1], ACTION_RESTART, sizeof(ACTION_RESTART)-1) == 0) {
        system_restart();
    } else if(strncmp(tokens[1], ACTION_SLEEP, sizeof(ACTION_SLEEP)-1) == 0) {
        system_sleep();
    } else if(strncmp(tokens[1], ACTION_LOG_OUT, sizeof(ACTION_LOG_OUT)-1) == 0) {
        system_logout();
    }

    // free tokens
    free_tokens(tokens, num_tokens);
}


result_t handle_shortcut_event(const char *request, size_t req_len) {

    // tokenize request message
    size_t num_tokens = 0;
    char **tokens = str_split(request, ": ", &num_tokens);

    if(num_tokens < 2) {
        fprintf(stderr, "Could not find correct number of tokens in rpc shortcut request.");
        return FAILURE;
    }

    if(strncmp(tokens[1], ACTION_SELECT_ALL, sizeof(ACTION_SELECT_ALL)-1) == 0) {
        system_hot_keys_event(kSelectAllHotKeys);
    } else if(strncmp(tokens[1], ACTION_CUT, sizeof(ACTION_CUT)-1) == 0) {
        system_hot_keys_event(kCutHotKeys);
    } else if(strncmp(tokens[1], ACTION_COPY, sizeof(ACTION_COPY)-1) == 0) {
        system_hot_keys_event(kCopyHotKeys);
    } else if(strncmp(tokens[1], ACTION_PASTE, sizeof(ACTION_PASTE)-1) == 0) {
        system_hot_keys_event(kPasteHotKeys);
    } else if(strncmp(tokens[1], ACTION_OPEN_FILE, sizeof(ACTION_OPEN_FILE)-1) == 0) {
        system_hot_keys_event(kOpenFileHotKeys);
    } else if(strncmp(tokens[1], ACTION_SAVE, sizeof(ACTION_SAVE)-1) == 0) {
        system_hot_keys_event(kSaveHotKeys);
    } else if(strncmp(tokens[1], ACTION_FIND, sizeof(ACTION_FIND)-1) == 0) {
        system_hot_keys_event(kFindHotKeys);
    } else if(strncmp(tokens[1], ACTION_PRINT, sizeof(ACTION_PRINT)-1) == 0) {
        system_hot_keys_event(kPrintHotKeys);
    } else if(strncmp(tokens[1], ACTION_NEW_WINDOW, sizeof(ACTION_NEW_WINDOW)-1) == 0) {
        system_hot_keys_event(kOpenNewWindowHotKeys);
    } else if(strncmp(tokens[1], ACTION_MINIMIZE_WINDOW, sizeof(ACTION_MINIMIZE_WINDOW)-1) == 0) {
        system_hot_keys_event(kMinimizeFrontWindowHotKeys);
    } else if(strncmp(tokens[1], ACTION_CLOSE_WINDOW, sizeof(ACTION_CLOSE_WINDOW)-1) == 0) {
        system_hot_keys_event(kCloseFrontWindowHotKeys);
    } else if(strncmp(tokens[1], ACTION_SWITCH_APPS, sizeof(ACTION_SWITCH_APPS)-1) == 0) {
        system_hot_keys_event(kSwitchAppsHotKeys);
    } else if(strncmp(tokens[1], ACTION_UNDO, sizeof(ACTION_UNDO)-1) == 0) {
        system_hot_keys_event(kUndoHotKeys);
    } else if(strncmp(tokens[1], ACTION_REDO, sizeof(ACTION_REDO)-1) == 0) {
        system_hot_keys_event(kRedoHotKeys);
    } else if(strncmp(tokens[1], ACTION_SYSTEM_SEARCH, sizeof(ACTION_SYSTEM_SEARCH)-1) == 0) {
        system_hot_keys_event(kSpotlightHotKeys);
    } else if(strncmp(tokens[1], ACTION_FORCE_QUIT, sizeof(ACTION_FORCE_QUIT)-1) == 0) {
        system_hot_keys_event(kForceQuitHotKeys);
    } else if(strncmp(tokens[1], ACTION_SHOW_DESKTOP, sizeof(ACTION_SHOW_DESKTOP)-1) == 0) {
        system_hot_keys_event(kShowDesktopHotKeys);
    } else if(strncmp(tokens[1], ACTION_LEFT_DESKTOP, sizeof(ACTION_LEFT_DESKTOP)-1) == 0) {
        system_hot_keys_event(kLeftDesktopHotKeys);
    } else if(strncmp(tokens[1], ACTION_RIGHT_DESKTOP, sizeof(ACTION_RIGHT_DESKTOP)-1) == 0) {
        system_hot_keys_event(kRightDesktopHotKeys);
    }

    // free tokens
    free_tokens(tokens, num_tokens);
}