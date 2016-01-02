all: TD1 TD2 TD3

clean:
	$(MAKE) -C lib/ clean
	for dir in src/*; do $(MAKE) -C $$dir clean ; done

libs:
	$(MAKE) -C lib/

TD%: libs
	$(MAKE) -C src/TD$*

.PHONY: all clean libs
