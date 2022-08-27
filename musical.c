#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "chord_finder.h"
#include "raylib.h"
#include "timer.h"
#include "util.h"

#define NOTE_NULL INT_MAX

typedef union {
  int i;
} Arg;

typedef struct {
  Rectangle rect;
  void (*left_click_func)(Arg *arg);
  void (*right_click_func)(Arg *arg);
  Arg arg;
} Button;

typedef struct {
  KeyboardKey key;
  void (*pressed_func)(Arg *arg);
  void (*down_func)(Arg *arg);
  Arg arg;
} Key;

void clear_notes(Arg *arg);
void down_note(Arg *arg);
void mark_note(Arg *arg);
void play_note(Arg *arg);

// Configurable global variables
#include "config.h"

bool *down_notes;
bool *marked_notes;
Timer *pressed_notes;
int marked_notes_count;
bool clicked;

void clear_notes(Arg *arg) {
  for (int i = 0; i < LENGTH(marked_notes); i++) marked_notes[i] = false;
  marked_notes_count = 0;
}

void down_note(Arg *arg) { down_notes[arg->i] = true; }

void mark_note(Arg *arg) {
  marked_notes[arg->i] = !marked_notes[arg->i];
  if (marked_notes[arg->i])
    marked_notes_count++;
  else
    marked_notes_count--;
}

void play_note(Arg *arg) {
  if (IsKeyDown(KEY_LEFT_CONTROL)) mark_note(arg);
  start_timer(&pressed_notes[arg->i], pressed_note_duration);
}

void handle_keyboard_input(void) {
  for (int i = 0; i < LENGTH(keys); i++)
    if (IsKeyPressed(keys[i].key))
      keys[i].pressed_func(&keys[i].arg);
    else if (IsKeyDown(keys[i].key) && keys[i].down_func)
      keys[i].down_func(&keys[i].arg);
}

void run(void) {
  screen.x = GetWindowPosition().x;
  screen.y = GetWindowPosition().y;
  screen.width = GetScreenWidth();
  screen.height = GetScreenHeight();
  piano.x = screen.width / 2 - piano.width / 2;
  piano.y = screen.height / 2 - piano.height / 2 + 20;
  clicked = false;
  for (int i = 0; i < LENGTH(down_notes); i++) down_notes[i] = false;
  handle_keyboard_input();
  BeginDrawing();
  ClearBackground(COLOR_BACKGROUND);
  DrawRectangleRounded(piano, 0.3f, 10, COLOR_PIANO);
  EndDrawing();
}

int main(void) {
  down_notes = calloc(last_note, sizeof(bool));
  marked_notes = calloc(last_note, sizeof(bool));
  pressed_notes = calloc(last_note, sizeof(Timer));
  SetTraceLogLevel(LOG_WARNING);
  InitWindow(screen.width, screen.height, program_title);
  SetTargetFPS(target_fps);
  while (!WindowShouldClose()) run();
  CloseWindow();
  free(down_notes);
  free(marked_notes);
  free(pressed_notes);
  return EXIT_SUCCESS;
}
