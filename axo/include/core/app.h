#ifndef APP_H
#define APP_H

#include "parent.h"
#include "backend_interface.h"

#define PRINT_THEME(name) printf("Switched to %s\n", name)
//------------------------- APP ___________
extern Font_ttf* global_font;
typedef struct {
    axParent window; // Window struct from window.h
} App;

App init_app(void);

int is_any_text_widget_active(void);

void app_run_(axParent *parent);

#endif // APP_H
