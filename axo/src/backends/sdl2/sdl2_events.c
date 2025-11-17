#include "../../../include/backends/sdl2/sdl2_events.h"
#include <SDL2/SDL.h>
#include <string.h>

/* --------------------------------------------------------------------- */
/* ORIGINAL KEY TRANSLATION (unchanged)                                 */
/* --------------------------------------------------------------------- */
Key translate_sdl_key(SDL_Scancode sc) {
    switch (sc) {
        case SDL_SCANCODE_A: return KEY_A;
        case SDL_SCANCODE_B: return KEY_B;
        case SDL_SCANCODE_C: return KEY_C;
        case SDL_SCANCODE_D: return KEY_D;
        case SDL_SCANCODE_E: return KEY_E;
        case SDL_SCANCODE_F: return KEY_F;
        case SDL_SCANCODE_G: return KEY_G;
        case SDL_SCANCODE_H: return KEY_H;
        case SDL_SCANCODE_I: return KEY_I;
        case SDL_SCANCODE_J: return KEY_J;
        case SDL_SCANCODE_K: return KEY_K;
        case SDL_SCANCODE_L: return KEY_L;
        case SDL_SCANCODE_M: return KEY_M;
        case SDL_SCANCODE_N: return KEY_N;
        case SDL_SCANCODE_O: return KEY_O;
        case SDL_SCANCODE_P: return KEY_P;
        case SDL_SCANCODE_Q: return KEY_Q;
        case SDL_SCANCODE_R: return KEY_R;
        case SDL_SCANCODE_S: return KEY_S;
        case SDL_SCANCODE_T: return KEY_T;
        case SDL_SCANCODE_U: return KEY_U;
        case SDL_SCANCODE_V: return KEY_V;
        case SDL_SCANCODE_W: return KEY_W;
        case SDL_SCANCODE_X: return KEY_X;
        case SDL_SCANCODE_Y: return KEY_Y;
        case SDL_SCANCODE_Z: return KEY_Z;
        case SDL_SCANCODE_0: return KEY_0;
        case SDL_SCANCODE_1: return KEY_1;
        case SDL_SCANCODE_2: return KEY_2;
        case SDL_SCANCODE_3: return KEY_3;
        case SDL_SCANCODE_4: return KEY_4;
        case SDL_SCANCODE_5: return KEY_5;
        case SDL_SCANCODE_6: return KEY_6;
        case SDL_SCANCODE_7: return KEY_7;
        case SDL_SCANCODE_8: return KEY_8;
        case SDL_SCANCODE_9: return KEY_9;
        case SDL_SCANCODE_F1: return KEY_F1;
        case SDL_SCANCODE_F2: return KEY_F2;
        case SDL_SCANCODE_F3: return KEY_F3;
        case SDL_SCANCODE_F4: return KEY_F4;
        case SDL_SCANCODE_F5: return KEY_F5;
        case SDL_SCANCODE_F6: return KEY_F6;
        case SDL_SCANCODE_F7: return KEY_F7;
        case SDL_SCANCODE_F8: return KEY_F8;
        case SDL_SCANCODE_F9: return KEY_F9;
        case SDL_SCANCODE_F10: return KEY_F10;
        case SDL_SCANCODE_F11: return KEY_F11;
        case SDL_SCANCODE_F12: return KEY_F12;
        case SDL_SCANCODE_ESCAPE: return KEY_ESCAPE;
        case SDL_SCANCODE_RETURN: return KEY_RETURN;
        case SDL_SCANCODE_SPACE: return KEY_SPACE;
        case SDL_SCANCODE_TAB: return KEY_TAB;
        case SDL_SCANCODE_BACKSPACE: return KEY_BACKSPACE;
        case SDL_SCANCODE_DELETE: return KEY_DELETE;
        case SDL_SCANCODE_LEFT: return KEY_LEFT;
        case SDL_SCANCODE_RIGHT: return KEY_RIGHT;
        case SDL_SCANCODE_UP: return KEY_UP;
        case SDL_SCANCODE_DOWN: return KEY_DOWN;
        case SDL_SCANCODE_LSHIFT: return KEY_LSHIFT;
        case SDL_SCANCODE_RSHIFT: return KEY_RSHIFT;
        case SDL_SCANCODE_LCTRL: return KEY_LCTRL;
        case SDL_SCANCODE_RCTRL: return KEY_RCTRL;
        case SDL_SCANCODE_LALT: return KEY_LALT;
        case SDL_SCANCODE_RALT: return KEY_RALT;
        case SDL_SCANCODE_LGUI: return KEY_LGUI;
        case SDL_SCANCODE_RGUI: return KEY_RGUI;
        case SDL_SCANCODE_HOME: return KEY_HOME;
        case SDL_SCANCODE_END:  return KEY_END;
        default: return KEY_UNKNOWN;
    }
}

