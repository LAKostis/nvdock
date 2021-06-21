CFLAGS=$(shell pkg-config --cflags gtk+-2.0 glib) $(shell pkg-config --cflags glib-2.0)
LDFLAGS=$(shell pkg-config --libs gtk+-2.0) $(shell pkg-config --libs glib-2.0)

all:
	@make clean
	mkdir build
	
	gcc src/nvdock.c -c -o build/nvdock.o ${CFLAGS}
	gcc src/icon.c -c -o build/icon.o ${CFLAGS}
	gcc src/util.c -c -o build/util.o ${CFLAGS}
	
	gcc build/nvdock.o build/icon.o build/util.o -o build/nvdock ${LDFLAGS}
	
	strip --strip-unneeded build/nvdock

install:
	cp build/nvdock /usr/bin
	chmod 755 /usr/bin/nvdock
	
	cp data/nvdock.png /usr/share/pixmaps
	chmod 644 /usr/share/pixmaps/nvdock.png
	
	cp data/nvdock.desktop /usr/share/applications
	chmod 644 /usr/share/pixmaps/nvdock.png

clean:
	@rm -rf build
	
uninstall:
	rm -f /usr/bin/nvdock
	rm -f /usr/share/pixmaps/nvdock.png
	rm -f /usr/share/applications/nvdock.png

