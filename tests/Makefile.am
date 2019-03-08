LDFLAGS=-lefence
EXTRA_DIST = README.md fetch-ucd-test.sh res

noinst_PROGRAMS = \
    getucharsandbreaks \
    drawglyphstringex \
    createlogfontex

COMMFILES = helpers.c helpers.h
getucharsandbreaks_SOURCES = getucharsandbreaks.c $(COMMFILES)
drawglyphstringex_SOURCES = drawglyphstringex.c $(COMMFILES)
createlogfontex_SOURCES = createlogfontex.c $(COMMFILES)


