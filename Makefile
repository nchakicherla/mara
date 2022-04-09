CC=gcc
TAGS=-Wall -Wextra -Wpedantic --std=c99
DEBUG_PARAM = -DLOG_WARNING -DLOG_ERROR -DENABLE_LOG #-DLOG_SUCCESS #-DPRINTLOG

mara: ./src/mara.c
	$(CC) $(TAGS) $(DEBUG_PARAM) ./src/mara.c -o ./mara.o
