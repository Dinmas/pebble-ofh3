#include <pebble.h>
#include "settings.h"



static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_cal_layer;
static GFont s_time_font;
static GFont s_cal_font;
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;

static char next_trail[25];
static char next_date [10];
static char debug_char[25];
 
static void inbox_recieved_handler(DictionaryIterator *iter, void *context) {
    //Read box 1 
    Tuple *date_string_t = dict_find(iter,AppKeyDateString); 
    if (date_string_t) {
        strcpy(next_trail,date_string_t->value->cstring);
    }
    
    Tuple *debug_t = dict_find(iter,AppKeyDebug);
    if (debug_t) {
        strcpy(debug_char,debug_t->value->cstring);
        strcat(next_trail,debug_char);
    }
    
    text_layer_set_text(s_cal_layer,next_trail);
} 

static void date_update() {
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);
  
    
    strftime(next_date, sizeof(next_date), "%D", tick_time);

    
    strcpy(next_trail,"Next trail on: ");   
    strcat(next_trail, next_date);
 
    text_layer_set_text(s_cal_layer, next_trail);   
}


static void time_update() {
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);
    
    static char s_buffer[10];   
         
    if (clock_is_24h_style()) {
        
        strftime(s_buffer, sizeof(s_buffer),"%H:%M", tick_time);
        if (s_buffer[0] == '0' && s_buffer[1] == '0') {
            date_update();
        }
    }
    else { 
        strftime(s_buffer, sizeof(s_buffer),"%l:%M%p", tick_time);
        if ((s_buffer[0] == '1' && s_buffer[1] == '2' && s_buffer[3] == '0' && s_buffer[4] == '0')) {
            date_update();
        }
    }
text_layer_set_text(s_time_layer, s_buffer); 

     
}



static void main_window_load (Window *window) {  
    //Get info about the Window
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);
    
    //Define s_bg_layer
    s_background_layer = bitmap_layer_create(bounds);
    s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMG_OFH3);   
     
    //Set the bitmap onto the layer
    bitmap_layer_set_bitmap(s_background_layer,s_background_bitmap);
    bitmap_layer_set_compositing_mode(s_background_layer,GCompOpSet);
    
    
    //Define time_layer
    s_time_layer = text_layer_create(
        GRect (0,120,bounds.size.w,165));
    s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_INCONSOLATA_REGULAR_36));
    text_layer_set_font(s_time_layer, s_time_font);
    text_layer_set_text_color(s_time_layer,GColorBlack);
    text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
    
    //Define cal_layer
    s_cal_layer = text_layer_create(
        GRect(58,66,80,158));
    s_cal_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_INCONSOLATA_BOLD_16));
    text_layer_set_overflow_mode(s_cal_layer,GTextOverflowModeWordWrap);
    text_layer_set_font(s_cal_layer, s_cal_font);
    text_layer_set_text_alignment(s_cal_layer, GTextAlignmentLeft);
         
    //Add children layers to Window_layer   
    layer_add_child(window_layer,text_layer_get_layer(s_cal_layer));
    layer_add_child(window_layer,text_layer_get_layer(s_time_layer));   
    layer_add_child(window_layer,bitmap_layer_get_layer(s_background_layer));
    
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    time_update();
}


static void main_window_unload (Window *window) {
    //Destroy TextLayers
    text_layer_destroy(s_time_layer);
    text_layer_destroy(s_cal_layer);
    //Unload GFont
    fonts_unload_custom_font(s_time_font);
    fonts_unload_custom_font(s_cal_font);
    //Destroy GBitmap
    gbitmap_destroy(s_background_bitmap);
    //Destroy BitmapLayer
    bitmap_layer_destroy(s_background_layer);
}

static void init() {
    
    //Registers handling function for incoming messages and opens API to accept messages from phone.
app_message_register_inbox_received(inbox_recieved_handler);    
    app_message_open(64,64);
    printf("AppMessage API open!");
    
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
    time_update();
    //date_update();
    
    
    
}

static void deinit() {
    window_destroy(s_main_window);
   
    
}


int main(void) {
    init();
    app_event_loop();
    deinit();
}
