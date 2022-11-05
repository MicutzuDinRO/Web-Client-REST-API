#include "commands.h"

void remove_newline(char *str) {
  char *aux = calloc(BUFLEN, sizeof(char));
  strcpy(aux, str + strlen(str));
  strcpy(str + strlen(str) - 1, aux);
  free(aux);
}

int is_natural_number(char *str) {
  if (!str)
    return 0;
  for (int i = 0; i < strlen(str); i++)
    if (str[i] < '0' || str[i] > '9')
      return 0;

  if (atoi(str) <= 0)
    return 0;

  return 1;
}

int is_alphabetic(char *str) {
  if (!str)
    return 0;
  for (int i = 0; i < strlen(str); i++)
    if (!(str[i] >= 'a' && str[i] <= 'z') && !(str[i] >= 'A' && str[i] <= 'Z'))
      return 0;

  return 1;
}

int verify_error(char *response) {
  JSON_Value *resp_val;
  resp_val = json_parse_string(strstr(response, "{"));

  JSON_Object *resp_obj;
  resp_obj = json_value_get_object(resp_val);

  const char *error = json_object_get_string(resp_obj, "error");
  if (error) {
    puts(error);
    json_value_free(resp_val);
    return 1;
  }

  json_value_free(resp_val);
  return 0;
}

void register_form(int sockfd) {
  char **payload, *message, *response;
  payload = calloc(ONE, sizeof(char *));
  payload[0] = calloc(BUFLEN, sizeof(char));
  char *username = calloc(BUFLEN, sizeof(char));
  char *password = calloc(BUFLEN, sizeof(char));

  JSON_Value *value = json_value_init_object();
  JSON_Object *object = json_value_get_object(value);

  printf("username: ");
  fgets(username, BUFLEN, stdin);
  remove_newline(username);
  json_object_set_string(object, "username", username);

  printf("password: ");
  fgets(password, BUFLEN, stdin);
  remove_newline(username);
  json_object_set_string(object, "password", password);

  payload[0] = json_serialize_to_string_pretty(value);

  message = compute_post_request(IP, REGISTER_PATH, PAYLOAD_TYPE, payload, ONE,
                                 NULL, 0, NULL);
  send_to_server(sockfd, message);
  response = receive_from_server(sockfd);

  if (!verify_error(response))
    puts("Successfully registered!");

  json_value_free(value);
  json_free_serialized_string(payload[0]);
}

void login(int sockfd, char **cookies) {
  if (strstr(cookies[0], "connect")) {
    puts("You are already logged in!");
    return;
  }

  char **payload, *message, *response;
  payload = calloc(ONE, sizeof(char *));
  payload[0] = calloc(BUFLEN, sizeof(char));
  char *username = calloc(BUFLEN, sizeof(char));
  char *password = calloc(BUFLEN, sizeof(char));

  JSON_Value *value = json_value_init_object();
  JSON_Object *object = json_value_get_object(value);

  printf("username: ");
  fgets(username, BUFLEN, stdin);
  remove_newline(username);
  json_object_set_string(object, "username", username);

  printf("password: ");
  fgets(password, BUFLEN, stdin);
  remove_newline(username);
  json_object_set_string(object, "password", password);

  payload[0] = json_serialize_to_string_pretty(value);

  message = compute_post_request(IP, LOGIN_PATH, PAYLOAD_TYPE, payload, ONE,
                                 NULL, 0, NULL);
  send_to_server(sockfd, message);
  response = receive_from_server(sockfd);

  char *start, *end;
  start = strstr(response, "connect");
  end = strstr(response, "; Path");

  memcpy(cookies[0], start, end - start);

  if (!verify_error(response))
    puts("Successfully logged in!");

  json_value_free(value);
  json_free_serialized_string(payload[0]);
}

void enter_library(int sockfd, char **cookies, char *token) {
  if (token[0] != '\0') {
    puts("You are already inside the library!");
    return;
  }
  char *message, *response;

  message = compute_get_request(IP, LIBRARY_PATH, NULL, cookies, 1, NULL);
  send_to_server(sockfd, message);
  response = receive_from_server(sockfd);

  JSON_Value *resp_val = json_parse_string(strstr(response, "{"));
  JSON_Object *resp_obj = json_value_get_object(resp_val);

  if (!verify_error(response)) {
    const char *given_token = json_object_get_string(resp_obj, "token");
    strcpy(token, given_token);
    puts("Successfully entered the library!");
  }

  json_value_free(resp_val);
}

