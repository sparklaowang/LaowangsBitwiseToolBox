


#include "memprintf.h"


int main()
{
  uint8_t buf[4] = {0, 1, 2, 3};

  char str[1024];

  memprintf(str, 1024, "0: %8d 1:%8d  3:%16d", buf, 4);

  printf("%s",str);
  int i = 0;

  return 0;
}
