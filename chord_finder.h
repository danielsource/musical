#define CHORD_NAME_LENGTH 64
#define CHORD_PARENTHESIS_LENGTH 64
#define CHORD_SIGNATURE_LENGTH 32

typedef struct {
  char signature[CHORD_SIGNATURE_LENGTH];
  char name[CHORD_NAME_LENGTH];
  char parenthesis[CHORD_PARENTHESIS_LENGTH];
  int root;
} Chord;

extern const Chord chords[LANGUAGE_LAST][74];
extern const char *const notes[LANGUAGE_LAST][12];

bool get_chord_name(Chord *dest, int *intervals, size_t len, Language lang);
