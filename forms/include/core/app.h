#ifndef APP_H
#define APP_H

#include "parent.h"

#define PRINT_THEME(name) printf("Switched to %s\n", name)
//------------------------- APP ___________
typedef struct {
    Parent window; // Window struct from window.h
} App;

App init_app(void);

int is_any_text_widget_active(void);

void app_run_(Parent *parent);

#endif // APP_H