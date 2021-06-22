ROOT=root
BINDIR=/usr/bin
CFLAGS=$(RPM_OPT_FLAGS) $(shell pkg-config --cflags gtk+-2.0) $(shell pkg-config --cflags glib-2.0) -I/usr/include/NVCtrl
LDFLAGS=$(shell pkg-config --libs gtk+-2.0) $(shell pkg-config --libs glib-2.0) -lXNVCtrl -lX11 -lXext

all: nvdock

nvdock: nvdock.o icon.o util.o xnvctrl.o
	$(CC) $(CFLAGS) nvdock.o icon.o util.o xnvctrl.o -o nvdock $(LDFLAGS)

nvdock.o: src/nvdock.c src/nvdock.h
	$(CC) $(CFLAGS) -Isrc -c src/nvdock.c

icon.o: src/icon.h src/icon.c
	$(CC) $(CFLAGS) -Isrc -c src/icon.c

util.o: src/util.c
	$(CC) $(CFLAGS) -Isrc -c src/util.c

xnvctrl.o: src/xnvctrl.c
	$(CC) $(CFLAGS) -Isrc -c src/xnvctrl.c

install:
	mkdir -p $(DESTDIR)$(BINDIR)
	$(INSTALL) -o $(ROOT) -g $(ROOT) -m 755 nvdock \
		$(DESTDIR)$(BINDIR)/nvdock

	mkdir -p $(DESTDIR)/usr/share/pixmaps
	$(INSTALL) -o $(ROOT) -g $(ROOT) -m 644 data/nvdock.png \
		$(DESTDIR)/usr/share/pixmaps

	mkdir -p $(DESTDIR)/usr/share/applications
	$(INSTALL) -o $(ROOT) -g $(ROOT) -m 644 data/nvdock.desktop \
		$(DESTDIR)/usr/share/applications

clean:
	rm -f nvdock *.o
