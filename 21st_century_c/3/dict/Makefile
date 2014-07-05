dict:
	mkdir -p dict_demo
	cp dict.automake dict_demo/Makefile.am
	for x in dict.c dict.h keyval.c keyval.h dict_use.c; do \
		ln -sf ../../../11/$$x dict_demo/; \
	done
	ln -sf ../../../2/dict_test.c dict_demo/
	cd dict_demo && autoscan
	sed -e 's/FULL-PACKAGE-NAME/dict/' \
		-e 's/VERSION/1/'               \
		-e 's|BUG-REPORT-ADDRESS|/dev/null|' \
		-e '/AC_OUTPUT/i AC_PROG_CC_C99' \
		-e '/AC_OUTPUT/i AM_INIT_AUTOMAKE' \
		-e '/AC_OUTPUT/i LT_INIT' \
		< dict_demo/configure.scan > dict_demo/configure.ac
	touch dict_demo/{NEWS,README,AUTHORS,ChangeLog}
	cd dict_demo && autoreconf -iv
	cd dict_demo && ./configure
	cd dict_demo && make

test:
	make -C dict_demo check

clean:
	rm -rf dict_demo
