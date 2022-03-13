package = tocken
version = 1.0
tarname = $(package)
distdir = $(tarname)-$(version)

all clean tocken:
	cd src && $(MAKE) $@

dist: $(distdir).tar.gz

$(distdir).tar.gz: $(distdir)
	tar chof - $(distdir) | gzip -9 -c > $@
	rm -rf $(distdir)

$(distdir): FORCE
	mkdir -p $(distdir)/src
	cp Makefile $(distdir)
	cp src/Makefile src/*.cpp src/*.hpp $(distdir)/src
	mkdir -p $(distdir)/source
	cp source/* $(distdir)/source

FORCE:
	-rm $(distdir).tar.gz >/dev/null 2>&1
	rm -rf $(distdir)

.PHONY: all clean dist FORCE