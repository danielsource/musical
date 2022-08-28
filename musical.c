// TODO: add sounds
// TODO: portuguese/english language support

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"

#include "chord_finder.h"
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
} Keybinding;

void clear_marked_notes(Arg *arg);
void down_note(Arg *arg);
void mark_note(Arg *arg);
void play_note(Arg *arg);

// Preprocessor macro to handle mouse input
#include "mouse_click.h"

// Configurable global variables
#include "config.h"

bool is_black_key[12] = {
    [1] = true, [3] = true, [6] = true, [8] = true, [10] = true,
};

Button *buttons;
Color *key_colors;
Rectangle keyboard;
Timer *pressed_notes;
int marked_notes_count;
bool *down_notes, *marked_notes;
bool help_pressed_notes = true, help_clear_marked_notes = true,
     help_mark_marked_notes = false;

void clear_marked_notes(Arg *arg) {
  for (int i = first_note; i <= last_note; i++) marked_notes[i] = false;
  marked_notes_count = 0;
  help_clear_marked_notes = false;
}

void down_note(Arg *arg) { down_notes[arg->i] = true; }

void mark_note(Arg *arg) {
  marked_notes[arg->i] = !marked_notes[arg->i];
  if (marked_notes[arg->i])
    marked_notes_count++;
  else
    marked_notes_count--;
  help_mark_marked_notes = false;
}

void play_note(Arg *arg) {
  static int count = 0;
  if (++count == 3) {
    help_pressed_notes = false;
    help_mark_marked_notes = true;
  }
  if (IsKeyDown(KEY_LEFT_CONTROL)) mark_note(arg);
  start_timer(&pressed_notes[arg->i], pressed_note_duration);
}

void handle_keyboard_input(void) {
  for (int i = 0; i < LENGTH(keybindings); i++)
    if (IsKeyPressed(keybindings[i].key))
      keybindings[i].pressed_func(&keybindings[i].arg);
    else if (IsKeyDown(keybindings[i].key) && keybindings[i].down_func)
      keybindings[i].down_func(&keybindings[i].arg);
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
  piano.y = screen.height / 2 - piano.height / 2;
  keyboard.x = piano.x + 10;
  keyboard.y = piano.y + piano.height / 2 - 5;
  keyboard.width = piano.width - 20;
  keyboard.height = piano.height / 2 + 5;
  clicked = false;
  Rectangle white_key = {keyboard.x, keyboard.y, keyboard.width / white_keys,
                         keyboard.height};

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
    white_key.x += white_key.width;
  }
  for (int i = first_note; i <= last_note; i++) {
    if (!is_black_key[i % 12]) {
      MOUSE_CLICK_HANDLER;
      key_colors[i] = COLOR_WHITE;
      if (!is_timer_done(&pressed_notes[i]))
        key_colors[i] = COLOR_PIANO_WHITE_KEY_PRESSED;
      else if (marked_notes[i])
        key_colors[i] = COLOR_PIANO_WHITE_KEY_MARKED;
      else if (down_notes[i])
        key_colors[i] = COLOR_PIANO_WHITE_KEY_DOWN;
    } else {
      key_colors[i] = COLOR_BLACK;
      if (!is_timer_done(&pressed_notes[i]))
        key_colors[i] = COLOR_PIANO_BLACK_KEY_PRESSED;
      else if (marked_notes[i])
        key_colors[i] = COLOR_PIANO_BLACK_KEY_MARKED;
      else if (down_notes[i])
        key_colors[i] = COLOR_PIANO_BLACK_KEY_DOWN;
    }
  }
}

