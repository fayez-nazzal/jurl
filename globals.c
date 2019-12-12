#include <time.h>
void delay(float seconds) {
  int time = (int)(CLOCKS_PER_SEC * seconds);

  clock_t start_time = clock();
  while (clock() < start_time + time )
   ;
}
