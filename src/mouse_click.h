static bool clicked;
static Button *last_button;

#define MOUSE_CLICK_HANDLER                                                   \
  do {                                                                        \
    if (last_button &&                                                        \
        !CheckCollisionPointRec(GetMousePosition(), last_button->rect))       \
      last_button = &(Button){.arg.i = NOTE_NULL};                            \
    if (!clicked &&                                                           \
        CheckCollisionPointRec(GetMousePosition(), buttons[i].rect)) {        \
      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {                          \
        buttons[i].left_click_func(&buttons[i].arg);                          \
        last_button = &buttons[i];                                            \
        clicked = true;                                                       \
      } else if (buttons[i].left_click_func == play_note &&                   \
                 IsMouseButtonDown(MOUSE_LEFT_BUTTON) &&                      \
                 (!last_button ||                                             \
                  (last_button && last_button->arg.i == buttons[i].arg.i))) { \
        down_note(&(Arg){i});                                                 \
        last_button = &buttons[i];                                            \
        clicked = true;                                                       \
      } else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {                  \
        buttons[i].right_click_func(&buttons[i].arg);                         \
        last_button = &buttons[i];                                            \
        clicked = true;                                                       \
      }                                                                       \
    }                                                                         \
  } while (0)
