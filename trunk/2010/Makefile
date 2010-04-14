all:
	$(MAKE) -C ai/
	$(MAKE) -C maps/generators/random/
	$(MAKE) -C problem/
	cp ai/random/bin/random problem/bin/ai/
	cp ai/simple/bin/simple problem/bin/ai/
	cp ai/simple/bin/greedy problem/bin/ai/

distclean:
	$(MAKE) -C ai/ distclean
	$(MAKE) -C maps/generators/random/ distclean
	$(MAKE) -C problem/ distclean
