CC = iccavr
LIB = ilibw
CFLAGS =  -e -D__ICC_VERSION=722 -DATMega16  -l -g -MLongJump -MHasMul -MEnhanced -Wf-const_is_flash -DCONST="" 
ASFLAGS = $(CFLAGS) 
LFLAGS =  -g -e:0x4000 -ucrtatmega.o -bfunc_lit:0x54.0x4000 -dram_end:0x45f -bdata:0x60.0x45f -dhwstk_size:16 -beeprom:0.512 -fihx_coff -S2
FILES = main.o

FIRST:	$(FILES) lcd.o
	$(CC) -o FIRST lcd.o $(LFLAGS) @FIRST.lk   -lcatm128
main.o: D:\iccv7avr\include\iom16v.h D:\iccv7avr\include\macros.h D:\iccv7avr\include\AVRdef.h
main.o:	main.c
	$(CC) -c $(CFLAGS) main.c
lcd.o: lcd.h
	$(CC) -c $(CFLAGS) lcd.c
