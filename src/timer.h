typedef struct Timer {
  double start;
  double lifetime;
} Timer;

double get_elapsed(Timer *t);
bool is_timer_done(Timer *t);
void start_timer(Timer *t, double lifetime);
