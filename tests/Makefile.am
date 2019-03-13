LDFLAGS=-lefence
EXTRA_DIST = README.md fetch-ucd-test.sh res

noinst_PROGRAMS = \
    ustrgetbreaks \
    drawglyphstringex \
    createlogfontex \
    getshapedglyphsbasic \
    biditest \
    bidicharactertest

COMMFILES = helpers.c helpers.h
ustrgetbreaks_SOURCES = ustrgetbreaks.c $(COMMFILES)
drawglyphstringex_SOURCES = drawglyphstringex.c $(COMMFILES)
createlogfontex_SOURCES = createlogfontex.c $(COMMFILES)
getshapedglyphsbasic_SOURCES = getshapedglyphsbasic.c $(COMMFILES)
biditest_SOURCES = biditest.c $(COMMFILES)
bidicharactertest_SOURCES = bidicharactertest.c $(COMMFILES)

