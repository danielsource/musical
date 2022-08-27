#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "chord_finder.h"
#include "mouse_click.h"
#include "raylib.h"
#include "timer.h"
#include "util.h"

#define BLACK_KEYS 5
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

bool is_black_key[12] = {
    [1] = true, [3] = true, [6] = true, [8] = true, [10] = true,
};

Button *buttons;
bool *down_notes;
bool *marked_notes;
Timer *pressed_notes;
int marked_notes_count;
bool clicked;

void clear_notes(Arg *arg) {
  for (int i = first_note; i <= last_note; i++) marked_notes[i] = false;
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

void update_piano(void) {
  int note_total = last_note - first_note + 1;
  int carry = MOD(note_total, 12);
  int octaves = (note_total - carry) / 12;
  int black_keys = BLACK_KEYS * octaves;
  if (carry)
    for (int i = last_note - carry + 1; i <= last_note; i++)
      if (is_black_key[i % 12]) black_keys++;
  int white_keys = note_total - black_keys;

  piano.x = screen.width / 2 - piano.width / 2;
  piano.y = screen.height / 2 - piano.height / 2 + 20;
  clicked = false;
  Rectangle keyboard = {piano.x + 10, piano.y + piano.height / 2 - 5,
                        piano.width - 20, piano.height / 2 + 5};
  Rectangle white_key = {keyboard.x, keyboard.y, keyboard.width / white_keys,
                         keyboard.height};
  DrawRectangleRounded(piano, 0.15f, 30, COLOR_PIANO);
  DrawRectangleRec(keyboard, GREEN);

  Color text_color = Fade(BLACK, 0.2f);
  int text_size = 30;
  char *text = program_title;
  DrawText(text, piano.x + piano.width / 2 - MeasureText(text, text_size) / 2,
           piano.y + 3, text_size, text_color);

  for (int i = first_note; i <= last_note; i++) {
    Rectangle *key = &buttons[i].rect;
    key->y = white_key.y;
    if (is_black_key[i % 12]) {
      key->width = white_key.width / 2;
      key->height = white_key.height / 1.7f;
      key->x = white_key.x - key->width / 2;
      MOUSE_CLICK_HANDLER;
      continue;
    }
    key->x = white_key.x;
    key->width = white_key.width;
    key->height = white_key.height;
    DrawRectangleRec(buttons[i].rect, COLOR_WHITE);
    DrawRectangle(key->x, key->y, 1, key->height, LIGHTGRAY);
    DrawRectangle(key->x + key->width - 1, key->y, 1, key->height, LIGHTGRAY);
    white_key.x += white_key.width;
  }
  for (int i = first_note; i <= last_note; i++) {
    if (!is_black_key[i % 12])
      MOUSE_CLICK_HANDLER;
    else
      DrawRectangleRec(buttons[i].rect, COLOR_BLACK);
  }
}

void run(void) {
  screen.x = GetWindowPosition().x;
  screen.y = GetWindowPosition().y;
  screen.width = GetScreenWidth();
  screen.height = GetScreenHeight();
  for (int i = first_note; i <= last_note; i++) down_notes[i] = false;
  handle_keyboard_input();
  update_piano();
  BeginDrawing();
  ClearBackground(COLOR_BACKGROUND);
  EndDrawing();
}

int main(void) {
  buttons = calloc(last_note + 1, sizeof(Button));
  down_notes = calloc(last_note + 1, sizeof(bool));
  marked_notes = calloc(last_note + 1, sizeof(bool));
  pressed_notes = calloc(last_note + 1, sizeof(Timer));
  for (int i = 0; i < LENGTH(keys); i++)
    if (keys[i].pressed_func == play_note) keys[i].down_func = down_note;
  SetTraceLogLevel(LOG_WARNING);
  InitWindow(screen.width, screen.height, program_title);
  SetTargetFPS(target_fps);
  while (!WindowShouldClose()) run();
  CloseWindow();
  free(buttons);
  free(down_notes);
  free(marked_notes);
  free(pressed_notes);
  return EXIT_SUCCESS;
}
