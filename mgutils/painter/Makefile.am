noinst_PROGRAMS=painter

CUSTOMMG_SRC=custom_minigui.c
painter_SOURCES=painter.c painter.h painter_res_en.h \
				painter_res_cn.h painter_res_tw.h $(CUSTOMMG_SRC)

painter_LDADD=../../src/libmgutils.la @APP_LIBS@
