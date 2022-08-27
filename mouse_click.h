#define MOUSE_CLICK_HANDLER                                          \
  do {                                                               \
    if (!clicked &&                                                  \
        CheckCollisionPointRec(GetMousePosition(), buttons[i].rect)) \
      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {                 \
        buttons[i].left_click_func(&buttons[i].arg);                 \
        clicked = true;                                              \
      } else if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {             \
        down_note(&(Arg){i});                                        \
        clicked = true;                                              \
      } else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {         \
        buttons[i].right_click_func(&buttons[i].arg);                \
        clicked = true;                                              \
      }                                                              \
  } while (0)
