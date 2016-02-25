#include <pebble.h>

Window *window; 


void init() {
    // Creates the window
    window = window_create(); 
    
    //Push to the stack, animated
    window_stack_push(window,true);

}

void deinit() {
    // Destroy window
    window_destroy(window);
    
}

int main(void) {
    //Initialize the app
    init();
    
    //wait for app events
    app_event_loop();
    
    //Deinitialize the app
    deinit(); 
    
    //App finished without error 
    return 0; 
}
  

