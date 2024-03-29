CC = gcc

objects := $(patsubst %.c,%.o,$(wildcard *.c))
target = thermodyn_cycle
CFLAGS += -I./

.PHONY: clean

%.o: %.c
	$(CC) $< -c $(CFLAGS) -o $@

$(target): $(objects)
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@

cov: clean
	make CFLAGS=--coverage
	./$(target)
	geninfo ./ -b ./ -o lcov.info
	genhtml lcov.info -o ./coverage
	@ echo "\nopen ./coverage/index.html in browser"

clean:
	- $(RM) *.o $(target) *.gcda *.gcno *.gcov lcov.info
	- $(RM) -r coverage

# $< to consider one dependancy at a time, whereas $^ for all at once.
# $@ is for target
