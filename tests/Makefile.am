LDFLAGS=-lefence
EXTRA_DIST = README.md fetch-ucd-test.sh res

noinst_PROGRAMS = \
    getucharsandbreaks \
    drawglyphstringex \
    createlogfontex \
    bidilogachars2visachars \
    ubidireorderline

COMMFILES = helpers.c helpers.h
getucharsandbreaks_SOURCES = getucharsandbreaks.c $(COMMFILES)
drawglyphstringex_SOURCES = drawglyphstringex.c $(COMMFILES)
createlogfontex_SOURCES = createlogfontex.c $(COMMFILES)
bidilogachars2visachars_SOURCES = bidilogachars2visachars.c $(COMMFILES)
ubidireorderline_SOURCES = ubidireorderline.c $(COMMFILES)