void get_books(int sockfd, char *token) {
  char *library, *message, *response;

  message = compute_get_request(IP, BOOKS_PATH, NULL, NULL, 0, token);
  send_to_server(sockfd, message);
  response = receive_from_server(sockfd);

  library = strstr(response, "[");

  if (library) {
    puts(library);
    return;
  }

  verify_error(response);
}

void get_book(int sockfd, char *token) {

  char *message, *response, *ID, *path, *book;
  ID = calloc(BUFLEN, sizeof(char));
  path = calloc(BUFLEN, sizeof(char));
  book = calloc(BUFLEN, sizeof(char));

  printf("ID = ");

  fgets(ID, BUFLEN, stdin);
  remove_newline(ID);
  strcpy(path, BOOKS_PATH);
  strcat(path, "/");
  strcat(path, ID);
  message = compute_get_request(IP, path, NULL, NULL, 0, token);
  send_to_server(sockfd, message);
  response = receive_from_server(sockfd);

  book = strstr(response, "[");

  if (book) {
    puts(book);
    return;
  }

  verify_error(response);
}

void add_book(int sockfd, char *token) {
  char **payload, *message, *response;
  payload = calloc(ONE, sizeof(char *));
  payload[0] = calloc(BUFLEN, sizeof(char));
  char *title = calloc(BUFLEN, sizeof(char));
  char *author = calloc(BUFLEN, sizeof(char));
  char *genre = calloc(BUFLEN, sizeof(char));
  char *page_count = calloc(BUFLEN, sizeof(char));
  char *publisher = calloc(BUFLEN, sizeof(char));

  JSON_Value *value = json_value_init_object();
  JSON_Object *object = json_value_get_object(value);

  printf("title: ");
  fgets(title, BUFLEN, stdin);
  remove_newline(title);
  if (title[0] == '\0') {
    puts("Title cannot be empty!");
    json_value_free(value);
    return;
  }
  json_object_set_string(object, "title", title);

  printf("author: ");
  fgets(author, BUFLEN, stdin);
  remove_newline(author);
  if (author[0] == '\0') {
    puts("Author cannot be empty!");
    json_value_free(value);
    return;
  }
  json_object_set_string(object, "author", author);

  printf("genre: ");
  fgets(genre, BUFLEN, stdin);
  remove_newline(genre);
  if (genre[0] == '\0' || !is_alphabetic(genre)) {
    puts("Genre cannot be empty or non-alphabetical!");
    json_value_free(value);
    return;
  }
  json_object_set_string(object, "genre", genre);

  printf("page_count: ");
  fgets(page_count, BUFLEN, stdin);
  remove_newline(page_count);
  if (!is_natural_number(page_count)) {
    puts("Page count must be a non-zero natural number!");
    json_value_free(value);
    return;
  }
  json_object_set_number(object, "page_count", atoi(page_count));

  printf("publisher: ");
  fgets(publisher, BUFLEN, stdin);
  remove_newline(publisher);
  if (publisher[0] == '\0') {
    puts("Publisher cannot be empty!");
    json_value_free(value);
    return;
  }
  json_object_set_string(object, "publisher", publisher);

  payload[0] = json_serialize_to_string_pretty(value);

  message = compute_post_request(IP, BOOKS_PATH, PAYLOAD_TYPE, payload, ONE,
                                 NULL, 0, token);
  send_to_server(sockfd, message);
  response = receive_from_server(sockfd);

  if (!verify_error(response)) {
    puts("Successfully added the book!");
  }

  json_value_free(value);
  json_free_serialized_string(payload[0]);
}

void delete_book(int sockfd, char *token) {
  char *message, *response, *ID, *path;
  ID = calloc(BUFLEN, sizeof(char));
  path = calloc(BUFLEN, sizeof(char));

  printf("ID = ");
  fgets(ID, BUFLEN, stdin);
  remove_newline(ID);

  strcpy(path, BOOKS_PATH);
  strcat(path, "/");
  strcat(path, ID);

  message = compute_delete_request(IP, path, NULL, NULL, 0, token);
  send_to_server(sockfd, message);
  response = receive_from_server(sockfd);

  if (!verify_error(response))
    puts("Successfully deleted the book!");
}

void logout(int sockfd, char **cookies, char *token) {
  char *message, *response;

  message = compute_get_request(IP, LOGOUT_PATH, NULL, cookies, 1, NULL);
  send_to_server(sockfd, message);
  response = receive_from_server(sockfd);

  if (!verify_error(response)) {
    puts("Successfully logged out!");
    memset(cookies[0], 0, BUFLEN);
    memset(token, 0, BUFLEN);
  }
}

void exit_client(int sockfd) {
  close_connection(sockfd);
  exit(0);
}