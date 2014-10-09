#include <stdio.h>
#include <stdlib.h>

#include <readline/readline.h>
#include <readline/history.h>

int main(int argc, char** argv)
{
  printf("toooooos\n", stdout);
  printf("^c to exit\n", stdout);

  for(;;) {
    char * input = readline("tos> ");

    add_history(input);

    puts(input);

    free(input);
  }

  return 0;
}
