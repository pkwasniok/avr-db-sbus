# Configuration
MCU = avr128db28
PROGRAMMER = snap

# Tools
GCC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

# Flags
CFLAGS = -B ./lib/atpack/gcc/dev/$(MCU)
CFLAGS += -isystem ./lib/atpack/include
CFLAGS += -mmcu=$(MCU)
CFLAGS += -Os

# Others
OUTDIR = .build



build: firmware.bin

upload: build
	avrdude -c $(PROGRAMMER) -p $(MCU) -U flash:w:$(OUTDIR)/firmware.bin:r

monitor:
	sudo minicom -D /dev/ttyACM1 -b 115200 --ansi

docs:
	doxygen

clean:
	rm -r $(OUTDIR)/*
	rm -r ./.docs/*



firmware.bin: firmware.elf
	$(OBJCOPY) -O binary $(OUTDIR)/firmware.elf $(OUTDIR)/firmware.bin

firmware.elf: src/main.o src/usart.o src/sbus.o
	$(GCC) $(CFLAGS) $(addprefix $(OUTDIR)/, $(^)) -o $(OUTDIR)/$(@)

%.o: %.c
	mkdir -p $(OUTDIR)/$(@D)
	$(GCC) $(CFLAGS) -c $(^) -o $(OUTDIR)/$(@)
