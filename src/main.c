#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static inline void print_invalid(void) {
  printf(PROJECT_NAME "\n"
    "Usage:\n"
    "  " PROJECT_NAME " <argument> \n\n\n"
    "Argument:\n"
    "  help         Show this screen.\n"
    "  version      Show version.\n"
    "\n"
  );
}
static inline void print_help_usage_message(void) {
  printf(PROJECT_NAME "\n"
    "Usage:\n"
    "  " PROJECT_NAME " <argument> \n\n\n"
    "Argument:\n"
    "  help         Show this screen.\n"
    "  version      Show version.\n"
    "\n"
  );
}
static inline void print_version_message(void) {
  printf(PROJECT_NAME " version " PROJECT_VERSION "\n");
}
static inline int parsing_arguments(int argc, const char **argv) {
  if (argc < 2) {
    printf(PROJECT_NAME " should have an argument");
  } else if (!strcmp(argv[1], "help")) {
  } else if (!strcmp(argv[1], "benchmark")) {
    return 0;
  } else if (!strcmp(argv[1], "version")) {
    print_version_message();
    return 1;
  } else {
    printf(PROJECT_NAME " unknown argument of %s", argv[1]);
  }
  print_help_usage_message();
  return 1;
}


int main(int argc, const char **argv) {
  // parsing arguments
  if (parsing_arguments(argc, argv)) {
    return 0;
  }
  // do benchmark

  return 0;
}