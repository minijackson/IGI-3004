all: TD1 TD2 TD3 TD4

clean:
	$(MAKE) -C lib/ clean
	for dir in src/*; do $(MAKE) -C $$dir clean ; done

libs:
	$(MAKE) -C lib/ $(TARGET)

TD%: libs
	$(MAKE) -C src/TD$* $(TARGET)

.PHONY: all clean libs
