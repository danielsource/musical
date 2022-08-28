typedef struct {
  char *signature;
  char *name;
  int root;
} Chord;

extern const Chord chords[];
extern const char *const notes[];

void get_chord_name(char *dest, int dest_len, int *intervals, int len);
