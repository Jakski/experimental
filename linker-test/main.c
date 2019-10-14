#include <stdio.h>

void say_hi_a(void);
void say_hi_b(void);

int main(int argc, char **argv)
{
 printf("Hi\n");
 say_hi_a();
 say_hi_b();
 return 0;
}
