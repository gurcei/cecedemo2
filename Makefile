CC65=	/usr/local/bin/cc65
CL65=	/usr/local/bin/cl65
COPTS=	-t c64 -O -Or -Oi -Os --cpu 6502
LOPTS=	-C gidemo.cfg

all: gidemo.d64

run:
	/Applications/Vice/x64.app/Contents/MacOS/x64 gidemo.d64 &

main.s:	main.c gidemo.cfg
	$(CC65) $(COPTS) --add-source -o $@ $<

gidemo.prg: main.s sprites.s
	$(CL65) $(COPTS) $(LOPTS) -vm -l gidemo.list -m gidemo.map -o gidemo.prg main.s sprites.s

gidemo.d64: gidemo.prg
	rm -f gidemo.d64
	cbmconvert -v2 -D4o gidemo.d64 gidemo.prg tini.seq

clean:
	rm -f gidemo.map
	rm -f gidemo.prg
	rm -f main.s main.o
	rm -f gidemo.list
	rm -f gidemo.d64
