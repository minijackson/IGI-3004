all: libgestion-fichiers.so libprocutils.so libpipe.so

debug: TARGET=debug
debug: all

release: TARGET=release
release: all

clean:
	$(RM) *.so

lib%.so: ../src/%/lib%.so
	cp ../src/$*/lib$*.so .

../src/%/lib%.so:
	$(MAKE) -C ../src/$*/ $(TARGET)

.PHONY: all debug release clean
