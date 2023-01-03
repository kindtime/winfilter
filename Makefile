CC :=x86_64-w64-mingw32
LSF :=$(wildcard lsanotif/*.cpp)
WSF :=$(wildcard winlogon/*.c)
SH :=$(wildcard shared/*.c)
OPTIONS :=-static -shared -lws2_32
INCLUDE :=-Ishared
OUTDIR :=bin
RSC :=$(OUTDIR)/resource.o

all: clean lsa winlogon 

lsa-resource:
	$(CC)-windres lsanotif/resource.rc -o $(RSC) $(INCLUDE)

winlogon-resource:
	$(CC)-windres winlogon/resource.rc -o $(RSC) $(INCLUDE)

lsa: lsa-resource
	$(CC)-g++ $(LSF) $(SH) $(RSC) -o $(OUTDIR)/lsanotification.dll $(OPTIONS) $(INCLUDE)
	rm $(RSC)

winlogon: winlogon-resource
	$(CC)-gcc $(WSF) $(SH) $(RSC) -o $(OUTDIR)/winlogon.dll $(OPTIONS) $(INCLUDE)
	rm $(RSC)

clean:
	mkdir -p $(OUTDIR)
	rm -f $(OUTDIR)/*
