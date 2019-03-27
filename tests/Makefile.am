LDFLAGS=-lefence
EXTRA_DIST = README.md fetch-ucd-test.sh res

noinst_PROGRAMS = \
    ustrgetbreaks \
    drawglyphstringex \
    createlogfontex \
    biditest \
    bidicharactertest \
    createtextrunsinfo \
    createlayoutinfo

COMMFILES = helpers.c helpers.h
ustrgetbreaks_SOURCES = ustrgetbreaks.c $(COMMFILES)
drawglyphstringex_SOURCES = drawglyphstringex.c $(COMMFILES)
createlogfontex_SOURCES = createlogfontex.c $(COMMFILES)
biditest_SOURCES = biditest.c $(COMMFILES)
bidicharactertest_SOURCES = bidicharactertest.c $(COMMFILES)
createtextrunsinfo_SOURCES = createtextrunsinfo.c $(COMMFILES)
createlayoutinfo_SOURCES = createlayoutinfo.c $(COMMFILES)

