LINK_TARGET=meta.exe

OBJS =  \
    meta.o \
 
 REBUILDABLES = $(OBJS) $(LINK_TARGET)

all : $(LINK_TARGET)
	echo all done
    
$(LINK_TARGET) : $(OBJS)
	gcc \
     -L/home/pi/code/c++/libav/libav-11.6/libavformat \
     -L/home/pi/code/c++/libav/libav-11.6/libavutil \
     -g -o $@ $^ \
     -lavutil \
    -lavformat
    
    
%.o : %.c
	gcc -g -o $@ \
    -I/home/pi/code/c++/libav/libav-11.6 \
    -c $<