CC = gcc

objects := $(patsubst %.c,%.o,$(wildcard *.c))
target = thermodyn_cycle
CFLAGS += -I./

.PHONY: clean

%.o: %.c
	$(CC) $< -c $(CFLAGS) -o $@

$(target): $(objects)
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@

clean:
	- rm *.o $(target)

# $< to consider one dependancy at a time, whereas $^ for all at once.
# $@ is for target
