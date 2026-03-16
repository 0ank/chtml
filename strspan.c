#include "strspan.h"
#include<stdio.h>
#include<unistd.h>
#include<string.h>

Span mkspan(char* string, size_t len)
{
    return (Span) {
        .string = string,
        .len = len
    };
}
void brkspan(Span* span)
{
    free(span->string);
    span->string = NULL;
    span->len = 0;
}
void printspan(Span span)
{
    printf("%.*s", (int)span.len, span.string);
}
void printspanln(Span span)
{
    printf("%.*s\n", (int)span.len, span.string);
}
// Delimiter-exclusive tokens
Span chopspan_left_mut(Span* span, char delim, bool keep_delim)
{
    size_t i = 0;
    while(i < span->len && span->string[i] != delim) i++;
    Span token = mkspan(span->string, i);
    if(i < span->len)
    {
        if(keep_delim)
        {
            span->len -= i;
            span->string += i;
        }
        else
        {
            span->len -= i + 1;
            span->string += i + 1;
        }
    }
    else
    {
        span->len -= i;
        span->string += i;
    }
    return token;
}
// Delimiter-exclusive tokens
Span chopspan_left(Span span, char delim, bool keep_delim)
{
    char delims[2];
    delims[0] = delim;
    delims[1] = '\0';
    if(strtok(span.string, delims) == NULL) return mkspan("", 0);
    Span token = mkspan(span.string, 0);
    while(span.string[0] != delim) span.string++, span.len--, token.len++;
    if(!keep_delim) span.string++, span.len--, token.len++;
    return token;
}
// Delimiter-exclusive tokens
Span chopspan_right_mut(Span* span, char delim, bool keep_delim)
{
    size_t i = span->len - 1;
    while(i >= 0 && span->string[i] != delim) i--;
    Span token = mkspan(span->string + i + 1, span->len - i);
    if(i >= 0)
    {
        if(keep_delim)
        {
            span->len -= i;
        }
        else
        {
            span->len -= i + 1;
        }
    }
    else
    {
        span->len -= i;
    }
    return token;
}
// Delimiter-exclusive tokens
Span chopspan_right(Span span, char delim, bool keep_delim)
{
    size_t i = span.len - 1;
    while(i >= 0 && span.string[i] != delim) i--;
    Span token = mkspan(span.string + i + 1, span.len - i);
    if(i >= 0)
    {
        if(keep_delim)
        {
            span.len -= i;
        }
        else
        {
            span.len -= i + 1;
        }
    }
    else
    {
        span.len -= i;
    }
    return token;
}

Span concatspan(Span span, Span tail)
{
    Span result;
    result.string = calloc(span.len + tail.len + 1, 1);
    result.len = span.len;
    strncpy(result.string, span.string, span.len);
    result.string[result.len] = '\0';
    strncat(result.string, tail.string, tail.len);
    result.len += tail.len;
    return result;
}
