ifdef STATIC
LUA_DEP=vendor/minilua
LUA=$(LUA_DEP)
SQLITE_DEP=vendor/sqlite3.o
SQLITE=$(SQLITE_DEP) -Ivendor
else
LUA=lua
SQLITE=-lsqlite3
endif

all: bin/pay

vendor/sqlite3.o:
	make -C vendor sqlite3.o

vendor/minilua:
	make -C vendor minilua

run: all
	lighttpd -D -f lighttpd.conf

gen-index.c: index.html | $(LUA_DEP)
	$(LUA) parse.lua < $< > $@

bin/pay: pay.c gen-index.c | $(SQLITE_DEP)
	$(CC) -o $@ $(SQLITE) $<

.PHONY: clean

clean:
	rm -f bin/pay gen-index.c
	make -C vendor clean
