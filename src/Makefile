CFLAGS = -Wall -Wextra -pedantic -Wno-long-long -O2
OBJS =$(patsubst %.c ,%.o,$(wildcard *.c))
EXEC = cartas

instalar : $(EXEC)
	cp $(EXEC) /usr/lib/cgi-bin
	cp -r cards /var/www/html
	cp -r botoes /var/www/html
	touch instalar

arch : $(EXEC)
	sudo cp $(EXEC) /srv/http/cgi-bin
	sudo cp -r cards /srv/http
	sudo cp -r botoes /srv/http
	touch instalar

travis : $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

limpar :
	rm $(EXEC) *.o
