AUTOMAKE_OPTIONS = foreign
SUBDIRS = theme data src desktops po

install-exec-hook:
	cd $(srcdir)/help && \
	for each in $$(find . -name '*.help'); do \
		${INSTALL} -d -m 755 ${DESTDIR}/usr/share/help/epuzzles/$$(dirname $${each}); \
		${INSTALL_DATA} $${each} ${DESTDIR}/usr/share/help/epuzzles/$${each}; \
	done 
	cd ${DESTDIR}/usr/share/help/epuzzles && ln -sf en C
