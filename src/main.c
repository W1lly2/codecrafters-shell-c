#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_COMMAND_LENGTH 100

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  char command[MAX_COMMAND_LENGTH];
  char *path = getenv("PATH");
  char path_copy[4096];

  if (path == NULL) {
      return 0;
  }

  while (1) {
      printf("$ ");

      fgets(command, sizeof(command), stdin);
      command[strcspn(command, "\n")] = '\0';

      char *builtin = strtok(command, " ");
      char *arg = strtok(NULL, "");

      if (builtin == NULL) {
          continue;
      }

      if (strcmp(builtin, "exit") == 0) {
          break;
      }

      else if (strcmp(builtin, "echo") == 0) {
          printf("%s\n", arg);
      }

      else if (strcmp(builtin, "type") == 0) {

          if (!strcmp(arg, "exit") ||
              !strcmp(arg, "echo") ||
              !strcmp(arg, "type")) {

              printf("%s is a shell builtin\n", arg);
          }

          else {
              strcpy(path_copy, path);

              int found = 0;

              char *directory = strtok(path_copy, ":");

              while (directory != NULL) {
                  char full_path[4096];

                  snprintf(
                      full_path,
                      sizeof(full_path),
                      "%s/%s",
                      directory,
                      arg
                  );

                  if (access(full_path, X_OK) == 0) {
                      printf("%s is %s\n", arg, full_path);
                      found = 1;
                      break;
                  }

                  directory = strtok(NULL, ":");
              }

              if (!found) {
                  printf("%s: not found\n", arg);
              }
          }
      }

      else {
          printf("%s: command not found\n", builtin);
       }
  }

  return 0;
}