void draw_piano(void) {
  char chord_name[64] = {0};
  char *chord_detail = NULL;
  int intervals[10];
  int j = 0;
  for (int i = first_note; i <= last_note && j < LENGTH(intervals); i++)
    if ((marked_notes_count && marked_notes[i]) ||
        ((!marked_notes_count && down_notes[i])))
      intervals[j++] = i;
  if (j > 0) {
    get_chord_name(chord_name, LENGTH(chord_name), intervals, j);
    for (char *s = chord_name; *s; s++) {
      if (*s == '\n') {
        *s = '\0';
        chord_detail = ++s;
        break;
      }
    }
    DrawText(chord_name, piano.x, (screen.height / 2 - piano.height / 2) - 48,
             20, COLOR_FOREGROUND);
    if (chord_detail)
      DrawText(chord_detail, piano.x,
               (screen.height / 2 - piano.height / 2) - 26, 20,
               COLOR_FOREGROUND);
  }

  DrawRectangleRounded(piano, 0.15f, 30, COLOR_PIANO);
  DrawRectangleRec(keyboard, GREEN);

  Color text_color = Fade(BLACK, 0.2f);
  int text_size = 30;
  char *text = program_title;
  DrawText(text, piano.x + piano.width / 2 - MeasureText(text, text_size) / 2,
           piano.y + 3, text_size, text_color);
  text_size = 10;

  if (help_pressed_notes) {
    text = "Use o teclado do computador ou o mouse para tocar o";
    DrawText(text, piano.x, piano.y + piano.height + 5, text_size,
             Fade(COLOR_FOREGROUND, 0.35f));
    text = "teclado musical";
    DrawText(text, piano.x, piano.y + piano.height + 17, text_size,
             Fade(COLOR_FOREGROUND, 0.35f));
  } else if (marked_notes_count && help_clear_marked_notes) {
    text = "Aperte barra de espaço para limpar as notas marcadas";
    DrawText(text, piano.x, piano.y + piano.height + 5, text_size,
             Fade(COLOR_FOREGROUND, 0.35f));
  } else if (!marked_notes_count && help_mark_marked_notes) {
    text = "Segure a tecla Ctrl enquanto toca as teclas ou clique com o";
    DrawText(text, piano.x, piano.y + piano.height + 5, text_size,
             Fade(COLOR_FOREGROUND, 0.35f));
    text = "botão direito para marcar notas";
    DrawText(text, piano.x, piano.y + piano.height + 17, text_size,
             Fade(COLOR_FOREGROUND, 0.35f));
  }

  for (int i = first_note; i <= last_note; i++) {
    if (is_black_key[i % 12]) continue;
    Rectangle *key = &buttons[i].rect;
    DrawRectangleRec(buttons[i].rect, key_colors[i]);
    DrawRectangle(key->x, key->y, 1, key->height,
                  (Color){0xac, 0xac, 0xac, 0xff});
    DrawRectangle(key->x + key->width - 1, key->y, 1, key->height,
                  (Color){0xac, 0xac, 0xac, 0xff});
    DrawRectangle(key->x, key->y, 1, key->height, Fade(key_colors[i], 0.5f));
    DrawRectangle(key->x + key->width - 1, key->y, 1, key->height,
                  Fade(key_colors[i], 0.5f));
  }
  for (int i = first_note; i <= last_note; i++) {
    if (!is_black_key[i % 12]) continue;
    DrawRectangleRec(buttons[i].rect, key_colors[i]);
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
  draw_piano();
  EndDrawing();
}

int main(void) {
  buttons = calloc(last_note + 1, sizeof(Button));
  key_colors = calloc(last_note + 1, sizeof(Color));
  down_notes = calloc(last_note + 1, sizeof(bool));
  marked_notes = calloc(last_note + 1, sizeof(bool));
  pressed_notes = calloc(last_note + 1, sizeof(Timer));
  for (int i = 0; i < LENGTH(keybindings); i++)
    if (keybindings[i].pressed_func == play_note)
      keybindings[i].down_func = down_note;
  for (int i = first_note; i <= last_note; i++) {
    buttons[i].left_click_func = play_note;
    buttons[i].right_click_func = mark_note;
    buttons[i].arg.i = i;
  }
  SetTraceLogLevel(LOG_WARNING);
  InitWindow(screen.width, screen.height, program_title);
  SetTargetFPS(target_fps);
  while (!WindowShouldClose()) run();
  CloseWindow();
  free(buttons);
  free(key_colors);
  free(down_notes);
  free(marked_notes);
  free(pressed_notes);
  return EXIT_SUCCESS;
}
