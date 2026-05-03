CC = gcc
WD = src
BD = build
TARGET = sha256
CFLAGS = -Wall 

ifdef DEBUG
CFLAGS += -g 
endif

$(TARGET): main.c $(BD)/sha-256.o
	$(CC) $(CFLAGS) -o $(TARGET) main.c $(BD)/sha-256.o -lm

$(BD)/sha-256.o: $(WD)/sha-256.c
	$(CC) $(CFLAGS) -o $(BD)/sha-256.o -c $(WD)/sha-256.c 

clean:
	rm -f $(BD)/*.o
