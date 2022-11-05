#ifndef COMMANDS_H
#define COMMANDS_H

#include "helpers.h"
#include "parson.h"
#include "requests.h"
#include <arpa/inet.h>
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <unistd.h>     /* read, write, close */

#define IP "34.241.4.235"
#define PORT 8080

#define REGISTER_PATH "/api/v1/tema/auth/register"
#define LOGIN_PATH "/api/v1/tema/auth/login"
#define LIBRARY_PATH "/api/v1/tema/library/access"
#define BOOKS_PATH "/api/v1/tema/library/books"
#define LOGOUT_PATH "/api/v1/tema/auth/logout"

#define PAYLOAD_TYPE "application/json"

#define ONE 1

// Removes newline from a string
void remove_newline(char *str);

// Verifies if the response is an error response
int verify_error(char *response);

// Checks if a string is a non-zero natural number
int is_natural_number(char *str);

// Checks if a string is alphabetical
int is_alphabetic(char *str);

// Register command
void register_form(int sockfd);

// Login command
void login(int sockfd, char **cookies);

// Enter library command
void enter_library(int sockfd, char **cookies, char *token);

// Get books command
void get_books(int sockfd, char *token);

// Get book command
void get_book(int sockfd, char *token);

// Add book command
void add_book(int sockfd, char *token);

// Delete book command
void delete_book(int sockfd, char *token);

// Logout command
void logout(int sockfd, char **cookies, char *token);

// Exit command
void exit_client(int sockfd);

#endif // COMMANDS_H