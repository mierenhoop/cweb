SQLITE= -lsqlite3

# if static 
# SQLITE=sqlite.o
# sqlite.c: wget 
# sqlite.o: sqlite.c
# end

all: bin/pay

run: all
	lighttpd -D -f lighttpd.conf

gen-index.c: index.html
	lua parse.lua < $< > $@

bin/pay: pay.c gen-index.c
	$(CC) -o $@ $(SQLITE) $<

.PHONY: clean

clean:
	rm -f bin/pay gen-index.c

