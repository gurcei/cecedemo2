#include <stdio.h>
#include <conio.h>

void main(void)
{
  // switch back to upper-case
  // https://www.cc65.org/mailarchive/2004-09/4446.html
  *(unsigned char *)0xd018 = 0x15;

  while (1)
  {
    printf("hello world!\n");
    //unsigned char key = cgetc();
    //printf("key = %u\n", key);
  }
}
