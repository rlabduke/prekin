#
CFLAGS = -I/opt/local/include 

ifeq ($(MAKECMDGOALS),debug)
CFLAGS = -g -I/opt/local/include  
endif

LIBS = -L/usr/lib -L/opt/local/lib -lXmu -lXm -lXt -lSM -lICE -lXext -lX11

ifeq ($(MAKECMDGOALS),nogui)
CFLAGS = -D NOGUI -I/opt/local/include
LIBS = -L/usr/lib -L/opt/local/lib -lXmu -lXt -lSM -lICE -lXext -lX11
endif

FIN =  -lm -pthread #for RH9.0, ok for RH7.3 & RH8.0


# ------------------------------------------------------------------------
# Variables specific to clients created by this Makefile
PKINHEADERS = PKIN.h PKINhdr.h PKINCRTL.h PKINMENU.h PKINDLOG.h PKINRIBB.h PKINHY36.h

SRCS = PKINANGL.c PKINCRTL.c PKINCSBS.c PKINCSUB.c PKINCOUT.c PKINMENU.c PKINROTL.c PKINDLOG.c PKINFILE.c PKINHBND.c PKINHY36.c PKININPT.c PKININIT.c PKINRIBB.c PKINRNGE.c PKINRSUB.c PKINSCRT.c PKINSPOS.c PKINUTIL.c PUXMFILE.c PUXMMAIN.c PUXMINIT.c PUXMDLOG.c PUXMMENU.c PUXMOUTP.c PUXMTEXT.c PUXMLNX.c
OBJS = PKINANGL.o PKINCRTL.o PKINCSBS.o PKINCSUB.o PKINCOUT.o PKINMENU.o PKINROTL.o PKINDLOG.o PKINFILE.o PKINHBND.o PKINHY36.o PKININPT.o PKININIT.o PKINRIBB.o PKINRNGE.o PKINRSUB.o PKINSCRT.o PKINSPOS.o PKINUTIL.o PUXMFILE.o PUXMMAIN.o PUXMINIT.o PUXMDLOG.o PUXMMENU.o PUXMOUTP.o PUXMTEXT.o PUXMLNX.o

# ------------------------------------------------------------------------
# Commands specific to clients created by this Makefile 

prekin: $(OBJS)
	cc -o prekin $(CFLAGS) $(OBJS) -L/opt/local/lib $(LIBS) -ldl $(FIN)

debug: $(OBJS)
	cc -o prekin $(CFLAGS) $(OBJS) -L/opt/local/lib $(LIBS) -ldl $(FIN)

nogui: $(OBJS)
	cc -o prekin $(CFLAGS) $(OBJS) -L/opt/local/lib $(LIBS) -ldl $(FIN)

clean:
	rm *.o
# ------------------------------------------------------------------------
# Dependencies  (presume .o<-.c by standard cc compiler)
# ?? gnu doesn't seem to make the .o<-.c connection automatically 

PKINANGL.o: $(PKINHEADERS) 
PKINCRTL.o: $(PKINHEADERS) 
PKINCSBS.o: $(PKINHEADERS) 
PKINCSUB.o: $(PKINHEADERS) 
PKINCOUT.o: $(PKINHEADERS) 
PKINMENU.o: $(PKINHEADERS) 
PKINROTL.o: $(PKINHEADERS) 
PKINDLOG.o: $(PKINHEADERS) 
PKINFILE.o: $(PKINHEADERS) 
PKINHBND.o: $(PKINHEADERS) 
PKINHY36.o: $(PKINHEADERS) 
PKININPT.o: $(PKINHEADERS) 
PKININIT.o: $(PKINHEADERS) 
PKINRIBB.o: $(PKINHEADERS) 
PKINRNGE.o: $(PKINHEADERS) 
PKINRSUB.o: $(PKINHEADERS) 
PKINSCRT.o: $(PKINHEADERS)
PKINSPOS.o: $(PKINHEADERS) 
PKINUTIL.o: $(PKINHEADERS) 
PUXMFILE.o: $(PKINHEADERS) 
PUXMMAIN.o: $(PKINHEADERS) 
PUXMINIT.o: $(PKINHEADERS) 
PUXMDLOG.o: $(PKINHEADERS) 
PUXMMENU.o: $(PKINHEADERS) 
PUXMOUTP.o: $(PKINHEADERS) 
PUXMTEXT.o: $(PKINHEADERS)
PUXMLNX.o:  $(PKINHEADERS)


