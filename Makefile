CC = gcc
WD = src
BD = build
TEST = test
TARGET = sha256
PERFORMANCE_FILE = performance
NIST_FILE = nist

CFLAGS += -MD -MP -O2 -march=native
CFLAGS += $(WARNINGS)

WARNINGS += -Wall -Wextra -Wwrite-strings
WARNINGS += -Wpedantic -Warray-bounds -Wconversion
WARNINGS += -Wstrict-prototypes

ifdef DEBUG
CFLAGS += -g 
endif

ifdef PERFORMANCE
$(PERFORMANCE_FILE): $(TEST)/$(PERFORMANCE_FILE).c $(BD)/sha-256.o
	$(CC) $(CFLAGS) -o $(PERFORMANCE_FILE) $(TEST)/$(PERFORMANCE_FILE).c $(BD)/sha-256.o -lm	
else ifdef NIST
$(NIST_FILE): $(TEST)/$(NIST_FILE).c $(BD)/sha-256.o
	$(CC) $(CFLAGS) -o $(NIST_FILE) $(TEST)/$(NIST_FILE).c $(BD)/sha-256.o -lm
else
$(TARGET): main.c $(BD)/sha-256.o
	$(CC) $(CFLAGS) -o $(TARGET) main.c $(BD)/sha-256.o -lm
endif

$(BD)/sha-256.o: $(WD)/sha-256.c
	$(CC) $(CFLAGS) -o $(BD)/sha-256.o -c $(WD)/sha-256.c 

clean:
	rm -f $(BD)/*.o
