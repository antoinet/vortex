include ../Makefile.inc

all: shellcode.h

shellcode.h: shellcode.bin
	$(MSFHOME)/msfencode -e x86/call4_dword_xor -t c -i shellcode.bin -o shellcode.h
	$(SED) -i 's/buf/shellcode/g' shellcode.h

shellcode.bin: shellcode.o
	$(OBJCOPY) -O binary -j .text -S shellcode.o shellcode.bin

shellcode.o: shellcode.s
	$(AS) --32 -march=i386 -o shellcode.o shellcode.s

clean:
	rm shellcode.o shellcode.bin shellcode.h
	
