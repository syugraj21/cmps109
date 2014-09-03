# $Id: Makefile,v 1.3 2013-02-12 15:09:10-08 - - $

# By default, gmake sets the variable ${CXX}=g++ on a Linux system.
# To use Sun Studio C++ (CC), set the variable CXX=CC, either on
# the command line when calling make, or in an environment variable.

MKFILE      = Makefile
DEPFILE     = ${MKFILE}.dep
NOINCL      = ci clean spotless
NEEDINCL    = ${filter ${NOINCL}, ${MAKECMDGOALS}}
GMAKE       = ${MAKE} --no-print-directory

COMPILECPP  = g++ -g -O0 -Wall -Wextra -std=gnu++0x
MAKEDEPCPP  = g++ -MM

CPPHEADER   = listmap.h trace.h util.h xcompare.h xpair.h
CPPSOURCE   = ${CPPHEADER:.h=.cpp} main.cpp
TCCSHARED   = templates.tcc
ALLCPPSRC   = ${CPPHEADER} ${CPPSOURCE} ${TCCSHARED}
OBJECTS     = ${CPPSOURCE:.cpp=.o}
EXECBIN     = keyvalue
OTHERS      = ${MKFILE} README
ALLSOURCES  = ${ALLCPPSRC} ${OTHERS}

LISTING     = ../asg4.code.listing.ps
CLASS       = cmps109-wm.w13
PROJECT     = asg4

all : ${EXECBIN}
	- checksource ${ALLSOURCES}

${EXECBIN} : ${OBJECTS}
	${COMPILECPP} -o $@ ${OBJECTS}

%.o : %.cpp
	cid + $<
	${COMPILECPP} -c $<

ci : ${ALLSOURCES}
	- checksource ${ALLSOURCES}
	cid + ${ALLSOURCES}

lis : ${ALLSOURCES}
	mkpspdf ${LISTING} ${ALLSOURCES} ${DEPFILE}

clean :
	- rm ${OBJECTS} ${DEPFILE} core ${EXECBIN}.errs

spotless : clean
	- rm ${EXECBIN} ${LISTING} ${LISTING:.ps=.pdf}

submit : ${ALLSOURCES}
	- checksource ${ALLSOURCES}
	submit ${CLASS} ${PROJECT} ${ALLSOURCES}

dep : ${CPPSOURCE} ${CPPHEADER}
	@ echo "# ${DEPFILE} created `LC_TIME=C date`" >${DEPFILE}
	${MAKEDEPCPP} ${CPPSOURCE} >>${DEPFILE}

${DEPFILE} :
	@ touch ${DEPFILE}
	${GMAKE} dep

again :
	${GMAKE} spotless dep ci all lis

ifeq (${NEEDINCL}, )
include ${DEPFILE}
endif


