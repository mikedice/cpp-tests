LINK_TARGET=meta.exe

OBJS =  \
    meta.o \

INCLUDEPATHS = \
    -I/home/pi/code/c++/libav/libav-11.6 \

LIBPATHS = \
     -L/home/pi/code/c++/libav/libav-11.6/libavformat \
     -L/home/pi/code/c++/libav/libav-11.6/libavutil

LIBS = \
      -lavutil \
      -lavformat

 REBUILDABLES = $(OBJS) $(LINK_TARGET)

all : $(LINK_TARGET)
	echo all done

clean :
	rm $(LINK_TARGET)
    
$(LINK_TARGET) : $(OBJS)
	gcc \
    $(LIBPATHS) \
    -g -o $@ $^ \
    $(LIBS)

%.o : %.c
	gcc -g -o $@ \
    $(INCLUDEPATHS) \
    -c $<