#include "commands.h"

int main(int argc, char *argv[]) {

  int sockfd;
  char **cookies = calloc(1, sizeof(char *)),
       *token = calloc(BUFLEN, sizeof(char)),
       *command = calloc(BUFLEN, sizeof(char));
  cookies[0] = calloc(BUFLEN, sizeof(char));

  while (1) {
    fflush(stdin);
    printf("Put your command: ");
    fgets(command, BUFLEN, stdin);
    remove_newline(command);
    sockfd = open_connection(IP, PORT, AF_INET, SOCK_STREAM, 0);

    if (!strcmp(command, "register")) {
      register_form(sockfd);
      continue;
    }

    if (!strcmp(command, "login")) {
      login(sockfd, cookies);
      continue;
    }

    if (!strcmp(command, "enter_library")) {
      enter_library(sockfd, cookies, token);
      continue;
    }

    if (!strcmp(command, "get_books")) {
      get_books(sockfd, token);
      continue;
    }

    if (!strcmp(command, "get_book")) {
      get_book(sockfd, token);
      continue;
    }

    if (!strcmp(command, "add_book")) {
      add_book(sockfd, token);
      continue;
    }

    if (!strcmp(command, "delete_book")) {
      delete_book(sockfd, token);
      continue;
    }

    if (!strcmp(command, "logout")) {
      logout(sockfd, cookies, token);
      continue;
    }

    if (!strcmp(command, "exit"))
      exit_client(sockfd);

    puts("Invalid command!");
  }
}
