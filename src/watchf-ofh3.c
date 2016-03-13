#include <pebble.h>


static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_cal_layer;
static GFont s_time_font;
static GFont s_cal_font;
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;


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
    //text_layer_set_text(s_cal_layer, date_buffer);
}

static const char *update_date() {
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);
    
    static char next_trail[25];
    static char next_date [10];
    
   strftime(next_date, sizeof(next_date), "%D", tick_time);

    strcpy(next_trail,"Next trail on: ");
    strcat(next_trail, next_date); 
    
    return next_trail;
}


static void main_window_load (Window *window) {
    

    
    
    
    //Get info about the Window
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);
    
    //Create GBitmap 
    s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMG_OFH3);
    
    //Create BitmapLayer to display the GBitmap
    s_background_layer = bitmap_layer_create(bounds);
    
    //Set the bitmap onto the layer and add to window
    bitmap_layer_set_bitmap(s_background_layer,s_background_bitmap);
    bitmap_layer_set_compositing_mode(s_background_layer,GCompOpSet);
    
    
    //Create the TextLayers with specific bounds
    s_time_layer = text_layer_create(
        GRect (0,120,bounds.size.w,165));
    s_cal_layer = text_layer_create(
        GRect(58,66,80,158));
       
    
    text_layer_set_overflow_mode(s_cal_layer,GTextOverflowModeWordWrap);
    text_layer_set_text(s_cal_layer, update_date());
    
    //Create GFont
    s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_INCONSOLATA_REGULAR_36));
    s_cal_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_INCONSOLATA_BOLD_16));
    
    
    //Apply to TextLayer
    text_layer_set_font(s_time_layer, s_time_font); 
    text_layer_set_font(s_cal_layer, s_cal_font);
    
    
    //Improve the layout to be more like a watchface 
  
    text_layer_set_text_color(s_time_layer,GColorBlack);
    text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
    text_layer_set_text_alignment(s_cal_layer, GTextAlignmentLeft);
  
    
    
    
    //Add children layers to Window_layer   
    layer_add_child(window_layer,text_layer_get_layer(s_cal_layer));
    layer_add_child(window_layer,text_layer_get_layer(s_time_layer));   
    layer_add_child(window_layer,bitmap_layer_get_layer(s_background_layer));
    
};

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    update_time();
}

static void main_window_unload (Window *window) {
    //Destroy TextLayer
    text_layer_destroy(s_time_layer);
    text_layer_destroy(s_cal_layer);
    //Unload GFont
    fonts_unload_custom_font(s_time_font);
    //Destroy GBitmap
    gbitmap_destroy(s_background_bitmap);
    //Destroy BitmapLayer
    bitmap_layer_destroy(s_background_layer);
}

static void init() {
    //Create main window element and assign to pointer
    s_main_window = window_create();
    
    
    
    //Set handlers to manage the elements inside the window
    window_set_window_handlers(s_main_window,(WindowHandlers) {
        .load = main_window_load,
        .unload = main_window_unload
    });
    
    window_set_background_color(s_main_window,GColorClear);
    
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
