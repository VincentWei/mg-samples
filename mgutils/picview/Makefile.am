noinst_PROGRAMS=picview

SUBDIRS = res

CUSTOMMG_SRC=custom_minigui.c
picview_SOURCES=picview.c picview_res_en.h \
				pivview_res_cn.h picview_res_tw.h $(CUSTOMMG_SRC)

picview_LDADD=../../src/libmgutils.la @APP_LIBS@
