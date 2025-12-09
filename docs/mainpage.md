@mainpage Axo GUI Framework Documentation

# Mainpage

Welcome to the official documentation of the **Axo** project --- a
lightweight, immediate mode GUI framework written in C.

Axo provides: - A flexible widget system
- A simple event, update, render loop
- Multiple backend implementations
- Extensible core components

Use the sections below to navigate the documentation.

------------------------------------------------------------------------

# Widgets

All widgets are organized into groups.
Click any link to browse all available widgets, functions, and
structures.

-   @ref group_button "Buttons"
-   @ref group_window "Windows"
-   @ref group_label "Labels"
-   @ref group_slider "Sliders"
-   @ref group_checkbox "Checkboxes"
-   @ref group_container "Containers"

------------------------------------------------------------------------

# Core

Core components provide low-level functionality such as parent windows,
color, event structures, rendering utilities, and initialization
helpers.

See:

-   @ref group_core_init "Backend interface"
-   @ref group_core_events "Color"
-   @ref group_core_render "parent"
-   @ref group_core_render "Debug"
-   @ref group_core_render "theme"

------------------------------------------------------------------------

# Backends

Backend modules implement platform-specific functionality and are
intended for developers and maintainers.

Examples:

-   @ref group_backend_sdl "SDL2 Backend"
-   @ref group_backend_win32 "SDL3 Backend"

------------------------------------------------------------------------

# Example: Basic Window + Button

``` c
#include <axo/axo.h>

int main(void)
{
    axInit();

    axParent *win = axCreateWindow("My Window", 1000, 700, WIN_RESIZABLE);

    axButton *button = axCreateButton(
        win,                // parent container
        100, 400,           // x, y
        120, 40,            // width, height
        "Click Me",         // label
        OVERRIDE            // flags
    );
    axRegisterButton(button);

    axEvent event;
    int running = 1;

    while (running)
    {
        while (poll_event(&event))
        {
            if (event.type == EVENT_QUIT)
                running = 0;

            // === UPDATE ALL WIDGETS ===
            axUpdateAllRegisteredButtons(&event);

            // === RENDER ===
            clear_screen(&win->base, win->color);
            axRenderAllRegisteredButtons();
            present_screen(&win->base);
        }
    }

    // === CLEANUP ===
    axFreeAllRegisteredButtons();

    return 0;
}
```

------------------------------------------------------------------------
