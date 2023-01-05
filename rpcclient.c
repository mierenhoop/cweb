#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

struct json_value {
    char *value;
    int n;
    struct json_value *children;
};

#define next do { (*s)++; if (!**s) internal_error("Json not complete"); } while (0)

static void skip_white(char **s) {
    while (isspace(**s)) next;
}

// Expects valid json, else either error 
// or parse nothing, encounter \0 and error
static struct json_value parse_json(char **s) {
    skip_white(s);

    if (!s || !*s || !**s || (**s != '{' && **s != '[')) internal_error("Invalid json");

    struct json_value root = { NULL, 0, NULL };

    do {
        next;
        skip_white(s);

        if (**s == ']' || **s == '}') break;

        struct json_value child = { NULL, 0, NULL };

        char *key, *end;

        if (**s == '[' || **s == '{') {
            key = *s;
            child = parse_json(s);
            next;
            end = *s;
        } else if (**s == '"') {
            next;
            key = *s;
            while (**s != '"') {
                if (**s == '\\') next;
                next;
            };
            end = *s;
            next;
        } else {
            key = *s;
            while (**s != ',' && **s != ':' && !isspace(**s) 
                    && **s != '}' && **s != ']') next;
            end = *s;
        }
        skip_white(s);


        child.value = malloc(end - key + 1); // TODO: make sure malloc works
        child.value[(int)(end - key)] = '\0';
        memcpy(child.value, key, end - key);


#define ARRAY_MINSIZE 8
        // TODO: decide if hack should be replaced by trivial len, cap vars
        // if n currently 0, alloc ARRAY_MINSIZE (n = 1 when child added)
        // if n >= ARRAY_MINSIZE and n is power of 2 then grow x2
        // so len:cap 0:0 1:8 2:8 ... 8:8 9:16 10:16 ... 32:32 33:64 ...
        if (!root.n || (root.n >= ARRAY_MINSIZE && !(root.n & (root.n - 1))))
            root.children = realloc(root.children,
                    (root.n ? (root.n * 2) : ARRAY_MINSIZE)
                    * sizeof(struct json_value)); // TODO: check realloc
        root.children[root.n] = child;
        root.n++;

        //printf("%s\n", child.value);
    } while(**s == ',' || **s == ':');

    return root;
}

struct json_value parse_rpc_response(char *response) {
    char *headers_end = strstr(response, "\r\n\r\n");
    if (!headers_end) internal_error("Http response does not contain body");
    return parse_json(&headers_end); // Can do with \r\n\r\n included...
}

int main(int argc, char *argv[])
{
    struct hostent *server = gethostbyname("127.0.0.1");

    struct sockaddr_in serv_addr = {
        .sin_port = htons(8080),
        .sin_family = AF_INET,
    };

    bcopy(server->h_addr, &serv_addr.sin_addr.s_addr, server->h_length);

    struct timeval timeout = {
        .tv_sec = 1,
        .tv_usec = 0,
    };
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(struct timeval));
    setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(struct timeval));

    connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

#define CHUNKSIZE 256
    int cap = 1024;
    char *buffer = realloc(NULL, cap);
    int len = 0;
    int n;
    do {
        n = recv(sockfd, buffer + len, CHUNKSIZE, 0);
        puts("reading");
        printf("%d\n", n);
        if (n < 0) internal_error("Reading from TCP socket error");
        len += n;
        if (len + CHUNKSIZE >= cap) {
            cap *= 2;
            buffer = realloc(buffer, cap);
        }
    } while(n == CHUNKSIZE);

    buffer[len] = '\0'; // TODO: WTF?

    printf("%s\n", buffer);

    //parse_rpc_response(buffer);
    return 0;
}
