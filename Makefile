CC := x86_64-w64-mingw32
LSF := $(wildcard lsanotif/*.cpp)
WSF := $(wildcard winlogon/*.c)
OPTIONS := -static -shared -lws2_32

all: clean lsa-resource winlogon-resource lsa winlogon

lsa-resource:
	$(CC)-windres lsanotif/resource.rc -o lsanotif/resource.o

winlogon-resource:
	$(CC)-windres winlogon/resource.rc -o winlogon/resource.o

lsa:
	$(CC)-g++ lsanotif/$(LSF) lsanotif/resource.o -o bin/lsanotifcation.dll $(OPTIONS)

winlogon:
	$(CC)-gcc winlogon/$(WSF) winlogon/resource.o -o bin/winlogon.dll $(OPTIONS)

clean:
	mkdir -p bin
	rm -f bin/*