#define CHORD_NAME_LENGTH 96
#define CHORD_SIGNATURE_LENGTH 32

typedef enum NoteRepresentation {
  SHARP,
  FLAT,
  NOTE_REPRESENTATION_LAST
} NoteRepresentation;

typedef struct Chord {
  char signature[CHORD_SIGNATURE_LENGTH];
  char name[CHORD_NAME_LENGTH];
  int root;
} Chord;

extern const Chord chords[LANGUAGE_LAST][89];
extern const char *const notes[LANGUAGE_LAST][NOTE_REPRESENTATION_LAST][12];

bool get_chord_name(Chord *dest, int *intervals, int len, Language lang, NoteRepresentation r);
