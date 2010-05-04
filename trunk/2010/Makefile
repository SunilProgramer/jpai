all:
	$(MAKE) -C ai/
	$(MAKE) -C maps/generators/
	$(MAKE) -C problem/
	cp ai/random/bin/random problem/bin/ai/
	cp ai/simple/bin/simple problem/bin/ai/
	cp ai/greedy/bin/greedy problem/bin/ai/

distclean:
	$(MAKE) -C ai/ distclean
	$(MAKE) -C maps/generators/ distclean
	$(MAKE) -C problem/ distclean
