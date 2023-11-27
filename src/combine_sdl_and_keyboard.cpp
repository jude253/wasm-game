// Copyright 2011 The Emscripten Authors.  All rights reserved.
// Emscripten is available under two separate licenses, the MIT license and the
// University of Illinois/NCSA Open Source License.  Both these licenses can be
// found in the LICENSE file.

#include <stdio.h>
#include <SDL2/SDL.h>

#include <emscripten.h>
#include <emscripten/key_codes.h>
#include <emscripten/html5.h>

SDL_Surface *screen = NULL;


// void draw() {

//   if (SDL_MUSTLOCK(screen)) SDL_LockSurface(screen);
//   int r = rand() % 256;
//   int g = rand() % 256;
//   int b = rand() % 256;
//   for (int i = 0; i < 256; i++) {
//     for (int j = 0; j < 256; j++) {
//       int alpha = 255;
//       *((Uint32*)screen->pixels + i * 256 + j) = SDL_MapRGBA(screen->format, r, g, b, alpha);
//     }
//   }
//   if (SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
//   SDL_Flip(screen); 
// }

static inline const char *emscripten_event_type_to_string(int eventType) {
  const char *events[] = { "(invalid)", "(none)", "keypress", "keydown", "keyup", "click", "mousedown", "mouseup", "dblclick", "mousemove", "wheel", "resize", 
    "scroll", "blur", "focus", "focusin", "focusout", "deviceorientation", "devicemotion", "orientationchange", "fullscreenchange", "pointerlockchange", 
    "visibilitychange", "touchstart", "touchend", "touchmove", "touchcancel", "gamepadconnected", "gamepaddisconnected", "beforeunload", 
    "batterychargingchange", "batterylevelchange", "webglcontextlost", "webglcontextrestored", "mouseenter", "mouseleave", "mouseover", "mouseout", "(invalid)" };
  ++eventType;
  if (eventType < 0) eventType = 0;
  if (eventType >= sizeof(events)/sizeof(events[0])) eventType = sizeof(events)/sizeof(events[0])-1;
  return events[eventType];
}


EM_BOOL mouse_callback(int eventType, const EmscriptenMouseEvent *e, void *userData) {
  printf("%s, screen: (%d,%d), client: (%d,%d),%s%s%s%s button: %hu, buttons: %hu, movement: (%d,%d), target: (%d, %d)\n",
    emscripten_event_type_to_string(eventType), e->screenX, e->screenY, e->clientX, e->clientY,
    e->ctrlKey ? " CTRL" : "", e->shiftKey ? " SHIFT" : "", e->altKey ? " ALT" : "", e->metaKey ? " META" : "", 
    e->button, e->buttons, e->movementX, e->movementY, e->targetX, e->targetY);

  if (e->screenX != 0 && e->screenY != 0 && e->clientX != 0 && e->clientY != 0 && e->targetX != 0 && e->targetY != 0) {
    if (eventType == EMSCRIPTEN_EVENT_CLICK) printf("gotClick = 1\n");
    if (eventType == EMSCRIPTEN_EVENT_MOUSEDOWN && e->buttons != 0) printf("gotClick = 1\n");
    if (eventType == EMSCRIPTEN_EVENT_DBLCLICK) printf("gotClick = 1\n");
    if (eventType == EMSCRIPTEN_EVENT_MOUSEUP) printf("gotClick = 1\n");
    if (eventType == EMSCRIPTEN_EVENT_MOUSEMOVE && (e->movementX != 0 || e->movementY != 0)) printf("gotMouseMove = 1\n");
  }
  // draw();

  return 0;
}

EM_BOOL key_callback(int eventType, const EmscriptenKeyboardEvent *e, void *userData)
{
  int dom_pk_code = emscripten_compute_dom_pk_code(e->code);

  printf("%s, key: \"%s\" \n",
    emscripten_event_type_to_string(eventType), 
    e->key
  );
  if (eventType == EMSCRIPTEN_EVENT_KEYUP) printf("\n"); // Visual cue

  // draw();
  // Return true for events we want to suppress default web browser handling for.
  // For testing purposes, want to return false here on most KeyDown messages so that they get transformed to KeyPress messages.
  return e->keyCode == DOM_VK_BACK_SPACE // Don't navigate away from this test page on backspace.
    || e->keyCode == DOM_VK_TAB // Don't change keyboard focus to different browser UI elements while testing.
    || (e->keyCode >= DOM_VK_F1 && e->keyCode <= DOM_VK_F24) // Don't F5 refresh the test page to reload.
    || e->ctrlKey // Don't trigger e.g. Ctrl-B to open bookmarks
    || e->altKey // Don't trigger any alt-X based shortcuts either (Alt-F4 is not overrideable though)
    || eventType == EMSCRIPTEN_EVENT_KEYPRESS || eventType == EMSCRIPTEN_EVENT_KEYUP; // Don't perform any default actions on these.
}


int main(int argc, char** argv) {
  printf("hello, world!\n");
  // emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, key_callback);
  // emscripten_set_click_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, mouse_callback);
  // srand (time(NULL));
  
  // SDL_Init(SDL_INIT_VIDEO);
  // screen = SDL_SetVideoMode(256, 256, 32, SDL_SWSURFACE);

  
  // draw();
  

  // SDL_Quit();

  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *window;
  SDL_Renderer *renderer;

  SDL_CreateWindowAndRenderer(600, 400, 0, &window, &renderer);

  SDL_Surface *screen = SDL_CreateRGBSurface(0, 600, 400, 8, 0, 0, 0, 0);

  //initialize sdl palette
  //with red green and blue
  //colors
  SDL_Color pal[4];
  pal[0].r = 255;
  pal[0].g = 0;
  pal[0].b = 0;
  pal[0].a = 255;

  pal[1].r = 0;
  pal[1].g = 255;
  pal[1].b = 0;
  pal[1].a = 255;

  pal[2].r = 0;
  pal[2].g = 0;
  pal[2].b = 255;
  pal[2].a = 255;

  pal[3].r = 255;
  pal[3].g = 255;
  pal[3].b = 0;
  pal[3].a = 255;

  SDL_SetPaletteColors(screen->format->palette, pal, 0, 4);

  SDL_FillRect(screen, NULL, 0);

  {
    SDL_Rect rect = { 300, 0, 300, 200 };
    SDL_FillRect(screen, &rect, 1);
  }

  {
    SDL_Rect rect = { 0, 200, 600, 200 };
    SDL_FillRect(screen, &rect, 2);
  }

  {
    SDL_Rect rect = { 300, 200, 300, 200 };
    SDL_FillRect(screen, &rect, 3);
  }

  SDL_Texture *screenTexture = SDL_CreateTextureFromSurface(renderer, screen);
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, screenTexture, NULL, NULL);
  SDL_RenderPresent(renderer);

  printf("you should see red, blue and yellow rectangles\n");

  // Don't quit - we need to reftest the canvas! SDL_Quit();

  return 0;
}

