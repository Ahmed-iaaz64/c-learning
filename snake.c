#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

const int window_width = 800;
const int window_height = 800;
const int number_of_cells = 20;

typedef struct {
    int x;
    int y;
} vec2;

struct snake {
    vec2 pos;
    struct snake *next;
};

struct food {
    vec2 pos;
};

void add_snake_node(struct snake *head) {
	struct snake *current = head;
	while (current != NULL) {
		if (current->next == NULL) {
            struct snake *new_node = malloc(sizeof(struct snake));
            vec2 position = current->pos;
            new_node->pos = position;
            new_node->next = NULL;
			current->next = new_node;
			break;
	    }
		current = current->next;
	}
}

void move_snake(struct snake *snake_head, vec2 direction) {
    // move body first (back to front)
    struct snake *current = snake_head->next;
    vec2 prev_pos = snake_head->pos;
    while (current != NULL) {
        vec2 current_pos = current->pos;
        current->pos = prev_pos;
        prev_pos = current_pos;
        current = current->next;
    }
    
    // then move head
    snake_head->pos.x += direction.x;
    snake_head->pos.y += direction.y;
}

int snake_collides_with_self_check(struct snake *snake_head) {
    vec2 snake_head_pos = snake_head->pos;

    struct snake *current = snake_head;
    current = current->next;
    while (current != NULL) {
        if (current->pos.x == snake_head_pos.x &&
            current->pos.y == snake_head_pos.y) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

int snake_out_of_bounds_check(struct snake *snake_head) {
    vec2 snake_head_pos = snake_head->pos;

    if (snake_head_pos.x < 0 || snake_head_pos.y < 0 ||
        snake_head_pos.x >= number_of_cells || snake_head_pos.y >= number_of_cells) {
        return 1;
    }
    return 0;
}

void draw_snake(struct snake *snake_head, SDL_Renderer *renderer) {
    int cell_size = window_width / number_of_cells;
    SDL_SetRenderDrawColor(renderer, 40, 255, 40, 255);

    struct snake *current = snake_head;
    while (current != NULL) {
        SDL_Rect snake_cell = {current->pos.x * cell_size, 
                               current->pos.y * cell_size, 
                               cell_size, cell_size};
        SDL_RenderDrawRect(renderer, &snake_cell);
        current = current->next;
    }
}

int food_snake_collision_check(struct snake *snake_head, struct food *food) {
    vec2 snake_head_pos = snake_head->pos;
    vec2 food_pos = food->pos;

    if (snake_head_pos.x == food_pos.x && snake_head_pos.y == food_pos.y) {
        return 1;
    }
    return 0;
}

void update_food_pos(struct food *food) {
    int min = 1;
    int max = number_of_cells - 1;
    vec2 food_pos = {rand() % (max - min + 1) + min, 
                     rand() % (max - min + 1) + min};

    food->pos = food_pos;
}

void draw_food(struct food *food, SDL_Renderer *renderer) {
    int cell_size = window_width / number_of_cells;
    SDL_SetRenderDrawColor(renderer, 255, 40, 40, 255);

    SDL_Rect food_cell = {food->pos.x * cell_size, 
                          food->pos.y * cell_size, 
                          cell_size, cell_size};
    SDL_RenderDrawRect(renderer, &food_cell);
}

void draw_score(int score, TTF_Font *font, SDL_Renderer *renderer) {
    char score_text[20];
    sprintf(score_text, "%d", score);

    SDL_Color text_color = {255, 255, 255, 255};
    SDL_Surface *text_surface = TTF_RenderText_Solid(font, score_text, text_color);
    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

    if (text_texture == NULL) {
        printf("%s\n", SDL_GetError());
    }
    
    int text_width = text_surface->w;
    int text_height = text_surface->h;

    SDL_Rect render_quad = {window_width - text_width - 10, 10, text_width, text_height};
    SDL_RenderCopy(renderer, text_texture, NULL, &render_quad);

    printf("%d %d\n", text_width, text_height);
    SDL_FreeSurface(text_surface);
    SDL_DestroyTexture(text_texture);
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    srand(time(NULL));
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSerif-Bold.ttf", 24);
    if (font == NULL) {
        printf("Font failed to load: %s\n", TTF_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Snake", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        window_width, window_height, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Surface *window_surface;
    SDL_Event ev;

    Uint32 last_move_time = SDL_GetTicks();
    const Uint32 move_delay = 160;

    // snake initialization
    vec2 start_pos = {10, 10};
    struct snake *snake_head = malloc(sizeof(struct snake));
    snake_head->pos = start_pos;
    snake_head->next = NULL;
    add_snake_node(snake_head);

    // food initialization
    struct food *food = malloc(sizeof(struct food));
    update_food_pos(food);

    // game logic variables
    int is_running = 1;
    int moved = 0;
    vec2 direction = {-1, 0};
    int score = 0;

    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    } else {
        while (is_running) {
            while (SDL_PollEvent(&ev) != 0) {
                if (ev.type == SDL_QUIT) {
                    is_running = 0;
                }
                else if (ev.type == SDL_KEYDOWN) {
                    switch (ev.key.keysym.sym) {
                        case SDLK_w:
                            if (direction.y != 1 && !moved) {
                                direction.x = 0;
                                direction.y = -1;
                                moved = 1;
                            }
                            break;

                        case SDLK_a:
                            if (direction.x != 1 && !moved) {
                                direction.x = -1;
                                direction.y = 0;
                                moved = 1;
                            }
                            break;

                        case SDLK_s:
                            if (direction.y != -1 && !moved) {
                                direction.x = 0;
                                direction.y = 1;
                                moved = 1;
                            }
                            break;

                        case SDLK_d:
                            if (direction.x != -1 && !moved) {
                                direction.x = 1;
                                direction.y = 0;
                                moved = 1;
                            }
                            break;
                    }
                }
            }
            window_surface = SDL_GetWindowSurface(window);

            // clear black screen
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            // draw grid lines to dark grey
            SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
            int cell_size = window_width / number_of_cells;
            for (int x = 0; x < number_of_cells; x++) {
                for (int y = 0; y < number_of_cells; y++) {
                    SDL_Rect cell = {x * cell_size, 
                                     y * cell_size, 
                                     cell_size, cell_size};
                    SDL_RenderDrawRect(renderer, &cell);
                }
            }

            // update game at fixed speed
            Uint32 current_time = SDL_GetTicks();
            if (current_time - last_move_time >= move_delay) {
                move_snake(snake_head, direction);
                last_move_time = SDL_GetTicks();
                moved = 0;

                // collision checks
                is_running = !snake_collides_with_self_check(snake_head) &&
                             !snake_out_of_bounds_check(snake_head);
                if (food_snake_collision_check(snake_head, food)) {
                    update_food_pos(food);
                    add_snake_node(snake_head);
                    score++;
                };
            }

            // draw everything
            draw_snake(snake_head, renderer);
            draw_food(food, renderer);
            draw_score(score, font, renderer);

            SDL_RenderPresent(renderer);
        }
        SDL_DestroyWindow(window);
    }

    SDL_Quit();
    return 0;
}

