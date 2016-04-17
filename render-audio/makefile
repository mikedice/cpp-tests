LINK_TARGET=read.exe

OBJS =  \
    il_ffmpeg_demux_render_audio.o \
 
 REBUILDABLES = $(OBJS) $(LINK_TARGET)

all : $(LINK_TARGET)
	echo all done
    
$(LINK_TARGET) : $(OBJS)
	gcc -g -o $@ $^
    
%.o : %.c
	gcc -g -o $@ \
    -I/opt/vc/include \
    -I/opt/vc/include/interface/vmcs_host/khronos/IL/ \
    -I/opt/vc/include/interface/vcos/pthreads/ \
    -I/opt/vc/include/interface/vmcs_host \
    -I/opt/vc/include/interface/vmcs_host/linux \
    -I/opt/vc/include/interface/vcos \
    -I/opt/vc/src/hello_pi/libs/ilclient \
    -I/home/pi/code/c++/libav/libav-11.6 \
    -c $<