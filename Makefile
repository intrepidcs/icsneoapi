CXX=g++
CC=gcc
CFLAGS=-g -c -fpic -fvisibility=hidden -fvisibility-inlines-hidden -O2 -Isrc/
LDFLAGS=-O2 -s -Wl,--unresolved-symbols=report-all -Wl,--no-allow-shlib-undefined -shared -fvisibility=hidden -fvisibility-inlines-hidden
LIBS=-lpthread -lftdi -lrt -lpcap
AR=ar

all: shared lib

shared: CFTDILib.o cicsneoVI.o icsneoLinuxAPI.o OCriticalSection.o OEvent.o OSAbstraction.o OThread.o
	$(CXX) $(LDFLAGS) -o libicsneoapi.so CFTDILib.o cicsneoVI.o icsneoLinuxAPI.o OCriticalSection.o OEvent.o OSAbstraction.o OThread.o $(LIBS)

lib: CFTDILib.o cicsneoVI.o icsneoLinuxAPI.o OCriticalSection.o OEvent.o OSAbstraction.o OThread.o
	$(AR) rcs libicsneoapi.a CFTDILib.o cicsneoVI.o icsneoLinuxAPI.o OCriticalSection.o OEvent.o OSAbstraction.o OThread.o

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

clean:
	rm -rf *.o libicsneo.a libicsneoapi.so
