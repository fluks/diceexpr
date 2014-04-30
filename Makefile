.PHONY: default clean debug

default:
	$(MAKE) -C src/ $@

debug:
	$(MAKE) -C src/ $@

check:
	$(MAKE) -C src/ $@

clean:
	$(MAKE) -C src/ $@

clean_test:
	$(MAKE) -C src/ $@
