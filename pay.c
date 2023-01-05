#include <stdio.h>

#include "gen-index.c"

int main() {
    fputs("Status: 200 OK\r\n", stdout);
    fputs("Content-type: text/html\r\n\r\n", stdout);
    {
        const char *value = "fjdaf";
        int some_number = 10;

        execute_index;
    }
}
