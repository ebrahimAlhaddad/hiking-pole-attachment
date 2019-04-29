#ifndef _GUI_H_INCLUDED
#define _GUI_H_INCLUDED

#define BTN_RADIUS 30

// STATE CONSTANTS
#define INITIAL 0
#define HOME    1
#define OTHER   2  // In a screen other than Home

void gui_display_home(void);
void gui_display_gps(void);
void gui_display_sos(void);
void gui_display_compass(void);
void gui_display_pulse(void);

void gui_loop(void);

#endif
