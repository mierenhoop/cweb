#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

static void internal_error(const char* msg) {
    fputs(msg, stderr);
    fputc('\n',stderr);
    fputs(strerror(errno), stderr);
    fputc('\n',stderr);
    // Write 
    fputs("Status: 500 Internal Server Error\r\n", stdout);
    fputs("Content-type: text/plain\r\n\r\n", stdout);
    fputs("Error 500", stdout);
    exit(1);
}

