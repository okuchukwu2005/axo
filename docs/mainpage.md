# Main Page

# Axo GUI Framework Library (C)

Welcome to the **GUI Framework Library** --- a lightweight, modular, and
extensible graphical user interface framework designed for C
applications.

## Overview

This library provides:

-   A platform‑independent abstraction layer for windowing and
    rendering\
-   A flexible event system supporting callbacks and custom event types\
-   A widget toolkit including buttons, labels, layouts, and containers\
-   A lightweight rendering backend with pluggable drivers\
-   Tools for building responsive, dynamic GUI applications in pure C

## Getting Started

### Initialization

To begin using the library, call the initialization function:

``` c
gui_init();
```

### Creating a Window

``` c
gui_window_t* window = gui_window_create(800, 600, "My GUI App");
gui_window_show(window);
```

### Main Loop

``` c
while (gui_window_is_open(window)) {
    gui_poll_events();
    gui_render(window);
}
```

## Features

### Widget Hierarchy

Widgets can be nested using layout managers:

-   Horizontal layout
-   Vertical layout
-   Grid layout
-   Custom layout providers

### Event System

Supports:

-   Mouse events
-   Keyboard events
-   Focus/blur
-   Custom events

Register callbacks:

``` c
gui_button_set_on_click(btn, on_button_clicked);
```

## Directory Structure

    /src
        core/        # Core types and memory systems
        events/      # Event dispatcher and handlers
        widgets/     # Built‑in widgets
        render/      # Rendering backends
    /include
        gui/         # Public headers

## Documentation

Run the following to generate full documentation:

    doxygen Doxyfile

## License

This project is released under the MIT License.
