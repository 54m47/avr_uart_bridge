# Project Name
PROJECT = uart_bridge

# AVR Device & Clock Speed
MCU = atmega2560
F_CPU = 16000000UL

# AVR-GCC Compiler & Flags
CC = avr-gcc
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os
CFLAGS += -IminiAVRfreeRTOS

# Source Files
SRC = main.c
SRC += $(wildcard miniAVRfreeRTOS/*.c)

# Object Files Directory
OBJDIR = obj

# Object Files
OBJ = $(SRC:%.c=$(OBJDIR)/%.o)

# AVRDUDE Programming
PROGRAMMER = wiring
PORT = COM7
BAUD = 9600
AVRDUDE = avrdude -C "D:\Programmes\Arduino\hardware\tools\avr\etc\avrdude.conf" -p atmega2560 -c wiring -P COM4 -b 115200 -D -U flash:w:uart_bridge.hex:i

# Targets
.PHONY: all clean flash

all: $(PROJECT).hex

$(PROJECT).hex: $(PROJECT).elf
	avr-objcopy -O ihex -R .eeprom $< $@

$(PROJECT).elf: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Notice the use of double quotes for paths which might include spaces
$(OBJDIR)/%.o: %.c
	@if not exist "$(OBJDIR)" mkdir "$(OBJDIR)"
	@if not exist "$(OBJDIR)\miniAVRfreeRTOS" mkdir "$(OBJDIR)\miniAVRfreeRTOS"
	$(CC) $(CFLAGS) -c $< -o $@

flash: $(PROJECT).hex
	$(AVRDUDE) -U flash:w:$<:i

clean:
	del /Q "$(OBJDIR)\*.o"
	del /Q "$(OBJDIR)\miniAVRfreeRTOS\*.o"
	del $(PROJECT).elf
	del $(PROJECT).hex
