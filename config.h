#define COLOR_BACKGROUND        (Color) {0x7c, 0xbf, 0xda, 0xff}
#define COLOR_FOREGROUND        (Color) {0x00, 0x00, 0x00, 0xff}
#define COLOR_WHITE             (Color) {0xff, 0xff, 0xff, 0xff}
#define COLOR_BLACK             (Color) {0x33, 0x33, 0x33, 0xff}
#define COLOR_PIANO             (Color) {0xdb, 0x7a, 0x7a, 0xff}
#define COLOR_PIANO_KEY_DOWN    (Color) {0x33, 0x33, 0x33, 0xff}
#define COLOR_PIANO_KEY_PRESSED (Color) {0x1b, 0xa7, 0xd1, 0xff}
#define COLOR_PIANO_KEY_MARKED  (Color) {0x1b, 0xd1, 0xa0, 0xff}

Key keys[] = {
	{.key = KEY_SPACE, .pressed_func = clear_notes, .arg = {0}},
	{.key = KEY_Q,     .pressed_func = play_note, .arg = {24}},
	{.key = KEY_TWO,   .pressed_func = play_note, .arg = {25}},
	{.key = KEY_W,     .pressed_func = play_note, .arg = {26}},
	{.key = KEY_THREE, .pressed_func = play_note, .arg = {27}},
	{.key = KEY_E,     .pressed_func = play_note, .arg = {28}},
	{.key = KEY_R,     .pressed_func = play_note, .arg = {29}},
	{.key = KEY_FIVE,  .pressed_func = play_note, .arg = {30}},
	{.key = KEY_T,     .pressed_func = play_note, .arg = {31}},
	{.key = KEY_SIX,   .pressed_func = play_note, .arg = {32}},
	{.key = KEY_Y,     .pressed_func = play_note, .arg = {33}},
	{.key = KEY_SEVEN, .pressed_func = play_note, .arg = {34}},
	{.key = KEY_U,     .pressed_func = play_note, .arg = {35}},
	{.key = KEY_I,     .pressed_func = play_note, .arg = {36}},
	{.key = KEY_Z,     .pressed_func = play_note, .arg = {36}},
	{.key = KEY_NINE,  .pressed_func = play_note, .arg = {37}},
	{.key = KEY_S,     .pressed_func = play_note, .arg = {37}},
	{.key = KEY_O,     .pressed_func = play_note, .arg = {38}},
	{.key = KEY_X,     .pressed_func = play_note, .arg = {38}},
	{.key = KEY_ZERO,  .pressed_func = play_note, .arg = {39}},
	{.key = KEY_D,     .pressed_func = play_note, .arg = {39}},
	{.key = KEY_P,     .pressed_func = play_note, .arg = {40}},
	{.key = KEY_C,     .pressed_func = play_note, .arg = {40}},
	{.key = KEY_V,     .pressed_func = play_note, .arg = {41}},
	{.key = KEY_G,     .pressed_func = play_note, .arg = {42}},
	{.key = KEY_B,     .pressed_func = play_note, .arg = {43}},
	{.key = KEY_H,     .pressed_func = play_note, .arg = {44}},
	{.key = KEY_N,     .pressed_func = play_note, .arg = {45}},
	{.key = KEY_J,     .pressed_func = play_note, .arg = {46}},
	{.key = KEY_M,     .pressed_func = play_note, .arg = {47}},
	{.key = KEY_COMMA, .pressed_func = play_note, .arg = {48}},
};

Rectangle piano = {-1, -1, 300, 80};
Rectangle screen = {-1, -1, 330, 150};
char *program_name = "musical";
char *program_title = "Musical";
int first_note = 24; // C4
int last_note = 60;  // C6
short target_fps = 60;
double pressed_note_duration = 0.3f;