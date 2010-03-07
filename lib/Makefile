# Makefile
# largement insipiré du Makefile libmoebus de Boris Lesner :)

CC = gcc
CFLAGS = -Wall

SRCS = libpiapc.c
OBJS = $(SRCS:.c=.o)

NAME = libpiapc.so
HEADER= piapc.h

EXAMPLE=piapc.c

PREFIX=/usr/

all: $(NAME)

$(NAME):	$(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -shared -o $(NAME)

install: 
	cp $(NAME) $(PREFIX)/lib/
	cp $(HEADER) $(PREFIX)/include/
	chmod a+r $(PREFIX)/include/$(HEADER)

uninstall:
	rm -rf $(PREFIX)/lib/$(NAME)
	rm -rf $(PREFIX)/include/$(HEADER)

example: $(EXAMPLE)
	$(CC) -W -Wall -lpiapc -L. $(EXAMPLE) -o  $(EXAMPLE:.c=)
	chmod ug+s piapc

clean:
	rm -f $(OBJS)
	rm -f $(NAME)
	rm -f $(EXAMPLE:.c=)
	rm -f *~
	rm -f *#
