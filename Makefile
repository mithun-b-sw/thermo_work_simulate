CC = gcc
LDLIBS += -lm

objects := $(patsubst %.c,%.o,$(wildcard *.c))
target = thermodyn_cycle
CFLAGS += -I./

.PHONY: clean

%.o: %.c
	$(CC) $< -c $(CFLAGS) -o $@

$(target): $(objects)
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@ $(LDLIBS)

cov: clean
	make CFLAGS=--coverage
	./$(target) 1
	geninfo ./ -b ./ -o lcov.info
	genhtml lcov.info -o ./coverage
	@ echo "\nopen ./coverage/index.html in browser"

clean:
	- $(RM) *.o $(target) *.gcda *.gcno *.gcov lcov.info
	- $(RM) -r coverage

# $< to consider one dependancy at a time, whereas $^ for all at once.
# $@ is for target
