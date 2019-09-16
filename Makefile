SRCS=scanner-button.c
CLFAGS?=-Wall
OBJS = $(SRC:.c=.o)
LOADLIBES=-lsane
BINDIR?=$(PREFIX)/usr/bin

scanner-button: $(OBJS)

install: scanner-button
	install -d $(BINDIR)
	install $^ $(BINDIR)
