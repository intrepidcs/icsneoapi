CXX=g++
CC=gcc
CFLAGS=-g -c -I. -I./OsAbstraction
LDFLAGS=
AR=ar

all: lib

lib: CFTDILib.o cicsneoVI.o icsneoLinuxAPI.o OsAbstraction/OCriticalSection.o OsAbstraction/OEvent.o OsAbstraction/OSAbstraction.o OsAbstraction/OThread.o
	$(AR) rcs libicsneo.a CFTDILib.o cicsneoVI.o icsneoLinuxAPI.o OCriticalSection.o OEvent.o OSAbstraction.o OThread.o

CFTDILib.o: CFTDILib.cpp
	$(CXX) $(CFLAGS) CFTDILib.cpp

cicsneoVI.o: cicsneoVI.cpp
	$(CXX) $(CFLAGS) cicsneoVI.cpp

icsneoLinuxAPI.o: icsneoLinuxAPI.cpp
	$(CXX) $(CFLAGS) icsneoLinuxAPI.cpp

OsAbstraction/OCriticalSection.o: OsAbstraction/OCriticalSection.cpp
	$(CXX) $(CFLAGS) OsAbstraction/OCriticalSection.cpp

OsAbstraction/OEvent.o: OsAbstraction/OEvent.cpp
	$(CXX) $(CFLAGS) OsAbstraction/OEvent.cpp

OsAbstraction/OSAbstraction.o: OsAbstraction/OSAbstraction.cpp
	$(CXX) $(CFLAGS) OsAbstraction/OSAbstraction.cpp

OsAbstraction/OThread.o: OsAbstraction/OThread.cpp
	$(CXX) $(CFLAGS) OsAbstraction/OThread.cpp

clean:
	rm -rf *.o libicsneo.a OsAbstraction/*.o
