#include <stdio.h>

#include "common.c"

#include "gen-index.c"

int main() {
    fputs("Status: 200 OK\r\n", stdout);
    fputs("Content-type: text/html\r\n\r\n", stdout);
    {
        const char *value = "fjdaf";
        int some_number = 10;

        int items[3] = {2, 3, 4};
        int n_items = 3;

        const char *query = getenv("QUERY_STRING");

        execute_index;
    }
}
