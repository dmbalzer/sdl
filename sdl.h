#ifndef SDL_INCLUDED
#define SDL_INCLUDED

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <stdbool.h>

int sdl_init(const char* title, int w, int h);
int sdl_init_font(const char* fn, int pt_size);
void sdl_process_events(void);
void sdl_begin_drawing(void);
void sdl_end_drawing(void);
void sdl_unload(void);
bool sdl_window_closed(void);
void sdl_set_clear_color(SDL_Color color);
SDL_Texture* sdl_load_texture(const char* fn);
void sdl_unload_texture(SDL_Texture* t);
void sdl_blit(SDL_Texture* t, int x, int y);
void sdl_blit_rect(SDL_Texture* t, int x, int y, SDL_Rect src);
SDL_Point sdl_get_mousepos(void);
bool sdl_is_mouse_pressed(int button);
void sdl_draw_text(const char* text, int x, int y);
float sdl_get_frametime(void);
void sdl_draw_rect(SDL_Rect r, SDL_Color c);
void sdl_draw_rect_lines(SDL_Rect r, SDL_Color c);

#endif // SDL_INCLUDED
