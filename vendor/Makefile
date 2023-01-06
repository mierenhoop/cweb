all: minilua sqlite3 sqlite3.o sqlite3.h

luajit-src.tar.gz:
	wget -O $@ https://luajit.org/download/LuaJIT-2.1.0-beta3.tar.gz

minilua.c: luajit-src.tar.gz
	tar -xzf $< --wildcards **/$@ -O > $@

minilua: minilua.c
	$(CC) -o $@ $< -lm

sqlite-src.tar.gz:
	wget -O $@ https://www.sqlite.org/2022/sqlite-autoconf-3400100.tar.gz

shell.c sqlite3.c sqlite3.h: sqlite-src.tar.gz
	tar -xzf $< --wildcards **/$@ -O > $@

sqlite3.o: sqlite3.c | sqlite3.h
	$(CC) -c -o $@ -O2 $^

sqlite3: shell.c sqlite3.o | sqlite3.h
	$(CC) -o $@ -O2 $^

.PHONY: clean

clean:
	rm -rf luajit* minilua* shell.c sqlite*
