#include <pebble.h>
  
static Window *s_main_window;

static TextLayer *s_time_layer;
static TextLayer *s_sec_layer;
static TextLayer *s_day_layer;

static GFont s_time_font;
static GFont s_sec_font;


static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;



static void main_window_load(Window *window) {
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 106, 100, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, "00:00");
  
  // Create time TextLayer
  s_sec_layer = text_layer_create(GRect(90, 113, 50, 50));
  text_layer_set_background_color(s_sec_layer, GColorClear);
  text_layer_set_text_color(s_sec_layer, GColorBlack);
  text_layer_set_text(s_sec_layer, "00");
  
    s_day_layer = text_layer_create(GRect(90, 65, 50, 50));
  text_layer_set_background_color(s_day_layer, GColorClear);
  text_layer_set_text_color(s_day_layer, GColorBlack);
  text_layer_set_text(s_day_layer, "00");


  // Improve the layout to be more like a watchface
  //text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  // Create GFont
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DIGITAL_38));
  
    s_sec_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DIGITAL_30));
  
  // Create GBitmap, then set to created BitmapLayer
s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));


  // Apply to TextLayer
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  
  // Apply to TextLayer
  text_layer_set_font(s_sec_layer, s_sec_font);
  text_layer_set_text_alignment(s_sec_layer, GTextAlignmentCenter);
  
    text_layer_set_font(s_day_layer, s_sec_font);
  text_layer_set_text_alignment(s_day_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_sec_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_day_layer));

}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_sec_layer);
  text_layer_destroy(s_day_layer);
  // Unload GFont
  fonts_unload_custom_font(s_time_font);
   fonts_unload_custom_font(s_sec_font);
 

  // Destroy GBitmap
gbitmap_destroy(s_background_bitmap);

// Destroy BitmapLayer
bitmap_layer_destroy(s_background_layer);
}

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "00:00";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    // Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
}

static void update_sec() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "00";

  // Write the current hours and minutes into the buffer
    strftime(buffer, sizeof("00"), "%S", tick_time);
 
  // Display this time on the TextLayer
  text_layer_set_text(s_sec_layer, buffer);
}

static void update_day() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "00";

  // Write the current hours and minutes into the buffer
    strftime(buffer, sizeof("00"), "%d", tick_time);
 
  // Display this time on the TextLayer
  text_layer_set_text(s_day_layer, buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void sec_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_sec();
}

static void day_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_day();
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Make sure the time is displayed from the start
  update_time();
  update_sec();
    update_day();
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  tick_timer_service_subscribe(SECOND_UNIT, sec_handler);
  tick_timer_service_subscribe(DAY_UNIT, day_handler);
}

static void deinit() {
    // Destroy Window
    window_destroy(s_main_window);
}



int main(void) {
  init();
  app_event_loop();
  deinit();
}





