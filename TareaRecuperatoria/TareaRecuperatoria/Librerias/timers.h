typedef struct timer_t timer_t;

timer_t *timers_new(void (* timeout_signal)(void));
bool timers_start(timer_t* timer, uint16_t timeout_cs);
bool timers_start(timer_t* timer);
