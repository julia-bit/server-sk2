HEADERS=wsServer/*.h cJSON.h
FILES=main.c wsServer/*.c cJSON.c
CFLAGS   =  -Wall -Wextra -O2
CFLAGS  +=  $(INCLUDE) -std=c99 -pthread -pedantic

main: $(FILES) $(HEADERS)
		gcc -g $(CFLAGS) -o main $(FILES)

ifeq ($(VERBOSE_EXAMPLES), no)
	CFLAGS += -DDISABLE_VERBOSE
endif

.PHONY: clean

clean:
		rm -f main
