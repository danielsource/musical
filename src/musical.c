// musical: see the chords being played on the piano keyboard

#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "raylib.h"

#include "language.h"
#include "chord_finder.h"
#include "timer.h"
#include "util.h"

#define IS_CTRL_DOWN()                                                        \
  (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL))
#define IS_SHIFT_DOWN()                                                       \
  (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
#define NOTE_NULL INT_MAX

typedef enum TutorialSegment {
  TUTORIAL_NULL = 0,
  TUTORIAL_PRESSED_NOTES,
  TUTORIAL_MARKED_NOTES,
  TUTORIAL_TRANSPOSE_MARKED_NOTES,
  TUTORIAL_TOGGLE_CHORD_NOTATION,
  TUTORIAL_TOGGLE_ACCIDENTAL,
  TUTORIAL_CLEAR_MARKED_NOTES,
  TUTORIAL_FINISH,
} TutorialSegment;

typedef union Arg {
  int i;
  const void *v;
} Arg;

typedef struct Button {
  Rectangle rect;
  void (*left_click_func)(Arg *arg);
  void (*right_click_func)(Arg *arg);
  Arg arg;
} Button;

typedef struct Keybinding {
  KeyboardKey key;
  void (*pressed_func)(Arg *arg);
  void (*down_func)(Arg *arg);
  Arg arg;
} Keybinding;

static void clear_marked_notes(Arg *arg);
static void down_note(Arg *arg);
static void mark_note(Arg *arg);
static void play_note(Arg *arg);
static void print_screen(Arg *arg);
static void toggle_chord_notation(Arg *arg);
static void transpose_notes(Arg *arg);
static void show_tutorial(Arg *arg);
static void cycle_language(Arg *arg);
static void draw_tutorial_pt(void);
static void draw_tutorial_en(void);

// Preprocessor macro to handle mouse input
#include "mouse_click.h"

// Configurable global variables
#include "config.h"

void (*draw_tutorial_funcs[LANGUAGE_LAST])(void) = {
  [PORTUGUESE] = draw_tutorial_pt,
  [ENGLISH] = draw_tutorial_en,
};

bool is_black_key[12] = {
  [1] = true, [3] = true, [6] = true, [8] = true, [10] = true,
};

Button buttons[last_note + 1];
Color key_colors[last_note + 1];
TutorialSegment tutorial_segment;
Rectangle keyboard;
Timer pressed_notes[last_note + 1];
Timer show_tutorial_notice;
bool down_notes[last_note + 1], marked_notes[last_note + 1];
int marked_notes_count;

static inline void change_tutorial(TutorialSegment target) {
  if (tutorial_segment != target)
    return;
  tutorial_segment++;
  tutorial_segment %= TUTORIAL_FINISH + 1;
}

void clear_marked_notes(Arg *arg) {
  UNUSED(arg);
  for (int i = first_note; i <= last_note; i++)
    marked_notes[i] = false;
  marked_notes_count = 0;
  change_tutorial(TUTORIAL_CLEAR_MARKED_NOTES);
}

void down_note(Arg *arg) {
  if (!IS_CTRL_DOWN())
    down_notes[arg->i] = true;
}

void mark_note(Arg *arg) {
  marked_notes[arg->i] = !marked_notes[arg->i];
  if (marked_notes[arg->i])
    marked_notes_count++;
  else
    marked_notes_count--;
  if (tutorial_segment == TUTORIAL_PRESSED_NOTES)
    tutorial_segment = TUTORIAL_MARKED_NOTES;
  change_tutorial(TUTORIAL_MARKED_NOTES);
}

void play_note(Arg *arg) {
  if (IS_CTRL_DOWN()) {
    mark_note(arg);
    return;
  }
  start_timer(&pressed_notes[arg->i], pressed_note_duration);
  change_tutorial(TUTORIAL_PRESSED_NOTES);
}

void print_screen(Arg *arg) {
  UNUSED(arg);
  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  char s[64];
  char datetime[32];
  strftime(datetime, sizeof(datetime), "%Y-%m-%d-%H-%M-%S", tm);
  snprintf(s, LENGTH(s), "%s-%s.%s", program_name, datetime,
           print_screen_file_extension);
  TakeScreenshot(s);
}

void toggle_chord_notation(Arg *arg) {
  UNUSED(arg);
  if (IS_SHIFT_DOWN()) {
    if (accidental == SHARP)
      accidental = FLAT;
    else
      accidental = SHARP;
    change_tutorial(TUTORIAL_TOGGLE_ACCIDENTAL);
  }
  else
    abbreviate_chords = !abbreviate_chords;
  change_tutorial(TUTORIAL_TOGGLE_CHORD_NOTATION);
}

void transpose_notes(Arg *arg) {
  int i = abs(arg->i);
  size_t n = (last_note - first_note - i + 1) * sizeof(bool);
  if (arg->i >= 0) {
    memmove(&marked_notes[first_note + i], &marked_notes[first_note], n);
    memset(&marked_notes[first_note], false, i * sizeof(bool));
  } else {
    memmove(&marked_notes[first_note], &marked_notes[first_note + i], n);
    memset(&marked_notes[last_note - i + 1], false, i * sizeof(bool));
  }
  change_tutorial(TUTORIAL_TRANSPOSE_MARKED_NOTES);
}

void show_tutorial(Arg *arg) {
  UNUSED(arg);
  show_tutorial_notice.lifetime = 0;
  change_tutorial(tutorial_segment = TUTORIAL_NULL);
}

void cycle_language(Arg *arg) {
  abbreviate_chords = false;
  language = MOD(language + arg->i, ABBREVIATED_CHORDS);
  tutorial_segment = TUTORIAL_NULL;
  start_timer(&show_tutorial_notice, show_tutorial_notice_duration);
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
  int black_keys = 5 * octaves;
  if (carry)
    for (int i = last_note - carry + 1; i <= last_note; i++)
      if (is_black_key[i % 12])
        black_keys++;
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

void draw_chord_name(void) {
  Chord chord = {0};
  int intervals[10];
  size_t len = 0;
  for (int i = first_note; i <= last_note && len < LENGTH(intervals); i++)
    if ((marked_notes_count && marked_notes[i]) ||
        ((!marked_notes_count && down_notes[i])))
      intervals[len++] = i;
  if (len < 1)
    return;
  else if (len == 1)
    snprintf(chord.name, CHORD_NAME_LENGTH, "%s",
             notes[language][accidental][intervals[0] % 12]);
  else if (!get_chord_name(&chord, intervals, len,
                           (abbreviate_chords ? ABBREVIATED_CHORDS : language),
                           accidental))
    return;
  char *parenthesis = NULL;
  len = strlen(chord.name);
  for (size_t i = 1; i < len; i++) {
    if (chord.name[i] == '(' && chord.name[i - 1] == ' ') {
      chord.name[i - 1] = '\0';
      parenthesis = &chord.name[i];
      break;
    }
  }
  DrawText(chord.name, piano.x, (screen.height / 2 - piano.height / 2) - 48,
           20, COLOR_FOREGROUND);
  if (parenthesis)
    DrawText(parenthesis, piano.x, (screen.height / 2 - piano.height / 2) - 26,
             20, COLOR_FOREGROUND);
}

void draw_piano(void) {
  DrawRectangleRounded(piano, 0.15f, 30, COLOR_PIANO);

  Color text_color = Fade(COLOR_FOREGROUND, 0.2f);
  char *text = program_name;
  int text_size = 30;
  DrawText(text,
           piano.x + piano.width / 2 -
               (double)MeasureText(text, text_size) / 2,
           piano.y + 3, text_size, text_color);

  for (int i = first_note; i <= last_note; i++) {
    if (is_black_key[i % 12])
      continue;
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
    if (!is_black_key[i % 12])
      continue;
    DrawRectangleRec(buttons[i].rect, key_colors[i]);
  }
}

static inline void draw_tutorial_line(const char *text, int spacing) {
  DrawText(text, piano.x, piano.y + piano.height + 5 + spacing, 10,
           Fade(COLOR_FOREGROUND, 0.35f));
}

void draw_tutorial_pt(void) {
  int line_spacing = 12;
  if (!is_timer_done(&show_tutorial_notice)) {
    draw_tutorial_line("Aperte F1 para mostrar o tutorial.", 0);
    draw_tutorial_line("Press F2 to change the language to English", line_spacing);
  }
  switch (tutorial_segment) {
  case TUTORIAL_NULL:
  case TUTORIAL_FINISH:
    break;
  case TUTORIAL_PRESSED_NOTES:
    draw_tutorial_line("Use o teclado do computador ou o mouse para tocar o", 0);
    draw_tutorial_line("teclado musical.", line_spacing);
    break;
  case TUTORIAL_MARKED_NOTES:
    draw_tutorial_line("Segure a tecla Ctrl enquanto toca as teclas ou clique com o", 0);
    draw_tutorial_line("botão direito para marcar notas.", line_spacing);
    break;
  case TUTORIAL_TRANSPOSE_MARKED_NOTES:
    draw_tutorial_line("Aperte a seta esquerda ou direita para transpor as notas", 0);
    draw_tutorial_line("marcadas.", line_spacing);
    break;
  case TUTORIAL_TOGGLE_CHORD_NOTATION:
    draw_tutorial_line("Aperte Tab para mudar notação dos acordes.", 0);
    break;
  case TUTORIAL_TOGGLE_ACCIDENTAL:
    draw_tutorial_line("Segure Shift e aperte Tab para trocar o acidente (sustenido", 0);
    draw_tutorial_line("e bemol).", line_spacing);
    break;
  case TUTORIAL_CLEAR_MARKED_NOTES:
    draw_tutorial_line("Aperte barra de espaço para limpar as notas marcadas.", 0);
    break;
  }
}

void draw_tutorial_en(void) {
  int line_spacing = 12;
  if (!is_timer_done(&show_tutorial_notice)) {
    draw_tutorial_line("Press F1 to show the tutorial.", 0);
    draw_tutorial_line("Aperte F2 para mudar o idioma para português.", line_spacing);
  }
  switch (tutorial_segment) {
  case TUTORIAL_NULL:
  case TUTORIAL_FINISH:
    break;
  case TUTORIAL_PRESSED_NOTES:
    draw_tutorial_line("Use your computer keyboard or mouse to play the piano", 0);
    draw_tutorial_line("keyboard.", line_spacing);
    break;
  case TUTORIAL_MARKED_NOTES:
    draw_tutorial_line("Hold down the Ctrl key while playing keys or right-click", 0);
    draw_tutorial_line("to mark notes.", line_spacing);
    break;
  case TUTORIAL_TRANSPOSE_MARKED_NOTES:
    draw_tutorial_line("Press the left and right arrows to transpose the marked", 0);
    draw_tutorial_line("notes.", line_spacing);
    break;
  case TUTORIAL_TOGGLE_CHORD_NOTATION:
    draw_tutorial_line("Press Tab to change the chord notation.", 0);
    break;
  case TUTORIAL_TOGGLE_ACCIDENTAL:
    draw_tutorial_line("Hold Shift and press Tab to change the accidental (sharp", 0);
    draw_tutorial_line("and flat).", line_spacing);
    break;
  case TUTORIAL_CLEAR_MARKED_NOTES:
    draw_tutorial_line("Press space bar to clear marked notes.", 0);
    break;
  }
}

void run(void) {
  screen.x = GetWindowPosition().x;
  screen.y = GetWindowPosition().y;
  screen.width = GetScreenWidth();
  screen.height = GetScreenHeight();
  for (int i = first_note; i <= last_note; i++)
    down_notes[i] = false;
  handle_keyboard_input();
  update_piano();
  if (tutorial_segment == TUTORIAL_FINISH) {
    tutorial_segment = TUTORIAL_NULL;
    reset_config();
  } else if (tutorial_segment > TUTORIAL_MARKED_NOTES && !marked_notes_count)
    tutorial_segment = TUTORIAL_MARKED_NOTES;
  BeginDrawing();
  ClearBackground(COLOR_BACKGROUND);
  draw_chord_name();
  draw_piano();
  draw_tutorial_funcs[language]();
  EndDrawing();
}

int main(void) {
  puts("Made by daniel.source (https://github.com/danielsource/musical) with\n"
       "raylib. See more about raylib here: https://www.raylib.com");
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
  if (show_tutorial_notice_duration)
    start_timer(&show_tutorial_notice, show_tutorial_notice_duration);
  while (!WindowShouldClose())
    run();
  CloseWindow();
  return EXIT_SUCCESS;
}
