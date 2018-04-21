#include <stdio.h>
#include <conio.h>

void main(void)
{
  while (1)
  {
    unsigned char key = cgetc();
    printf("key = %u\n", key);
  }
}
