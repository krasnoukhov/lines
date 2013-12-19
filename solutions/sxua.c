//
//  sxua.c
//  lines
//
//  Created by Oleksandr Skrypnyk on 12/17/13.
//  Copyright (c) 2013 Unteleported. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  kHorizontal,
  kVertical,
  kMainDiagonal,
  kAntiDiagonal
} LineType;

struct SquareMatrix {
  int size;
  char *values;
};

void SM_initialize(struct SquareMatrix *matrix, int size) {
  matrix->values = calloc(size * size, sizeof(char));
  matrix->size = size;
}

void SM_set(struct SquareMatrix *matrix, int x, int y, char value) {
  if (x < matrix->size && x >= 0 && y < matrix->size && y >= 0) {
    matrix->values[(x * matrix->size) + y] = value;
  }
}

char SM_get(struct SquareMatrix *matrix, int x, int y) {
  if (x < matrix->size && x >= 0 && y < matrix->size && y >= 0) {
    return matrix->values[(x * matrix->size) + y];
  }
  return '\0';
}

void SM_move(struct SquareMatrix *matrix, int x1, int y1, int x2, int y2) {
  char old;
  if (x1 < matrix->size && x1 >= 0 && y1 < matrix->size && y1 >= 0 &&
      x2 < matrix->size && x2 >= 0 && y2 < matrix->size && y2 >= 0 &&
      abs(x2-x1) >= 0 && abs(x2-x1) <= 1 && abs(y2-y1) >= 0 && abs(y2-y1) <= 1) {
    if ((old = SM_get(matrix, x1, y1))) {
      SM_set(matrix, x2, y2, old);
      SM_set(matrix, x1, y1, '\0');
    }
  }
}

void SM_free(struct SquareMatrix *matrix) {
  free(matrix->values);
  matrix->values = NULL;
}

void SM_find_plain_lines(struct SquareMatrix *matrix, LineType type, int *result) {
  char c = '\0';
  int x, y, len = 0, max = 0;

  for (x = 0; x < matrix->size; x++) {
    len = 0;
    max = 0;
    for (y = 0; y < matrix->size; y++) {
      char v = (type == kVertical) ? SM_get(matrix, x, y) : SM_get(matrix, y, x);
      if (y == 0) {
        c = v;
        if (v != '\0') len++;
      } else {
        if (v != '\0') len = (c == v) ? len + 1 : 1;
        if (len > max) max = len;
        c = v;
      }
    }
    if (max >= 5) ++*result;
  }
}

void SM_find_diagonal_lines(struct SquareMatrix *matrix, LineType type, int *result) {
  char c = '\0';
  int x, y, len = 0, max = 0;

  for (x = 0; x < matrix->size * 2 - 1; x++) {
    len = 0;
    max = 0;
    int o = x < matrix->size ? 0 : x - matrix->size + 1;
    for (y = o; y <= matrix->size; y++) {
      int n = (type == kMainDiagonal) ? (x - y) : (matrix->size - 1) - (x - y);
      char v = SM_get(matrix, y, n);
      if (y - o == 0) {
        c = v;
        if (v != '\0') len++;
      } else {
        if (v != '\0') len = (c == v) ? len + 1 : 1;
        if (len > max) max = len;
        c = v;
      }
    }
    if (max >= 5) ++*result;
  }
}

void SM_find_lines(struct SquareMatrix *matrix, int *result) {
  SM_find_plain_lines(matrix, kHorizontal, result);
  SM_find_plain_lines(matrix, kVertical, result);
  SM_find_diagonal_lines(matrix, kMainDiagonal, result);
  SM_find_diagonal_lines(matrix, kAntiDiagonal, result);
}

char *last_chunk(char *line) {
  char *result;
  while (strsep(&line, " ") != NULL) {
    result = strsep(&line, " ");
  }
  return result;
}

void ball(struct SquareMatrix *matrix, char *t) {
  int x, y;
  char *color;
  while (t != NULL) {
    x = atoi(strsep(&t, ","));
    y = atoi(strsep(&t, ","));
    color = strsep(&t, ",");
  }
  char c = color[0];
  SM_set(matrix, x, y, c);
}

void move(struct SquareMatrix *matrix, char *t) {
  int x1, y1, x2, y2;
  while (t != NULL) {
    x1 = atoi(strsep(&t, ","));
    y1 = atoi(strsep(&t, ","));
    x2 = atoi(strsep(&t, ","));
    y2 = atoi(strsep(&t, ","));
  }
  SM_move(matrix, x1, y1, x2, y2);
}

int main(int argc, const char * argv[]) {
  int result = 0;
  size_t content_size = 1;
  struct SquareMatrix matrix;
  char buffer[BUFSIZ], command[5], *s, *t, *tmp, *content = malloc(BUFSIZ);

  if (content == NULL) {
    perror("Failed to allocate content");
    exit(1);
  }

  content[0] = '\0';

  while (fgets(buffer, BUFSIZ, stdin)) {
    char *old = content;
    content_size += strlen(buffer);
    content = realloc(content, content_size);
    if (content == NULL) {
      perror("Failed to reallocate content");
      free(old);
      exit(2);
    }
    strcat(content, buffer);
  }

  if (ferror(stdin)) {
    free(content);
    perror("Error reading from stdin");
    exit(3);
  }

  while ((tmp = strsep(&content, "\n")) != NULL) {
    t = last_chunk(tmp);
    strcpy(command, "SIZE");
    s = strstr(tmp, command);
    if (s != NULL && t != NULL) {
      SM_initialize(&matrix, atoi(t));
    } else {
      strcpy(command, "BALL");
      s = strstr(tmp, command);
      if (s != NULL) {
        ball(&matrix, t);
      } else {
        strcpy(command, "MOVE");
        s = strstr(tmp, command);
        if (s != NULL) {
          move(&matrix, t);
        }
      }
    }
  }

  SM_find_lines(&matrix, &result);
  SM_free(&matrix);
  fflush(stdout);
  printf("%d", result);

  return 0;
}