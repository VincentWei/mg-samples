LDFLAGS=-lefence
EXTRA_DIST = res

noinst_PROGRAMS = \
    getucharsandbreaks \
    getglyphsextentfromuchars \
    createlogfontex

COMMFILES = helpers.c helpers.h
getucharsandbreaks_SOURCES = getucharsandbreaks.c $(COMMFILES)
getglyphsextentfromuchars_SOURCES = getglyphsextentfromuchars.c $(COMMFILES)
createlogfontex_SOURCES = createlogfontex.c $(COMMFILES)


