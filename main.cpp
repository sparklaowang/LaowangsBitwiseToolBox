


#include "memprintf.h"


int main()
{
  uint8_t buf[4] = {0, 1, 2, 0xFF};

  char str[1024];

  memprintf(str, 1024, "0: %7d %1n 1:%8d  3:%16d", buf, 4);

  printf("%s",str);

  memprintf(str, 1024, "0: %8d 1a:%4d 1b:%4d, 3:%8d, 4%1n, %7d", buf, 4);

  printf("%s",str);

  int i = 0;

  return 0;
}
