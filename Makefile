TARGET = testdnsd
SOURCES = $(wildcard src/*.c) 
OBJS = $(SOURCES:.c=.o)

CFLAGS = -Wall -O0 -g
LDFLAGS = 

all: $(TARGET)

%.o: %.c
	$(CC) -o $@ $(CFLAGS) -c $<

$(TARGET): $(OBJS)
	$(CC) -o $@ $(LDFLAGS) $^

clean:
	rm -rf $(TARGET) $(OBJS)
