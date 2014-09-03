# $Id: Makefile,v 1.5 2011-04-19 23:14:55-07 - - $

JAVASRC      = chatter.java client.java gui.java server.java
SOURCES      = ${JAVASRC} Makefile README
MAINCLASS    = chatter
CLASSES      = ${JAVASRC:.java=.class}
INNERCLASSES = chatter\$$options.class \
               client\$$sender.class client\$$receiver.class \
               gui\$$main_gui_client.class gui\$$confirm.class \
               gui\$$sender.class gui\$$receiver.class \
               server\$$main_server.class server\$$new_client.class \
               server\$$receiver.class server\$$sender.class \
               server\$$user_manger.class server\$$queue_manger.class\
               server\$$output_manger.class
JARCLASSES   = ${CLASSES} ${INNERCLASSES}
JARFILE      = chatter
LISTING      = ../asg5-chatter-threads.code.ps

all : ${JARFILE}

${JARFILE} : ${CLASSES}
	echo Main-class: ${MAINCLASS} >Manifest
	jar cvfm ${JARFILE} Manifest ${JARCLASSES}
	- rm Manifest
	chmod +x ${JARFILE}

%.class : %.java
	javac $<

lis : ${SOURCES} all
	mkpspdf ${LISTING} ${SOURCES}

clean :
	- rm ${JARCLASSES}

spotless : clean
	- rm ${JARFILE}

ci : ${SOURCES}
	checksource ${SOURCES}
	cid + ${SOURCES}

submit : ${SOURCES}
	submit cmps109-wm.w13 asg5 ${SOURCES}

again :
	S{MAKE} spotless ci all lis
