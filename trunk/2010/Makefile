all:
	$(MAKE) -C ai/random/
	$(MAKE) -C maps/generators/random/
	$(MAKE) -C problem/
	cp ai/random/bin/random problem/bin/ai/

distclean:
	$(MAKE) -C ai/random distclean
	$(MAKE) -C maps/generators/random/ distclean
	$(MAKE) -C problem/ distclean
