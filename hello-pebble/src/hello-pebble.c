#include <pebble.h>

Window *window; 
TextLayer *text_layer;


void init() {
    // Creates the window
    window = window_create(); 
    
    //Push to the stack, animated
    window_stack_push(window,true);
    
    //Create the Textlayer, for display at (0,0),
    //and with a size of 144 x 40
    text_layer = text_layer_create(GRect(0,0,144,40));
    
    //Set the text that the TextLayer will display
    text_layer_set_text(text_layer,"Hello,Pebble!"); 
    
    //Add as child layer to be included in rendering 
    layer_add_child(window_get_root_layer(window),
                    text_layer_get_layer(text_layer));
    

}

void deinit() {
    // Destroy text layer
    text_layer_destroy(text_layer); 
    
    // Destroy window
    window_destroy(window);
    

    
}

int main() {
    //Initialize the app
    init();
    
    //wait for app events
    app_event_loop();
    
    //Deinitialize the app
    deinit(); 
    
    //App finished without error 
    return 0; 
}
  

