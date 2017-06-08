# banderas de compilacion
PRE=G-2311-01
CC = gcc
CFLAGS = -Wall -pedantic -g
FLAGSMK = -I./includes -fG-2311-01-makefile -p./obj
LDLIBS = -lpthread -lircinterface -lircredes -lirctad
MYLIB = -Llib/ -l2311-01
OBJECTSER = obj/$(PRE)-P1_server_funciones.o obj/$(PRE)-P1_server.o
OBJECTLIB = obj/$(PRE)-P1_socket.o obj/$(PRE)-P1_utils.o obj/$(PRE)-P1_semaforos.o obj/$(PRE)-P3_ssl.o
OBJECTCLT = obj/$(PRE)-P2_client.o obj/$(PRE)-P2_client_funciones.o obj/$(PRE)-P2_client_funciones_user.o
SRC = $(PRE)-P1_server.c
SRCLIBS = $(PRE)-P1_socket.c

all:clean certificados lib/lib2311-01.a server client echo/servidor_echo echo/cliente_echo

lib/lib2311-01.a: $(OBJECTLIB)
	ar -rv lib/lib2311-01.a $(OBJECTLIB)

server: $(OBJECTSER)
	$(CC) $^ -o $@  $(LDLIBS) $(MYLIB)

echo/servidor_echo: obj/$(PRE)-P3_echo_server.o
	$(CC) $^ -o $@  $(LDLIBS) $(MYLIB)  -lssl -lcrypto

echo/cliente_echo: obj/$(PRE)-P3_echo_cliente.o
	$(CC) $^ -o $@  $(LDLIBS) $(MYLIB)  -lssl -lcrypto

client: $(OBJECTCLT)
	$(CC) $^ -o $@  $(LDLIBS) $(MYLIB) `pkg-config --libs gtk+-3.0` -rdynamic

obj/$(PRE)-P1_server.o: src/$(PRE)-P1_server.c includes/$(PRE)-P1_server.h includes/$(PRE)-P1_server_funciones.h  includes/$(PRE)-P1_general.h
	$(CC) $(CFLAGS) -c -o  $@ $< $(LDLIBS)  $(MYLIB)

obj/$(PRE)-P3_echo_server.o: src/$(PRE)-P3_echo_server.c includes/$(PRE)-P3_echo_server.h includes/$(PRE)-P1_general.h
	$(CC) $(CFLAGS) -c -o  $@ $< $(LDLIBS)  $(MYLIB)

obj/$(PRE)-P3_echo_cliente.o: src/$(PRE)-P3_echo_cliente.c includes/$(PRE)-P3_echo_cliente.h includes/$(PRE)-P1_general.h
	$(CC) $(CFLAGS) -c -o  $@ $< $(LDLIBS)  $(MYLIB)

obj/$(PRE)-P1_server_funciones.o: src/$(PRE)-P1_server_funciones.c includes/$(PRE)-P1_server_funciones.h  includes/$(PRE)-P1_general.h
	$(CC) $(CFLAGS) -c -o  $@ $< $(LDLIBS)  $(MYLIB)

obj/$(PRE)-P1_socket.o: srclib/$(PRE)-P1_socket.c includes/$(PRE)-P1_socket.h includes/$(PRE)-P1_general.h
	$(CC) $(CFLAGS) -c -o  $@ $<

obj/$(PRE)-P1_utils.o: srclib/$(PRE)-P1_utils.c includes/$(PRE)-P1_utils.h includes/$(PRE)-P1_general.h
	$(CC) $(CFLAGS) -c -o  $@ $<

obj/$(PRE)-P1_semaforos.o: srclib/$(PRE)-P1_semaforos.c includes/$(PRE)-P1_semaforos.h includes/$(PRE)-P1_general.h
	$(CC) $(CFLAGS) -c -o  $@ $<

obj/$(PRE)-P3_ssl.o: srclib/$(PRE)-P3_ssl.c includes/$(PRE)-P3_ssl.h includes/$(PRE)-P1_general.h
	$(CC) $(CFLAGS) -c -o  $@ $< -lssl -lcrypto

obj/$(PRE)-P2_client.o: src/$(PRE)-P2_client.c includes/$(PRE)-P1_general.h includes/$(PRE)-P2_client_funciones.h includes/$(PRE)-P2_client_funciones_user.h
	$(CC) $(CFLAGS) -c -o  $@ $< `pkg-config --cflags gtk+-3.0` -rdynamic

obj/$(PRE)-P2_client_funciones.o: src/$(PRE)-P2_client_funciones.c includes/$(PRE)-P1_general.h
	$(CC) $(CFLAGS) -c -o  $@ $< `pkg-config --cflags gtk+-3.0` -rdynamic

obj/$(PRE)-P2_client_funciones_user.o: src/$(PRE)-P2_client_funciones_user.c includes/$(PRE)-P1_general.h
	$(CC) $(CFLAGS) -c -o  $@ $< `pkg-config --cflags gtk+-3.0` -rdynamic

comprimir: clean
	tar -cvzf ../$(PRE).tar.gz ../$(PRE)

certificados: certCA certCl certSv

certCA:
	openssl genrsa -out certs/rootkey.pem 2048
	openssl req -new -x509 -key certs/rootkey.pem -out certs/rootcert.pem -subj "/CN=Redes2 CA"
	cat certs/rootcert.pem certs/rootkey.pem > certs/ca.pem
	openssl x509 -subject -issuer -noout -in certs/ca.pem

certCl:
	openssl genrsa -out certs/clientekey.pem 2048
	openssl req -new -key certs/clientekey.pem -out certs/clientetemp.pem -subj "/CN=G-2311-01-P1-client"
	openssl x509 -req -in certs/clientetemp.pem -CA certs/ca.pem -CAkey certs/rootkey.pem -CAcreateserial -out certs/clientecert.pem
	cat certs/clientecert.pem certs/clientekey.pem certs/rootcert.pem > certs/cliente.pem
	openssl x509 -subject -issuer -noout -in certs/cliente.pem

certSv:
	openssl genrsa -out certs/svkey.pem 2048
	openssl req -new -key certs/svkey.pem -out certs/svtemp.pem -subj "/CN=G-2311-01-P1-server"
	openssl x509 -req -in certs/svtemp.pem -CA certs/ca.pem -CAkey certs/rootkey.pem -CAcreateserial -out certs/svcert.pem
	cat certs/svcert.pem certs/svkey.pem certs/rootcert.pem > certs/servidor.pem
	openssl x509 -subject -issuer -noout -in certs/servidor.pem

.PHONY: clean

clean:
	rm -f obj/*.o
	rm -f lib/*a
	rm -f server
	rm -f client
	rm -f includes/*.gch
	rm -f echo/servidor_echo
	rm -f echo/cliente_echo
	rm -f certs/*.pem
	rm -f certs/*.srl

documentacion:
	doxygen Doxyfile
