CC=gcc
OBJS=eurovision.o state.o judge.o statePoints.o eurovisionHelper.o map.o mapNode.o
EXEC=eurovision
DEBUG=-g
LIB=libmtm.a
CFLAGS=-std=c99 -Wall -pedantic-errors -Werror $(DEBUG) -L.

$(EXEC) : $(OBJS)
	$(CC) $(DEBUG_FLAGS) $(OBJS) -o $@ $(LIB)

eurovision.o: eurovision.c eurovision.h list.h map.h state.h eurovisionHelper.h judge.h statePoints.h

judge.o: judge.c judge.h map.h eurovisionHelper.h

state.o: state.c state.h map.h eurovisionHelper.h

statePoints.o: statePoints.c statePoints.h eurovisionHelper.h

eurovisionHelper.o: eurovisionHelper.c eurovisionHelper.h

map.o: map.c map.h mapNode.h

mapNode.o: mapNode.c mapNode.h

clean:
	rm -f $(OBJS)

