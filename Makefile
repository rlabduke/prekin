# prekin 

ifeq ($(MAKECMDGOALS),debug)
	CFLAGS = -g  -isysroot /Developer/SDKs/MacOSX10.4u.sdk -arch i386 -arch ppc \
                 -mmacosx-version-min=10.4 \
	         -I/usr/local/include/ -I/usr/X11R6/include/ -headerpad_max_install_names
else
	CFLAGS = -isysroot /Developer/SDKs/MacOSX10.4u.sdk -arch i386 -arch ppc \
                 -mmacosx-version-min=10.4 \
	         -I/usr/local/include/ -I/usr/X11R6/include/ -headerpad_max_install_names
endif
   LIBS = -L/Applications/Mage.app/Contents/lib -L/usr/X11R6/lib \
          -lXm -lXt -lX11 -lSM -lICE -lpng.3 -ljpeg.62

ifeq ($(MAKECMDGOALS),nogui)
   CFLAGS = -D NOGUI -isysroot /Developer/SDKs/MacOSX10.4u.sdk -arch i386 -arch ppc \
            -mmacosx-version-min=10.4 \
            -I/usr/local/include/ -I/usr/X11R6/include/ -headerpad_max_install_names
   LIBS = -L/usr/X11R6/lib -lXt -lX11 -lSM -lICE
endif

ifeq ($(MAKECMDGOALS),debugnogui)
   CFLAGS = -g -D NOGUI -isysroot /Developer/SDKs/MacOSX10.4u.sdk -arch i386 -arch ppc \
            -mmacosx-version-min=10.4 \
            -I/usr/local/include/ -I/usr/X11R6/include/ -headerpad_max_install_names
   LIBS = -L/usr/X11R6/lib -lXt -lX11 -lSM -lICE
endif

   PKINHEADERS = PKIN.h PKINhdr.h PKINCRTL.h PKINMENU.h PKINDLOG.h PKINRIBB.h PKINHY36.h

   SRCS = PKINANGL.c PKINCRTL.c PKINCSBS.c PKINCSUB.c PKINCOUT.c \
          PKINMENU.c PKINROTL.c PKINDLOG.c PKINFILE.c PKINHBND.c \
          PKINHY36.c PKININPT.c PKININIT.c PKINRIBB.c PKINRNGE.c \
          PKINRSUB.c PKINSCRT.c PKINSPOS.c PKINUTIL.c PUXMFILE.c \
          PUXMMAIN.c PUXMINIT.c PUXMDLOG.c PUXMMENU.c PUXMOUTP.c \
          PUXMTEXT.c PUXMOSX.c
          
   OBJS = PKINANGL.o PKINCRTL.o PKINCSBS.o PKINCSUB.o PKINCOUT.o \
          PKINMENU.o PKINROTL.o PKINDLOG.o PKINFILE.o PKINHBND.o \
          PKINHY36.o PKININPT.o PKININIT.o PKINRIBB.o PKINRNGE.o \
          PKINRSUB.o PKINSCRT.o PKINSPOS.o PKINUTIL.o PUXMFILE.o \
          PUXMMAIN.o PUXMINIT.o PUXMDLOG.o PUXMMENU.o PUXMOUTP.o \
          PUXMTEXT.o PUXMOSX.c

   FIN =  -lm -Wl,-syslibroot,/Developer/SDKs/MacOSX10.4u.sdk \
          -arch i386 -arch ppc -bind_at_load -mmacosx-version-min=10.4

# ------------------------------------------------------------------------
# Commands specific to clients created by this Makefile 

prekin: $(OBJS)
	cc -o prekin $(CFLAGS) $(OBJS) $(LIBS) $(FIN)

debug: $(OBJS)
	cc -o prekin $(CFLAGS) $(OBJS) $(LIBS) $(FIN)

nogui: $(OBJS)
	cc -o prekin $(CFLAGS) $(OBJS) $(LIBS) $(FIN)

debugnogui: $(OBJS)
	cc -o prekin $(CFLAGS) $(OBJS) $(LIBS) $(FIN)

clean:
	rm -f *.o

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
PUXMOSX.o:  $(PKINHEADERS)


