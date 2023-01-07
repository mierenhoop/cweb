#include <sqlite3.h>

#include "common.c"

#include "gen-index.c"

sqlite3 *db;

static void db_exec(const char*sql) {
    if (sqlite3_exec(db, sql, 0, 0, NULL) != SQLITE_OK ) {
        internal_error("SQL error");
    } 
}

sqlite3_stmt *res;

static void db_prep(const char *sql) {
    if (sqlite3_prepare_v2(db, sql, -1, &res, 0) != SQLITE_OK) {
        internal_error("Failed to fetch data");
    }    
}

// return 1 if ok and 0 if done
static int db_step() {
    int rc = sqlite3_step(res);
    if (rc == SQLITE_ROW) return 1;
    else if (rc == SQLITE_DONE) return 0;
    else internal_error("SQLITE Step failed");
}

static char * db_col_text(int c) {
    const char *t = sqlite3_column_text(res, c);
    char *dup;
    if (!t || !(dup = strdup(t))) {
        internal_error("Failed to fetch data");
    }
    return dup;
}

static void _db_close() {
    sqlite3_close_v2(db);
}

static void db_open() {
    if (sqlite3_open("/tmp/test.db", &db) != SQLITE_OK) {
        // TODO: close too when open failed?
        internal_error("Cannot open database");
    }
    atexit(_db_close);
}

static char *read_post() {
    char *cont_len_s = getenv("CONTENT_LENGTH");
    int cont_len;
    if (!cont_len_s || strlen(cont_len_s) <= 0 || !(cont_len = atoi(cont_len_s)))
        internal_error("Content length header incorrect");
    char *post_data = malloc(cont_len+1);
    fread(post_data, 1, cont_len, stdin);
    post_data[cont_len]='\0';
    return post_data;
}

int main() {
    db_open();

    char *method = getenv("REQUEST_METHOD");
    if (method && !strcmp(method, "POST")) {
        char *post_data = read_post();
        if (!strncmp(post_data, "remove", strlen("remove"))) {
            char *pos = strchr(post_data, '=') + 1;
            int index = 0;
            if (*pos == '\0' || !(index = atoi(pos)))
                internal_error("Correct index not provided");
            db_prep("DELETE FROM Todos WHERE Id = ?");
            sqlite3_bind_int(res, 1, index);
            while (db_step()) {;}
        } else if (!strncmp(post_data, "add", strlen("add"))) {
            char *content = strchr(post_data, '=') + 1;
            // TODO: POST body and the content
            if (strlen(content) != 0) {
                db_prep("INSERT INTO Todos(Content) VALUES (?)");
                sqlite3_bind_text(res, 1, content, strlen(content), NULL);
                while (db_step()) {;}
            }
        }
    }

    fputs("Status: 200 OK\r\n", stdout);
    fputs("Content-type: text/html\r\n\r\n", stdout);

    execute_index;
}
