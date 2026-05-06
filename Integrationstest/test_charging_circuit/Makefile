DEVICE = atmega2560
CLOCK  = 16000000UL

COMMON_SRC = ../../Common/src
COMMON_LIB = ../../Common/lib

VPATH = $(COMMON_SRC)

OBJECTS = main.o uart.o timer.o charging_circuit.o adc.o

COMPILE_COMMON = -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) -I. -I$(COMMON_LIB)
COMPILE_C = avr-gcc $(COMPILE_COMMON)
COMPILE_CXX = avr-g++ $(COMPILE_COMMON)

PORT = ...

find:
	ls /dev/cu.* | grep usbmodem | head -n 1

%.o: %.c
	$(COMPILE_C) -c $< -o $@

%.o: %.cpp
	$(COMPILE_CXX) -c $< -o $@

main.elf: $(OBJECTS)
	$(COMPILE_CXX) -o $@ $(OBJECTS)

main.hex: main.elf
	rm -f main.hex
	avr-objcopy -O ihex -R .eeprom main.elf main.hex

flash: main.hex
	avrdude -c wiring -p m2560 -P $(PORT) -b 115200 -D -U flash:w:main.hex:i

clean_all:
	rm -f *.hex *.obj *.o *.elf

CXX = g++

COMMON_SRC = ../../Common/src
COMMON_LIB = ../../Common/lib

TEST_CXXFLAGS = -Wall -Wextra -std=c++17 \
	-I. \
	-I$(COMMON_LIB)

test: test_charging_circuit
	./test_charging_circuit

test_charging_circuit: test_charging_circuit.cpp \
	$(COMMON_SRC)/charging_circuit.cpp
	$(CXX) $(TEST_CXXFLAGS) -o $@ $^

clean_test:
	rm -f test_charging_circuit
