#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;
static GFont s_time_font;


static void update_time() {
    //get a tm structure
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);
    
    //write the current hours and minutes into a buffer
    static char s_buffer[8];
    strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                            "%H:%M" : "%I:%M", tick_time);
    
    //Display this time on the TextLayer
    text_layer_set_text(s_time_layer, s_buffer);
}


static void main_window_load (Window *window) {
    //Get info about the Window
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);
    
    //Create the TextLayer with specific bounds
    s_time_layer = text_layer_create(
        GRect (0,PBL_IF_ROUND_ELSE(58,52), bounds.size.w,50));
    
    //Create GFont
    s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_INCONSOLATA_REGULAR_36));
    
    //Apply to TextLayer
    text_layer_set_font(s_time_layer, s_time_font); 
    
    
    //Improve the layout to be more like a watchface 
    text_layer_set_background_color(s_time_layer,GColorClear);
    text_layer_set_text_color(s_time_layer,GColorBlack);
    text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
    
    
    
    //Add it as a child layer to the Window's root layer
    layer_add_child(window_layer,text_layer_get_layer(s_time_layer));   
    
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    update_time();
}

static void main_window_unload (Window *window) {
    //Destroy TextLayer
    text_layer_destroy(s_time_layer);
    //Unload GFont
    fonts_unload_custom_font(s_time_font);
}

static void init() {
    //Create main window element and assign to pointer
    s_main_window = window_create();
    
    //Set handlers to manage the elements inside the window
    window_set_window_handlers(s_main_window,(WindowHandlers) {
        .load = main_window_load,
        .unload = main_window_unload
    });
    
    //Register with TickTimerService
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
    
    //Show window on watch, animated 
    window_stack_push(s_main_window,true);
    
    //Make sure time is displayed from the start
    update_time();
    
    
}

static void deinit() {
    window_destroy(s_main_window);
    
}


int main(void) {
    init();
    app_event_loop();
    deinit();
}
