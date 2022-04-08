CC=gcc
TAGS=-Wall -Wextra -Wpedantic --std=c99
DEBUG_PARAM = -DPRINTLOG -DWARN -DERROR #-DSUCCESS

mara: ./src/mara.c
	$(CC) $(TAGS) $(DEBUG_PARAM) ./src/mara.c -o ./mara.o
