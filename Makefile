TARGET = testdnsd
SOURCES = $(wildcard src/*.c) 
OBJS = $(SOURCES:.c=.o)
DEPS = $(SOURCES:.c=.d)

CFLAGS = -Wall -O0 -g
CPPFLAGS = 
LDFLAGS = 

# ###################################################################

CPPFLAGS += -MMD -MP

all: $(TARGET) dbgen/dbgen

%.o: %.c
	$(CC) -o $@ $(CPPFLAGS) $(CFLAGS) -c $<

#%.d: %.c
#	@set -e; rm -f $@; \
#	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
#	echo -n 'src/' > $@; \
#	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ >> $@; \
#	rm -f $@.$$$$

$(TARGET): $(OBJS)
	$(CC) -o $@ $(LDFLAGS) $^ -lpthread

dbgen/dbgen:
	$(MAKE) -C dbgen

clean:
	rm -rf $(TARGET) $(OBJS) $(DEPS)
	$(MAKE) -C dbgen clean

-include $(DEPS)
