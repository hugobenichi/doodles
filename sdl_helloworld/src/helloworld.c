#include <unistd.h>

#include "SDL2/SDL.h"

#define null NULL

#define _ERR printf("error at %s:%d", __FILE__, __LINE__)

// TODO: move to timeutil
#define MILLI_IN_MICRO * 1000
#define SEC_IN_MICRO * 1000 MILLI_IN_MICRO
#define MIN_IN_MICRO * 60 SEC_IN_MICRO
#define SEC_IN_MILLIS * 1000
#define MIN_IN_MILLIS * 60 SEC_IN_MILLIS

struct Ctx {
  // configuration
  int width;
  int height;

  // stateful context
  SDL_Window*   screen;
  SDL_Renderer* renderer;

  // assets
  // should load textures from a keyed table of file names.
  SDL_Texture*  texture;
};

int init_ctx(struct Ctx* c) {
  int ret = SDL_CreateWindowAndRenderer(c -> width, c -> height, SDL_WINDOW_OPENGL, &(c->screen), &(c->renderer));
  if (0 != ret) { _ERR; return ret; }

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
  SDL_RenderSetLogicalSize(c -> renderer, c -> width, c -> height);

  return 0;
}

int load_assets(struct Ctx* c) {
  SDL_Surface* object = SDL_LoadBMP( "lena512.bmp" );
  if (!object) { _ERR; return -1; }

  c -> texture = SDL_CreateTextureFromSurface(c -> renderer, object);
  if (!c -> texture) { _ERR; return -1; }

  // TODO: release 'object'
  return 0;
}

int draw_frame(struct Ctx* c, SDL_Rect* rec) {
  int ret;
  ret = SDL_RenderClear(c -> renderer);
  if (0 != ret) { _ERR; return -1; }
  // parametrize this: tecture assets need to live in entity structs that hold the calculation of blit rectangles
  ret = SDL_RenderCopy(c -> renderer, c -> texture, rec, rec);
  if (0 != ret) { _ERR; return -1; }
  SDL_RenderPresent(c -> renderer);

  return 0;
}

void advance_rec(SDL_Rect* rec, int step, int wrap_x, int wrap_y) {
  rec -> x += step;
  if (rec -> x < wrap_x) return;
  rec -> x = 0;
  rec -> y += step;
  if (rec -> y < wrap_y) return;
  rec -> y = 0;
}

void draw_something() {
  struct Ctx c;
  c.width  = 512;
  c.height = 512;

  int ret;

  ret = init_ctx(&c);
  if (0 != ret) goto err;

  ret = load_assets(&c);
  if (0 != ret) goto err;

  puts("Ready to draw");

  int step = 512 / 4;

  SDL_Rect rec = (SDL_Rect) {
    .x = 0,
    .y = 0,
    .w = step,
    .h = step,
  };

  int total_time_us = 5 SEC_IN_MICRO;
  int time_step_us = 50 MILLI_IN_MICRO;

  int count_down = total_time_us / time_step_us;
  while (count_down--) {
    printf("count_down = %d\n", count_down);
    ret = draw_frame(&c, &rec);
    if (0 != ret) goto err;
    usleep(time_step_us);
    advance_rec(&rec, step, c.width, c.height);
  }

  usleep(1 SEC_IN_MICRO);

  return;
err:
  puts(SDL_GetError());
  return;
}

int main(int argc, char* args[]) {
  SDL_Init(SDL_INIT_EVERYTHING);

  draw_something();

  SDL_Quit();
  return 0;
}
