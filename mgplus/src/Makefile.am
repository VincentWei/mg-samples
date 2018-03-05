AM_CPPFLAGS = -I../animate -I../animate/common_animates

LDADD = ../animate/libanimate.la -lmgplus

COMMON_PROGS = mgplustest gradientdemo mgglyph_outline coverflow \
	       mgcompositedemo blinds centersplit cubicrotate lion_test mgplus_efficiency

noinst_PROGRAMS = $(COMMON_PROGS)

mgplus_efficiency_SOURCES = mgplus_efficiency.c
lion_test_SOURCES = lion_test.c
gradientdemo_SOURCES = gradientdemo.c
coverflow_SOURCES = coverflow.c
mgplustest_SOURCES = mgplustest.c
mgglyph_outline_SOURCES = mgglyph_outline.c
mgcompositedemo_SOURCES = mgcompositedemo.c
blinds_SOURCES = blinds.c
cubicrotate_SOURCES = cubicrotate.c
centersplit_SOURCES = centersplit.c

SUBDIRS = res
