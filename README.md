# Build
Install the following dependencies:
* For runtime: `lighttpd`
* When normal build: `sqlite3` + its headers

Required for compiling:
* `make`
* `gcc` or any other c99 compiler
* When normal build: `lua5.3` or any version >= 5.1
* Only when static AND from git: `wget` and `tar` (likely already installed)

Normal build: `make all`

Static build: `make STATIC=true`

To run: `make run`

IMPORTANT: you will have to change the server.document-root in the config to match this the full path of directory.

# Running
The server will run on port 8000.

# Why
The biggest inspiration for this project is the BCHS stack (BSD C httpd sqlite), I don't have an OpenBSD machine or VM so the BSD and (OpenBSD) httpd parts are replaced by Linux and Lighttpd.

Other popular dynamic website stacks are mostly written in interpreted languages, adding more complexity on top of libc and providing more possibilities for bugs.

Memory management (frees) aren't really required (for CGI, NOT FastCGI): in the time the server sends a response there won't be too much allocated memory and the OS can free it afterwards. Even in garbage collected languages the garbage collector also doesn't have to run for this reason, to emphasize that is only the case when using CGI like PHP is/was often used. Just to be sure one should write a wrapper around the libc memory allocation function to check for large memory use and return a 500.
