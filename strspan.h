#ifndef STRSPAN_H
#define STRSPAN_H

#include<stdlib.h>
#include<stdbool.h>
#include<unistd.h>

typedef struct
{
    size_t len;
    char* string;
} Span;

Span mkspan(char* string, size_t len);
void brkspan(Span* span);
void printspan(Span span);
void printspanln(Span span);
Span chopspan_left_mut(Span* span, char delim, bool keep_delim);
Span chopspan_right_mut(Span* span, char delim, bool keep_delim);
Span chopspan_right(Span span, char delim, bool keep_delim);
Span concatspan(Span span, Span tail);

#endif
