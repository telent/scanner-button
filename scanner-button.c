#include <sane/sane.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int find_option_index(SANE_Handle scanner_handle, char *name)
{
  const SANE_Option_Descriptor * odesc;
  SANE_Int options_index;

  options_index = 0;
  while(odesc = sane_get_option_descriptor(scanner_handle, options_index)) {
    if(!strcmp(odesc->name, name))
      return options_index;
    options_index++;
  }
  return -1;
}

struct button {
  char *name;
  int index;
};

int main(int argc, char *argv[]) {
  char * scanner = getenv ("SCANNER");
  if(scanner == NULL) {
    fprintf(stderr, "Expecting a SCANNER environment variable\n");
    exit(1);
  }

  SANE_Status status;
  status = sane_init(NULL, NULL);
  if(status != SANE_STATUS_GOOD) {
    fprintf(stderr, "sane_init: %s\n", sane_strstatus(status));
    exit(1);
  }

  SANE_Handle scanner_handle;
  status = sane_open(scanner, &scanner_handle);
  if(status != SANE_STATUS_GOOD) {
    fprintf(stderr, "sane_open: %s\n", sane_strstatus(status));
    exit(1);
  }

  int wait_until_pressed = 0;
  if(argc > 1 && !strcmp(argv[1], "--wait")) {
    argc--;
    argv++;
    wait_until_pressed = 1;
  }

  struct button * buttons = calloc(sizeof (struct button), argc);
  int i=1, buttons_count=0;
  char *name;
  while(i < argc) {
    name = argv[i];
    int b;
    if ((b = find_option_index(scanner_handle, name)) >= 0) {
      buttons[buttons_count].name = name;
      buttons[buttons_count].index = b;
      buttons_count++;
    } else {
      fprintf(stderr, "Button name '%s' not recognized by backend\n", name);
      exit(1);
    }
    i++;
  }

  int iterations = 0, finished = 0;
  while(! finished) {
    for(i=0;i< buttons_count;i++) {
      SANE_Bool is_set = 0;
      sane_control_option(scanner_handle, buttons[i].index,
			  SANE_ACTION_GET_VALUE,
			  (void *) &is_set,
			  NULL);
      if(is_set) {
	finished = 1;
	printf("%s ", buttons[i].name);
      }
    }
    if(iterations < 2)
      /* there is (as far as i can tell) a bug in the sane fujitsu
	 backend such that it will always report 0/false the first
	 time you call sane_control_option for any given option.  So,
	 workaround is to run it twice at minimum */
      iterations++;
    if(wait_until_pressed) {
      usleep(1000 * 50);
    } else {
      if(iterations > 1)
	finished = 1;
    }
  }
  printf("\n");
}