/* --------------------------------------------------------------------- */
/* ORIGINAL EVENT TRANSLATION (unchanged)                                */
/* --------------------------------------------------------------------- */
int translate_sdl_event(const SDL_Event *s, Event *out) {
    if (!s || !out) return 0;

    switch (s->type) {
        case SDL_KEYDOWN:
            out->type = EVENT_KEYDOWN;
            out->key.key = translate_sdl_key(s->key.keysym.scancode);
            out->key.repeat = s->key.repeat;
            out->key.mod = s->key.keysym.mod;
            return 1;

        case SDL_KEYUP:
            out->type = EVENT_KEYUP;
            out->key.key = translate_sdl_key(s->key.keysym.scancode);
            out->key.repeat = s->key.repeat;
            out->key.mod = s->key.keysym.mod;
            return 1;

        case SDL_TEXTINPUT:
            out->type = EVENT_TEXTINPUT;
            strncpy(out->text.text, s->text.text, EVENT_TEXT_MAX - 1);
            out->text.text[EVENT_TEXT_MAX - 1] = '\0';
            return 1;

        case SDL_MOUSEMOTION:
            out->type = EVENT_MOUSEMOTION;
            out->mouseMove.x = s->motion.x;
            out->mouseMove.y = s->motion.y;
            out->mouseMove.dx = s->motion.xrel;
            out->mouseMove.dy = s->motion.yrel;
            out->mouseMove.button_state = s->motion.state;
            return 1;

        case SDL_MOUSEBUTTONDOWN:
            out->type = EVENT_MOUSEBUTTONDOWN;
            out->mouseButton.button = (MouseButton)s->button.button;
            out->mouseButton.x = s->button.x;
            out->mouseButton.y = s->button.y;
            out->mouseButton.clicks = s->button.clicks;
            return 1;

        case SDL_MOUSEBUTTONUP:
            out->type = EVENT_MOUSEBUTTONUP;
            out->mouseButton.button = (MouseButton)s->button.button;
            out->mouseButton.x = s->button.x;
            out->mouseButton.y = s->button.y;
            out->mouseButton.clicks = s->button.clicks;
            return 1;

        case SDL_MOUSEWHEEL:
            out->type = EVENT_MOUSEWHEEL;
            out->mouseWheel.dx = s->wheel.x;
            out->mouseWheel.dy = s->wheel.y;
            return 1;

        case SDL_WINDOWEVENT:
            if (s->window.event == SDL_WINDOWEVENT_RESIZED) {
                out->type = EVENT_WINDOWRESIZED;
                out->windowResized.width = s->window.data1;
                out->windowResized.height = s->window.data2;
                return 1;
            }
            if (s->window.event == SDL_WINDOWEVENT_CLOSE) {
                out->type = EVENT_WINDOWCLOSE;
                return 1;
            }
            break;

        case SDL_QUIT:
            out->type = EVENT_QUIT;
            return 1;

        default:
            break;
    }
    return 0;
}

/* --------------------------------------------------------------------- */
/* ORIGINAL POLLING (unchanged)                                          */
/* --------------------------------------------------------------------- */
int poll_event(Event *out) {
    SDL_Event s;
    while (SDL_PollEvent(&s)) {
        if (translate_sdl_event(&s, out)) {
            return 1;
        }
    }
    return 0;
}

void enable_text_input(void)  { SDL_StartTextInput(); }
void disable_text_input(void) { SDL_StopTextInput(); }

/* --------------------------------------------------------------------- */
/* NEW INPUT HELPERS (implemented here)                                 */
/* --------------------------------------------------------------------- */

void input_get_mouse(int *x, int *y)
{
    SDL_GetMouseState(x, y);
}

bool input_mouse_down(MouseButton btn)
{
    Uint32 state = SDL_GetMouseState(NULL, NULL);
    return (state & SDL_BUTTON((int)btn)) != 0;
}

Uint16 input_get_mod_state(void)
{
    return SDL_GetModState();
}

/* --------------------------------------------------------------------- */
/* NEW CLIPBOARD HELPERS (implemented here)                             */
/* --------------------------------------------------------------------- */

bool clipboard_has_text(void)
{
    return SDL_HasClipboardText();
}

char* clipboard_get_text(void)
{
    return SDL_GetClipboardText();   /* SDL allocates */
}

void clipboard_set_text(const char *text)
{
    SDL_SetClipboardText(text);
}

void clipboard_free(char *text)
{
    if (text) SDL_free(text);
}
