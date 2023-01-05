SQLITE= -lsqlite3

# if static 
# SQLITE=sqlite.o
# sqlite.c: wget 
# sqlite.o: sqlite.c
# end

# gen-index.c: index.html
# 	awk -f parse.awk -- $< > $@

bin/pay: pay.c
	$(CC) -o $@ $(SQLITE) $<
