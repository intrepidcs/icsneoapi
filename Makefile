CXX=g++
CC=gcc
CFLAGS=-g -c -fpic -fvisibility=hidden -fvisibility-inlines-hidden -O2 -Isrc/
LDFLAGS=-O2 -s -Wl,--unresolved-symbols=report-all -Wl,--no-allow-shlib-undefined -shared -fvisibility=hidden -fvisibility-inlines-hidden
LIBS=-lpthread
AR=ar

# Some distros name it 'libftdi', others 'libftdi1' We have to try them both
# This dance is so that we evaluate pkg-config once, instead of each
# invocation of $(CC)
FTDI_CFLAGS := $(shell pkg-config --cflags libftdi 2>/dev/null)
FTDI_LIBS   := $(shell pkg-config --libs-only-l libftdi 2>/dev/null)
ifeq ($(FTDI_LIBS),)
	FTDI_CFLAGS := $(shell pkg-config --cflags libftdi1)
	FTDI_LIBS   := $(shell pkg-config --libs-only-l libftdi1)
endif
CFLAGS += $(FTDI_CFLAGS)
LIBS   += $(FTDI_LIBS)

all: shared lib

shared: CFTDILib.o cicsneoVI.o icsneoLinuxAPI.o OCriticalSection.o OEvent.o OSAbstraction.o OThread.o NeoviSerialNumberFormatter.o
	$(CXX) $(LDFLAGS) -o libicsneoapi.so CFTDILib.o cicsneoVI.o icsneoLinuxAPI.o OCriticalSection.o OEvent.o OSAbstraction.o OThread.o NeoviSerialNumberFormatter.o $(LIBS)

lib: CFTDILib.o cicsneoVI.o icsneoLinuxAPI.o OCriticalSection.o OEvent.o OSAbstraction.o OThread.o NeoviSerialNumberFormatter.o
	$(AR) rcs libicsneoapi.a CFTDILib.o cicsneoVI.o icsneoLinuxAPI.o OCriticalSection.o OEvent.o OSAbstraction.o OThread.o NeoviSerialNumberFormatter.o

CFTDILib.o: src/CFTDILib.cpp
	$(CXX) $(CFLAGS) src/CFTDILib.cpp

cicsneoVI.o: src/cicsneoVI.cpp
	$(CXX) $(CFLAGS) src/cicsneoVI.cpp

icsneoLinuxAPI.o: src/icsneoLinuxAPI.cpp
	$(CXX) $(CFLAGS) src/icsneoLinuxAPI.cpp

OCriticalSection.o: src/OCriticalSection.cpp
	$(CXX) $(CFLAGS) src/OCriticalSection.cpp

OEvent.o: src/OEvent.cpp
	$(CXX) $(CFLAGS) src/OEvent.cpp

OSAbstraction.o: src/OSAbstraction.cpp
	$(CXX) $(CFLAGS) src/OSAbstraction.cpp

OThread.o: src/OThread.cpp
	$(CXX) $(CFLAGS) src/OThread.cpp

NeoviSerialNumberFormatter.o: src/NeoviSerialNumberFormatter.cpp
	$(CXX) $(CFLAGS) src/NeoviSerialNumberFormatter.cpp

clean:
	rm -rf *.o libicsneo.a libicsneoapi.so
