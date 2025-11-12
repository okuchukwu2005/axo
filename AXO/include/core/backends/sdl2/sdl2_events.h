#ifndef SDL2_EVENTS_H
#define SDL2_EVENTS_H

#include <SDL2/SDL.h>
#include <string.h>

// ======================
// EVENT TYPES
// ======================
typedef enum {
    EVENT_KEYDOWN,
    EVENT_KEYUP,
    EVENT_TEXTINPUT,
    EVENT_MOUSEMOTION,
    EVENT_MOUSEBUTTONDOWN,
    EVENT_MOUSEBUTTONUP,
    EVENT_MOUSEWHEEL,
    EVENT_WINDOWRESIZED,
    EVENT_WINDOWCLOSE,
    EVENT_QUIT
} EventType;

// ======================
// KEY & MOUSE BUTTONS
// ======================
typedef enum {
    KEY_UNKNOWN = 0,
    KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J,
    KEY_K, KEY_L, KEY_M, KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T,
    KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,
    KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,
    KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8,
    KEY_F9, KEY_F10, KEY_F11, KEY_F12,
    KEY_ESCAPE, KEY_RETURN, KEY_SPACE, KEY_TAB, KEY_BACKSPACE, KEY_DELETE,
    KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN,
    KEY_LSHIFT, KEY_RSHIFT, KEY_LCTRL, KEY_RCTRL, KEY_LALT, KEY_RALT,
    KEY_LGUI, KEY_RGUI,
    /* ---- NEW KEYS --------------------------------------------------- */
    KEY_HOME,                     // SDL_SCANCODE_HOME
    KEY_END                       // SDL_SCANCODE_END
} Key;

typedef enum {
    MOUSE_LEFT = 1,
    MOUSE_MIDDLE = 2,
    MOUSE_RIGHT = 3,
    MOUSE_X1 = 4,
    MOUSE_X2 = 5
} MouseButton;

// ======================
// MOUSE BUTTON MASKS
// ======================
#define MOUSE_BUTTON_LEFT_MASK   (1u << (MOUSE_LEFT - 1))
#define MOUSE_BUTTON_MIDDLE_MASK (1u << (MOUSE_MIDDLE - 1))
#define MOUSE_BUTTON_RIGHT_MASK  (1u << (MOUSE_RIGHT - 1))
#define MOUSE_BUTTON_X1_MASK     (1u << (MOUSE_X1 - 1))
#define MOUSE_BUTTON_X2_MASK     (1u << (MOUSE_X2 - 1))

// ======================
// KEY MODIFIER MASKS (SDL-compatible)
// ======================
#define KEY_MOD_NONE  0x0000
#define KEY_MOD_SHIFT 0x0003  // LSHIFT | RSHIFT
#define KEY_MOD_CTRL  0x00C0  // LCTRL  | RCTRL
#define KEY_MOD_ALT   0x0300  // LALT   | RALT
#define KEY_MOD_GUI   0x0C00  // LGUI   | RGUI

// ======================
// EVENT STRUCT
// ======================
#define EVENT_TEXT_MAX 32
typedef struct {
    EventType type;
    union {
        struct { Key key; int repeat; Uint16 mod; } key;
        struct { char text[EVENT_TEXT_MAX]; } text;
        struct { int x, y; int dx, dy; Uint32 button_state; } mouseMove;
        struct { MouseButton button; int x, y; int clicks; } mouseButton;
        struct { int dx, dy; } mouseWheel;
        struct { int width, height; } windowResized;
    };
} Event;

// ======================
// FUNCTION DECLARATIONS
// ======================
Key translate_sdl_key(SDL_Scancode sc);
int translate_sdl_event(const SDL_Event *s, Event *out);
int poll_event(Event *out);
void enable_text_input(void);
void disable_text_input(void);

#endif // SDL2_EVENTS_H
