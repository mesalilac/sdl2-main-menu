#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define HEXCOLOR(code)                                                                                                 \
    ((code) >> (3 * 8)) & 0xFF, ((code) >> (2 * 8)) & 0xFF, ((code) >> (1 * 8)) & 0xFF, ((code) >> (0 * 8)) & 0xFF

#define WIDTH 800
#define HEIGHT 600
#define FONT_NAME "CascadiaCode.ttf"

#define BACKGROUND_COLOR 0x212121FF
#define TEXT_COLOR 255, 255, 255, 255
#define TEXT_COLOR_SELECTED 255, 0, 0, 255

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        fprintf(stderr, "ERROR: Failed to init sdl video: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_Window *win = SDL_CreateWindow("Main menu", 0, 0, WIDTH, HEIGHT, 0);
    if (win == NULL)
    {
        fprintf(stderr, "ERROR: Failed to create window: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (ren == NULL)
    {
        fprintf(stderr, "ERROR: Failed to create renderer: %s\n", SDL_GetError());
        exit(1);
    }

    if (TTF_Init() == -1)
    {
        fprintf(stderr, "ERROR: Failed to init TTF: %s\n", SDL_GetError());
        exit(1);
    }

    TTF_Font *font = TTF_OpenFont(FONT_NAME, 20);
    if (font == NULL)
    {
        fprintf(stderr, "ERROR: Failed to load font: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_Event event;
    bool is_running = true;
    SDL_Color font_color_normal = {TEXT_COLOR};
    SDL_Color font_color_selected = {TEXT_COLOR_SELECTED};

    char **list = malloc(100 * sizeof(char));
    int list_count = 0;

    list[list_count] = "text1";
    list_count++;
    list[list_count] = "text2";
    list_count++;
    list[list_count] = "text3";
    list_count++;

    int cursor = 0;
    bool show_menu = true;
    int menu_max_x = 400;
    int menu_max_y = 400;
    int menu_starting_x = 100;
    int menu_starting_y = 100;
    int menu_padding = 10;

    while (is_running)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                is_running = false;
                break;
            case SDL_KEYDOWN:
                if (show_menu && event.key.keysym.sym == 'j' && cursor != list_count - 1)
                    cursor++;
                if (show_menu && event.key.keysym.sym == 'k' && cursor != 0)
                    cursor--;
                if (show_menu && event.key.keysym.sym == SDLK_RETURN)
                    printf("%s\n", list[cursor]);
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    show_menu = !show_menu;
                break;
            }
        }

        SDL_SetRenderDrawColor(ren, HEXCOLOR(BACKGROUND_COLOR));
        SDL_RenderClear(ren);

        // -- RENDER

        if (show_menu)
        {
            // Draw border
            {
                SDL_SetRenderDrawColor(ren, 10, 10, 10, 255);
                SDL_RenderDrawLine(ren, menu_starting_x, menu_starting_y, menu_max_x,
                                   menu_starting_y);                                                    // -----------
                SDL_RenderDrawLine(ren, menu_starting_x, menu_starting_y, menu_starting_x, menu_max_y); // |
                SDL_RenderDrawLine(ren, menu_starting_x, menu_max_y, menu_max_x, menu_max_y);           // |----------
                SDL_RenderDrawLine(ren, menu_max_x, menu_starting_y, menu_max_x, menu_max_y);           //           |
            }

            {
                for (int i = 0; i < list_count; ++i)
                {
                    bool currently_selected = cursor == i;

                    SDL_Surface *surface = TTF_RenderText_Solid(
                        font, list[i], currently_selected ? font_color_selected : font_color_normal);
                    SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, surface);

                    SDL_SetRenderDrawColor(ren, HEXCOLOR(BACKGROUND_COLOR));
                    SDL_Rect message_rect = {
                        .x = menu_padding + menu_starting_x,
                        .y = menu_padding + menu_starting_y + i * surface->h,
                        .w = surface->w,
                        .h = surface->h,
                    };

                    SDL_RenderCopy(ren, texture, NULL, &message_rect);
                    SDL_RenderDrawRect(ren, &message_rect);

                    SDL_FreeSurface(surface);
                    SDL_DestroyTexture(texture);
                }
            }
        }

        // RENDER --

        SDL_RenderPresent(ren);
    }

    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(ren);
    SDL_Quit();
}
