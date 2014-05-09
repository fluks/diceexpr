.PHONY: default clean debug check clean_check example

default:
	$(MAKE) -C src/ $@

debug:
	$(MAKE) -C src/ $@

check:
	$(MAKE) -C src/ $@

example:
	$(MAKE) -C src/ $@

clean:
	$(MAKE) -C src/ $@

clean_check:
	$(MAKE) -C src/ $@
