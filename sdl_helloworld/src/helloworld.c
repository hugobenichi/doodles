#include <unistd.h>

#include "SDL2/SDL.h"

#define null NULL

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
  if (0 != ret) return ret;

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
  SDL_RenderSetLogicalSize(c -> renderer, c -> width, c -> height);

  return 0;
}

int load_assets(struct Ctx* c) {
  SDL_Surface* object = SDL_LoadBMP( "lena512.bmp" );
  if (!object) return -1;

  c -> texture = SDL_CreateTextureFromSurface(c -> renderer, object);
  if (!c -> texture) return -1;

  // TODO: release 'object'
  return 0;
}

int draw_frame(struct Ctx* c) {
  int ret;
  ret = SDL_RenderClear(c -> renderer);
  if (0 != ret) return -1;
  // parametrize this: tecture assets need to live in entity structs that hold the calculation of blit rectangles
  ret = SDL_RenderCopy(c -> renderer, c -> texture, null, null);
  if (0 != ret) return -1;
  SDL_RenderPresent(c -> renderer);

  return 0;
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

  ret = draw_frame(&c);
  if (0 != ret) goto err;

  usleep(6000);

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
