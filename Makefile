CC :=x86_64-w64-mingw32
LSF :=$(wildcard lsanotif/*.c)
WSF :=$(wildcard winlogon/*.c)
SH :=$(wildcard shared/*.c)
OPTIONS :=-shared -lws2_32 -s
INCLUDE :=-Ishared
OUTDIR :=bin
RSC :=$(OUTDIR)/resource.o

all: clean lsa winlogon 

lsa-resource:
	$(CC)-windres lsanotif/resource.rc -o $(RSC) $(INCLUDE)

winlogon-resource:
	$(CC)-windres winlogon/resource.rc -o $(RSC) $(INCLUDE)

lsa: lsa-resource
	$(CC)-gcc $(LSF) $(SH) $(RSC) -o $(OUTDIR)/lsanotification.dll $(OPTIONS) $(INCLUDE)
	rm $(RSC)

winlogon: winlogon-resource
	$(CC)-gcc $(WSF) $(SH) $(RSC) -o $(OUTDIR)/winlogon.dll $(OPTIONS) $(INCLUDE)
	rm $(RSC)

clean:
	mkdir -p $(OUTDIR)
	rm -f $(OUTDIR)/*
