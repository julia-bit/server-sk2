LIBS=-lsqlite3 -lpthread
HEADERS=wsServer/*.h cJSON_lib/cJSON.h http_parser/picohttpparser.h requests.h operations.h chat_actions.h hashtable/*.h messages_control.h
FILES=main.c wsServer/*.c cJSON_lib/cJSON.c http_parser/picohttpparser.c requests.c operations.c chat_actions.c hashtable/*.c messages_control.c
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
