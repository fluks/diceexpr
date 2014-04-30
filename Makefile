.PHONY: default clean debug check clean_check

default:
	$(MAKE) -C src/ $@

debug:
	$(MAKE) -C src/ $@

check:
	$(MAKE) -C src/ $@

clean:
	$(MAKE) -C src/ $@

clean_check:
	$(MAKE) -C src/ $@
