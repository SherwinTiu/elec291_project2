SHELL=cmd
CC = xc32-gcc
OBJCPY = xc32-bin2hex
ARCH = -mprocessor=32MX130F064B
OBJ = PushButton.o
PORTN=$(shell type COMPORT.inc)

PushButton.elf: $(OBJ)
	$(CC) $(ARCH) -o PushButton.elf PushButton.o -mips16 -DXPRJ_default=default -legacy-libc -Wl,-Map=PushButton.map
	$(OBJCPY) PushButton.elf
	@echo Success!
   
PushButton.o: PushButton.c
	$(CC) -mips16 -g -x c -c $(ARCH) -MMD -o PushButton.o PushButton.c -DXPRJ_default=default -legacy-libc

clean:
	@del *.o *.elf *.hex *.map *.d 2>NUL
	
LoadFlash:
	@taskkill /f /im putty.exe /t /fi "status eq running" > NUL
	pro32 -p PushButton.hex
	@cmd /c start putty -serial $(PORTN) -sercfg 115200,8,n,1,N

putty:
	@taskkill /f /im putty.exe /t /fi "status eq running" > NUL
	@cmd /c start putty -serial $(PORTN) -sercfg 115200,8,n,1,N

dummy: PushButton.hex PushButton.map
	$(CC) --version
