SOURCES		=	main.cpp 

TARGET		=	../bin/random

all:
	g++ -fpermissive -w -o ${TARGET} ${SOURCES}

debug:
	g++ -g3 -fpermissive -w -o ${TARGET} ${SOURCES}

strict:
	g++ -Wall -o ${TARGET} ${SOURCES}

dist-clean:
	rm -f ${TARGET}
