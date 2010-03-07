# Makefile

# Pour utiliser la carte piapc (je ne dis pas ça pour toi alain, c'est au cas ou un
# internaute en aurait une ;-) ) il faut d'abord installer la bibliothèque:
# cd lib/
# less README.fr
# su to root 
# make 
# make install
# ldconfig

# Sinon: make all-serial

CC=gcc
CFLAGS=-Wall
DAEMON=-DDAEMON_MODE
LFLAGS=-lpiapc
VERSION=1.03

PREFIX=/usr/local/

PROGS_PIA=pia-robby-server pia-robby-client
PROGS_SERIAL=serial-robby-server serial-robby-client robbyd
PROGS=$(PROGS_PIA) $(PROGS_SERIAL)

MAKEFLAGS += --no-print-directory

all: all-pia all-serial

# Controle du robot via la carte pia
all-pia: $(PROGS_PIA)

pia-robby-server: pia-robby-server.o messages.o
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $+
	chmod u+s pia-robby-server

pia-robby-client: pia-robby-client.o
	$(CC) $(CFLAGS) -o $@ $+


# Controle du robot via la carte E/S sur port serie
all-serial: $(PROGS_SERIAL)

serial-robby-server: serial-robby-server.o messages.o serial-robby.o serial.o
	$(CC) $(CFLAGS) -o $@ $+

robbyd: robbyd.o messages.o serial-robby.o serial.o
	$(CC) $(CFLAGS) -o $@ $+

robbyd.o: serial-robby-server.c
	$(CC) $(CFLAGS) $(DAEMON) -c -o $@ $+

serial-robby-client: serial-robby-client.o
	$(CC) $(CFLAGS) -o $@ $+

# Dés/Installation des binaires

install: 
	cp $(PROGS) $(PREFIX)bin/

install-serial:
	cp $(PROGS_SERIAL) $(PREFIX)bin/


install-pia:
	cp $(PROGS_PIA) $(PREFIX)bin/

uninstall:
	rm -rf $(PREFIX)bin/pia-robby-client
	rm -rf $(PREFIX)bin/pia-robby-server
	rm -rf $(PREFIX)bin/serial-robby-client
	rm -rf $(PREFIX)bin/serial-robby-server
	rm -rf $(PREFIX)bin/robbyd

# Divers
package: libclean clean
	cd .. &&\
	tar -czvf Robby-$(VERSION).tgz Robby-$(VERSION)/

libclean:
	cd lib && make clean

clean:
	rm -f *.o core *~ *% *#	
	rm -f $(PROGS)

new: clean all

# end of Makefile
