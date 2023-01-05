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
    exit(1);
}

