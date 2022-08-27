typedef struct {
  char *signature;
  char *name;
} Chord;

extern const Chord chords[];

void get_chord_name(char *dest, int dest_size, char **intervals, int len);
