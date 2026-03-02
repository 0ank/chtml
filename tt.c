#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

#include "strspan.h"

void exec_template(char* fname, Span* span)
{
    char str[span->len + 1];
    strncpy(str, span->string, span->len);
    Span copy = mkspan(str, span->len);
    bool code = false;
    FILE* outf = fopen(strcat(fname, ".h"), "w");
    Span temp = mkspan(fname, strlen(fname));
    Span tmpl = chopspan_right_mut(&temp, '/', false);
    tmpl = chopspan_left_mut(&tmpl, '.', false);
    tmpl.string[tmpl.len] = '\0';
    int cnt = 0;
    Span params = chopspan_left_mut(&copy, '\n', false);
    char* define = strcat(tmpl.string, "_TEMPLATE");
    fprintf(outf, "#ifndef %s\n"
                  "#define %s\n"
                  "void views_%.*s(%.*s)\n"
                  "{\n", define, define, (int)tmpl.len, tmpl.string, (int)params.len, params.string);
    while(copy.len)
    {
        Span token = chopspan_left_mut(&copy, '$', false);
        if(code)
        {
            fprintf(outf, "    %.*s\n", (int)token.len, token.string);
        }
        else
        {
            fprintf(outf, "    OUT(\"");
            for(size_t i = 0; i < token.len; i++)
            {
                fprintf(outf, "\\x%02x", token.string[i]);
            }
            fprintf(outf, "\", %lu);\n", token.len);
        }
        code = !code;
        cnt++;
    }
    fprintf(outf, "}\n"
                  "#endif");
    fclose(outf);
}

int main(int argc, char** argv)
{
    char* buf;
    size_t len;
    if(argc < 1) return 1;
    FILE* tmpl = fopen(argv[1], "r");
    fseek(tmpl, 0, SEEK_END);
    len = ftell(tmpl);
    fseek(tmpl, 0, SEEK_SET);
    buf = calloc(len + 1, sizeof(char));
    fread(buf, len, 1, tmpl);
    printf("Everything's OK\n");
    Span span = mkspan(buf, len);
    exec_template(argv[1], &span);
    fclose(tmpl);
    free(buf);
    return 0;
}