LDFLAGS = -lnsl
CFLAGS=-g
CC=gcc


CLIENT= client
APPEND= server_append
VERIFY= server_verify

all: $(CLIENT) $(APPEND)



$(CLIENT): client.o append_clnt.o verify_clnt.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

$(APPEND): server_append.o append_svc.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

$(VERIFY): server_verify.o verify_svc.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	$(RM) *.o $(CLIENT) $(APPEND) $(VERIFY)