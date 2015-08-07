CC=g++
CFLAGS=-g -c -I. -I./OsAbstraction
AR=ar

all: lib sample

lib: CFTDILib.o cicsneoVI.o icsneoLinuxAPI.o OsAbstraction/OCriticalSection.o OsAbstraction/OEvent.o OsAbstraction/OSAbstraction.o OsAbstraction/OThread.o
	$(AR) rcs libicsneo.a CFTDILib.o cicsneoVI.o icsneoLinuxAPI.o OCriticalSection.o OEvent.o OSAbstraction.o OThread.o

sample: GeneralTestProject/main.o
	$(CC) main.o -o icsneo_sample -l:libicsneo.a -lpthread -lftdi

CFTDILib.o: CFTDILib.cpp
	$(CC) $(CFLAGS) CFTDILib.cpp

cicsneoVI.o: cicsneoVI.cpp
	$(CC) $(CFLAGS) cicsneoVI.cpp

icsneoLinuxAPI.o: icsneoLinuxAPI.cpp
	$(CC) $(CFLAGS) icsneoLinuxAPI.cpp

OsAbstraction/OCriticalSection.o: OsAbstraction/OCriticalSection.cpp
	$(CC) $(CFLAGS) OsAbstraction/OCriticalSection.cpp

OsAbstraction/OEvent.o: OsAbstraction/OEvent.cpp
	$(CC) $(CFLAGS) OsAbstraction/OEvent.cpp

OsAbstraction/OSAbstraction.o: OsAbstraction/OSAbstraction.cpp
	$(CC) $(CFLAGS) OsAbstraction/OSAbstraction.cpp

OsAbstraction/OThread.o: OsAbstraction/OThread.cpp
	$(CC) $(CFLAGS) OsAbstraction/OThread.cpp

GeneralTestProject/main.o: GeneralTestProject/main.cpp
	$(CC) $(CFLAGS) GeneralTestProject/main.cpp

clean:
	rm -rf *.o libicsneo.a OsAbstraction/*.o