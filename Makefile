# MCU
MCU   = atmega328p
F_CPU = 16000000UL  
BAUD  = 115200
PORT  = /dev/ttyACM0

LIBDIR = /usr/avr/include

# Programmer
PROGRAMMER_TYPE = arduino
PROGRAMMER_ARGS = -P $(PORT) -b $(BAUD) 

# Programs
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
AVRSIZE = avr-size
AVRDUDE = avrdude

# Project folder
TARGET = main
# TARGET = $(lastword $(subst /, ,$(CURDIR)))

SRC_DIR = src
INC_DIR = inc
OBJ_DIR = obj

SRCS = $(wildcard $(SRC_DIR)/*.c)
INCS = $(wildcard $(INC_DIR)/*.h)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# C Flags
CPPFLAGS = -DF_CPU=$(F_CPU) -DBAUD=$(BAUD) -I$(INC_DIR) -I$(LIBDIR)
CFLAGS  = -Os -g -std=gnu99 -Wall
CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums 
CFLAGS += -ffunction-sections -fdata-sections 

# LD Flags
LDFLAGS  = -Wl,-Map,$(TARGET).map 
LDFLAGS += -Wl,--gc-sections 
TARGET_ARCH = -mmcu=$(MCU)

# Generate Objects files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCS) Makefile
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c -o $@ $<

# Generate ELF
$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) $(TARGET_ARCH) $^ $(LDLIBS) -o $@

# Generate HEX
$(TARGET).hex: $(TARGET).elf
	 $(OBJCOPY) -j .text -j .data -O ihex $< $@

## These targets don't have files named after them
.PHONY: all size clean flash

all: $(TARGET).hex 

debug:
	@echo
	@echo "Source files:"  $(SRCS)
	@echo "Obkect files:"  $(OBJS)
	@echo "         MCU:"  $(MCU)
	@echo "       F_CPU:"  $(F_CPU)
	@echo "        BAUD:"  $(BAUD)
	@echo	

size:  $(TARGET).elf
	$(AVRSIZE) -C --mcu=$(MCU) $(TARGET).elf

clean:
	rm -rf $(OBJ_DIR) *.map *.hex *.elf

flash: $(TARGET).hex
	$(AVRDUDE) -c $(PROGRAMMER_TYPE) -p $(MCU) $(PROGRAMMER_ARGS) -U flash:w:$<
