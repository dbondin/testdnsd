TARGET = testdnsd
SOURCES = $(wildcard src/*.c) 
OBJS = $(SOURCES:.c=.o)

CFLAGS = -Wall -O0 -g
LDFLAGS = 

all: $(TARGET) dbgen/dbgen

%.o: %.c
	$(CC) -o $@ $(CFLAGS) -c $<

$(TARGET): $(OBJS)
	$(CC) -o $@ $(LDFLAGS) $^ -lpthread

dbgen/dbgen:
	$(MAKE) -C dbgen

clean:
	rm -rf $(TARGET) $(OBJS)
	$(MAKE) -C dbgen clean
