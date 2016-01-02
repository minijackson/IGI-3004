all: libgestion-fichiers.so libprocutils.so libpipe.so

clean:
	$(RM) *.so

lib%.so: ../src/%/lib%.so
	cp ../src/$*/lib$*.so .

../src/%/lib%.so:
	$(MAKE) -C ../src/$*/ lib$*.so

.PHONY: all
