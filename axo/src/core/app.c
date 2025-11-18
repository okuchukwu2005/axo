#include "../../include/core/app.h"

// core
#include "../../include/core/theme.h"
// widgets
#include"../../include/widgets/container.h"
#include"../../include/widgets/entry.h"
#include"../../include/widgets/drop.h"
#include"../../include/widgets/radio.h"
#include"../../include/widgets/textbox.h"
#include"../../include/widgets/slider.h"
#include"../../include/widgets/button.h"
#include"../../include/widgets/text.h"
#include"../../include/widgets/progress.h"
#include"../../include/widgets/image.h"

Font_ttf* global_font = NULL;

App init_app(void) {
    App app = {0}; // Initialize struct members to zero

    // Set default theme (e.g., dark mode)
    set_theme(&THEME_DARK);

    return app;
}

void app_quit(){
	
}
int is_any_text_widget_active(void) {
    // Check entries
    for (int i = 0; i < entrys_count; i++) {
        if (entry_widgets[i] && entry_widgets[i]->is_active) {
            return 1;
        }
    }
    // Check textboxes
    for (int i = 0; i < textboxs_count; i++) {
        if (textbox_widgets[i] && textbox_widgets[i]->is_active) {
            return 1;
        }
    }
    return 0;
}

void app_run_(axParent *parent) {
    axEvent event;
    int running = 1;
    while (running) {
        while (poll_event(&event)) {
            if (event.type == EVENT_QUIT) {
                running = 0;
            } else {
                // === THEME SWITCHING (F1–F10) ===
                if (event.type == EVENT_KEYDOWN) {
                    switch (event.key.key) {
                        case KEY_F1:  set_theme(&THEME_LIGHT); break;
                        case KEY_F2:  set_theme(&THEME_DARK); break;
                        case KEY_F3:  set_theme(&THEME_HACKER); break;
                        case KEY_F4:  set_theme(&THEME_IMGUI_DARK); break;
                        case KEY_F5:  set_theme(&THEME_NUKLEAR); break;
                        case KEY_F6:  set_theme(&THEME_MATERIAL_DARK); break;
                        case KEY_F7:  set_theme(&THEME_DRACULA); break;
                        case KEY_F8:  set_theme(&THEME_NORD); break;
                        case KEY_F9:  set_theme(&THEME_SOLARIZED_DARK); break;
                        case KEY_F10: set_theme(&THEME_WIN95); break;
                    }
                }

                // === UPDATE ALL WIDGETS ===
                axUpdateAllRegisteredContainers(&event);
                axUpdateAllRegisteredRadioButtons(&event);
                axUpdateAllRegisteredEntries(&event);
            	axUpdateAllRegisteredTextBoxes(&event);
                axUpdateAllRegisteredSliders(&event);
                axUpdateAllRegisteredButtons(&event);
                axUpdateAllRegisteredTexts(&event);     // ← Added
                axUpdateAllRegisteredDropDown(&event);
                axUpdateAllRegisteredProgressBars(&event);
                axUpdateAllRegisteredImages(&event);

                // Text input management
                if (is_any_text_widget_active()) {
                    SDL_StartTextInput();
                } else {
                    SDL_StopTextInput();
                }
            }
        }

        // === RENDER ===
        clear_screen(&parent->base, parent->color);
        axRenderAllRegisteredContainers();
        axRenderAllRegisteredDropDown();
        axRenderAllRegisteredRadioButtons();
        axRenderAllRegisteredEntries();
        axRenderAllRegisteredTextBoxes();
        axRenderAllRegisteredSliders();
        axRenderAllRegisteredButtons();
        axRenderAllRegisteredTexts();
        axRenderAllRegisteredProgressBars();
        axRenderAllRegisteredImages();
        present_screen(&parent->base);
    }

    // === CLEANUP ===
    axFreeAllRegisteredImages();
    axFreeAllRegisteredButtons();
    axFreeAllRegisteredDropDown();
    axFreeAllRegisteredRadioButtons();
    axFreeAllRegisteredTexts();
    axFreeAllRegisteredContainers();
    axFreeAllRegisteredEntries();
    axFreeAllRegisteredProgressBars();
    axFreeAllRegisteredSliders();
    axFreeAllRegisteredTextBoxes();  // ← Match name above

    app_quit();
    free_parent(parent);
}
