#include <pebble.h>

static Window *s_main_window;

static void main_window_load (Window *window) {
    
}

static void main_window_unload (Window *window) {
    
}

static void init() {
    //Create main window element and assign to pointer
    s_main_window = window_create();
    
    //Set handlers to manage the elements inside the window
    window_set_window_handlers(s_main_window,(WindowHandlers) {
        .load = main_window_load,
        .unload = main_window_unload
    });
    
    //Show window on watch, animated 
    window_stack_push(s_main_window,true);
    
    
}

static void deinit() {
    window_destroy(s_main_window);
    
}


int main(void) {
    init();
    app_event_loop();
    deinit();
}