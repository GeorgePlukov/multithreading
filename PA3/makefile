LDFLAGS = -lnsl -lpthread -lm
CFLAGS=-g -Wall -fopenmp
CC=gcc


CLIENT= client
APPEND= server_append
VERIFY= server_verify

all: $(CLIENT) $(APPEND) $(VERIFY)



$(CLIENT): client.o append_clnt.o verify_clnt.o verify_xdr.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

$(APPEND): server_append.o append_svc.o propertyEnforcer.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

$(VERIFY): server_verify.o verify_svc.o verify_xdr.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	$(RM) *.o $(CLIENT) $(APPEND) $(VERIFY)
