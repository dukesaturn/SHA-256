CC = gcc
WD = src
BD = build
TARGET = sha
CFLAGS = -Wall 

ifdef DEBUG
CFLAGS += -g -DDEBUG
endif

$(TARGET): main.c $(BD)/debug.o $(BD)/sha-256.o
	$(CC) $(CFLAGS) -o $(TARGET) main.c $(BD)/debug.o $(BD)/sha-256.o -lm

$(BD)/debug.o: $(WD)/debug.c
	$(CC) $(CFLAGS) -o $(BD)/debug.o -c $(WD)/debug.c

$(BD)/sha-256.o: $(WD)/sha-256.c
	$(CC) $(CFLAGS) -o $(BD)/sha-256.o -c $(WD)/sha-256.c 

clean:
	rm -f $(BD)/*.o
