#Makefile for Lamina-Project

all: .sqlite_built_marker lib/sim/libsim.a

.sqlite_built_marker:
	touch .sqlite_built_marker; cd lib/sqlite;./configure --enable-shared --disable-threadsafe; make

lib/sim/libsim.a:
	make -C lib/sim

clean:
	rm -f .sqlite_built_marker
	make -C lib/sqlite clean
	make -C lib/sim clean

