LDFLAGS+=-lm

.PHONY: run clean all

universe_to_json : universe.o

run : universe_to_json
	@./$< -

all : run

clean :
	$(RM) *.o
	$(RM) universe_to_json

