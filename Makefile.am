COMMON_SUBDIRS = src graphics ctrl_dlg \
				lf_skin font_text look_feel dbuff \
                same housekeeper minesweeper commlcd \
                mgutils mgplus mgeff mgncs

if MGRM_PROCESSES
LITE_SUBDIRS=mginit
else
LITE_SUBDIRS=
endif

DIST_SUBDIRS = m4 $(COMMON_SUBDIRS) mginit
SUBDIRS = $(COMMON_SUBDIRS) $(LITE_SUBDIRS) m4

EXTRA_DIST = README.md LICENSE
