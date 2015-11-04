#Makefile for Lamina-Project by James Birdsong

all: .sqlite_built_marker

.sqlite_built_marker:
	touch .sqlite_built_marker; cd ../lib/sqlite;./configure --enable-shared --disable-threadsafe 

clean:
	rm -f .sqlite_built_marker;
	make -C ../lib/sqlite clean;

