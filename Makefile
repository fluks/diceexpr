.PHONY: default clean debug

default:
	$(MAKE) -C src/ $@

debug:
	$(MAKE) -C src/ $@

clean:
	$(MAKE) -C src/ $@
