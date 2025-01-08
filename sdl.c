#include "sdl.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static bool quit = false;
static SDL_Color clear_color = (SDL_Color){ 0,0,0,255 };
static Uint32 mouse_pressed = 0;
static SDL_Texture* glyphs[0xFF] = { 0 };

int sdl_init(const char* title, int w, int h)
{
	if ( SDL_Init(SDL_INIT_VIDEO) != 0 ) { puts(SDL_GetError()); return -1; }
	
	window = SDL_CreateWindow(
			title,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			w,
			h,
			SDL_WINDOW_SHOWN
			);
	if ( window == NULL ) { puts(SDL_GetError()); return -1; }
	
	renderer = SDL_CreateRenderer(
			window,
			-1,
			SDL_RENDERER_PRESENTVSYNC |
			SDL_RENDERER_ACCELERATED
			);
	if ( renderer == NULL ) { puts(SDL_GetError()); return -1; }

	if ( IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == 0 ) { puts(SDL_GetError()); return -1; }
	
	
	return 0;
}
int sdl_init_font(const char* fn)
{

	if ( TTF_Init() != 0 ) { puts(SDL_GetError()); return -1; }
	TTF_Font* font = TTF_OpenFont("assets/GenericMobileSystemNuevo.ttf", 16);
	if ( font == NULL ) { puts(SDL_GetError()); return -1; }
	for ( int i = 0; i < 0xFF; i++ )
	{
		SDL_Surface* s = TTF_RenderGlyph32_Solid(font, i, (SDL_Color){ 0xFF, 0xFF, 0xFF, 0xFF });
		if ( s != NULL ) glyphs[i] = SDL_CreateTextureFromSurface(renderer, s);
		if ( glyphs[i] == NULL ) { puts(SDL_GetError()); }
		SDL_FreeSurface(s);
	}
	TTF_CloseFont(font);
	return 0;
}

void sdl_process_events(void)
{
	SDL_Event event;
	mouse_pressed = 0;
	while ( SDL_PollEvent(&event) )
	{
		switch ( event.type )
		{
			case SDL_QUIT: quit = true; break;
			case SDL_MOUSEBUTTONDOWN: mouse_pressed |= event.button.button; break;
		}
	}
}

void sdl_begin_drawing(void)
{
	SDL_SetRenderDrawColor(
			renderer,
			clear_color.r,
			clear_color.g,
			clear_color.b,
			clear_color.a
			);
	SDL_RenderClear(renderer);
}

void sdl_end_drawing(void)
{
	SDL_RenderPresent(renderer);
}

void sdl_unload(void)
{
	for ( int i = 0; i < 0xFF; i++ ) SDL_DestroyTexture(glyphs[i]);
	TTF_Quit();
	IMG_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

bool sdl_window_closed(void)
{
	return quit;
}

void sdl_set_clear_color(SDL_Color color)
{
	clear_color = color;
}

SDL_Texture* sdl_load_texture(const char* fn)
{
	return IMG_LoadTexture(renderer, fn);
}

void sdl_unload_texture(SDL_Texture* t)
{
	SDL_DestroyTexture(t);
}

void sdl_blit(SDL_Texture* t, int x, int y)
{
	SDL_Rect dest = (SDL_Rect){ x, y, 0, 0};
	SDL_QueryTexture(t, NULL, NULL, &dest.w, &dest.h);
	SDL_RenderCopy(renderer, t, NULL, &dest);
}

SDL_Point sdl_get_mouse_pos(void)
{
	SDL_Point p = { 0 };
	SDL_GetMouseState(&p.x, &p.y);
	return p;
}

bool sdl_is_mouse_pressed(int button)
{
	return mouse_pressed & SDL_BUTTON(button);
}

void sdl_blit_text(const char* text, int x, int y)
{
	int i = 0;
	char c = 0;
	do
	{
		c = text[i++];
		sdl_blit(glyphs[(int)c], x, y);
		int w;
		SDL_QueryTexture(glyphs[(int)c], NULL, NULL, &w, NULL);
		x+=w;	
	}
	while ( c != '\0' );
}
