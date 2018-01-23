#include <pebble.h>

static Window *s_main_window;

static TextLayer *s_time_layer;
static TextLayer *s_date_layer;
static TextLayer *s_connected_indicator;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  // Write current hours and minutes into a buffer
  static char s_time_buffer[8];
  strftime(s_time_buffer, sizeof(s_time_buffer), clock_is_24h_style() ?
                                  "%H:%M" : "%I:%M", tick_time);
  
  // Write current date into buffer
  static char s_date_buffer[11];
  strftime(s_date_buffer, sizeof(s_date_buffer), "%B %d", tick_time);
  
  // Display time on TextLayer
  text_layer_set_text(s_time_layer, s_time_buffer);
  text_layer_set_text(s_date_layer, s_date_buffer);
}

static void tick_handler(struct tm *time_time, TimeUnits units_changed) {
  update_time();
}

static void bluetooth_callback(bool connected) {
  // If connected, show heart and do short vibration
  if (connected) {
    text_layer_set_text(s_connected_indicator, "\U0001F497");
    vibes_short_pulse();  
  } 
  // If not connected, show poo and do LOOOONG vibration
  else {
    text_layer_set_text(s_connected_indicator, "\U0001F4A9");
    
    static const uint32_t segments[] = { 200, 50, 4000 };
    VibePattern pat = {
      .durations = segments,
      .num_segments = ARRAY_LENGTH(segments),
    };
    vibes_enqueue_custom_pattern(pat);
  }
}


static void display_text_layer(Layer *window_layer, TextLayer *layer, GFont font) {
  // Format layer to match watchface
  text_layer_set_background_color(layer, GColorClear);
  text_layer_set_text_color(layer, GColorWhite);
  text_layer_set_font(layer, font);
  text_layer_set_text_alignment(layer, GTextAlignmentCenter);
  
  // Add layer to window
  layer_add_child(window_layer, text_layer_get_layer(layer));
}


static void display_time(Layer *window_layer, GRect bounds) {
  // Create layer of proper size
  s_time_layer = text_layer_create(
      GRect(0, PBL_IF_COLOR_ELSE(60, 54), bounds.size.w, 52));
    
  // Format and display
  display_text_layer(window_layer, s_time_layer, fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49));
}

static void display_date(Layer *window_layer, GRect bounds) {
  // Create layer of proper size
  s_date_layer = text_layer_create(
      GRect(0, PBL_IF_COLOR_ELSE(38, 32), bounds.size.w, 30));
    
  // Format and display
  display_text_layer(window_layer, s_date_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
}

static void display_connected_indicator(Layer *window_layer, GRect bounds) {
  // TODO How do these pixels actually work?
  s_connected_indicator = text_layer_create(
      GRect(0, PBL_IF_COLOR_ELSE(118, 112), bounds.size.w, 110));
  
  // Set initial bluetooth icon on watchface
  text_layer_set_text(s_connected_indicator, connection_service_peek_pebble_app_connection() 
                      ? "\U0001F497" : "\U0001F4A9");
  
  // Format and display
  display_text_layer(window_layer, s_connected_indicator, fonts_get_system_font(FONT_KEY_GOTHIC_28));
}


static void main_window_load(Window *window) {
  // Get info about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  // Display layers
  display_time(window_layer, bounds);
  display_date(window_layer, bounds);
  display_connected_indicator(window_layer, bounds);
}

static void main_window_unload(Window *window) {
  // Give memory back to system when window is destroyed
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_date_layer);
}


static void init() {
  // Register with TickTimerService for time
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  // Register for bluetooth connection updates
  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = bluetooth_callback
  });
  
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the water, with animated=true
  window_stack_push(s_main_window, true);
  
  // Show time now
  update_time();
  
  // Set background
  window_set_background_color(s_main_window, GColorBlack);
}

static void deinit() {
  // Give memory back to system when app exists
  window_destroy(s_main_window);
}


int main(void) {
  init();
  app_event_loop();
  deinit();
}