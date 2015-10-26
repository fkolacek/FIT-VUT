CXX=$(shell /usr/bin/which g++-4.8 2>/dev/null || /usr/bin/which g++)
CXXFLAGS=
INCLUDEDIR=$(shell test -e /usr/include/simlib && echo "-I/usr/include/simlib")
LDFLAGS=-lsimlib -lm

SRCS=main.cc config.cc scheduler.cc bee.cc
RESULT=ims

all:
	${CXX} ${INCLUDEDIR} ${CXXFLAGS} ${LDFLAGS} ${SRCS} -o ${RESULT}

run:
	./${RESULT}
	
debug: all
	time ./${RESULT} | tee ims.log && grep CSV ims.output > ims.csv
	cp ims.csv /var/www/html/

clean:
	rm -f ${RESULT} *.log *.tar.gz *.output *.csv
	
archive:
	tar cvzf 03_xkolac12_xmatya03.tar.gz Makefile *.cc *.h *.pdf
