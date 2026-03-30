FLAGS = -Wall -g
DEPENDENCIES = shared.h process_manager.h worker.h file_processing.h aggregator.h output.h

all: program

program: main.o process_manager.o worker.o file_processing.o aggregator.o output.o
	gcc ${FLAGS} -o $@ $^

main.o: main.c ${DEPENDENCIES}
	gcc ${FLAGS} -c $<

process_manager.o: process_manager.c ${DEPENDENCIES}
	gcc ${FLAGS} -c $<

worker.o: worker.c ${DEPENDENCIES}
	gcc ${FLAGS} -c $<

file_processing.o: file_processing.c ${DEPENDENCIES}
	gcc ${FLAGS} -c $<

aggregator.o: aggregator.c ${DEPENDENCIES}
	gcc ${FLAGS} -c $<

output.o: output.c ${DEPENDENCIES}
	gcc ${FLAGS} -c $<

clean:
	rm -f *.o program *.gch