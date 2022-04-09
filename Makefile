CC=gcc
TAGS=-Wall -Wextra -Wpedantic --std=c99
DEBUG_PARAM = -DLOGGING -DWARN -DERROR #-DSUCCESS -DPRINTLOG

mara: ./src/mara.c
	$(CC) $(TAGS) $(DEBUG_PARAM) ./src/mara.c -o ./mara.o
