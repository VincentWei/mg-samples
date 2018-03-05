SUBDIRS=res

COMMON_LDADD=-lmgncs $(APP_LIBS)

noinst_PROGRAMS=helloworld helloworld_domodal helloworld_dlgbox \
    wnd_template event_listener data_source data_binding \
    button checkbutton radiogroup menubutton colorbutton \
	spinner spinbox progressbar resmgr edit panel container \
	propsheet scrollview listview iconview listbox trackbar \
	scrollbar timer combobox monthcalender

resmgr_SOURCES=resmgr.c resmgr_main.c resource.h
resmgr_LDADD=$(COMMON_LDADD)

monthcalender_SOURCES=monthcalender.c
monthcalender_LDADD=$(COMMON_LDADD)

helloworld_SOURCES=helloworld.c
helloworld_LDADD=$(COMMON_LDADD)

helloworld_domodal_SOURCES=helloworld_domodal.c
helloworld_domodal_LDADD=$(COMMON_LDADD)

helloworld_dlgbox_SOURCES=helloworld_dlgbox.c
helloworld_dlgbox_LDADD=$(COMMON_LDADD)

wnd_template_SOURCES=wnd_template.c
wnd_template_LDADD=$(COMMON_LDADD)

event_listener_SOURCES=event_listener.c
event_listener_LDADD=$(COMMON_LDADD)

data_source_SOURCES=data_source.c
data_source_LDADD=$(COMMON_LDADD)

data_binding_SOURCES=data_binding.c
data_binding_LDADD=$(COMMON_LDADD)

checkbutton_SOURCES=checkbutton.c
checkbutton_LDADD=$(COMMON_LDADD)


radiogroup_SOURCES=radiogroup.c
radiogroup_LDADD=$(COMMON_LDADD)

button_SOURCES=button.c
button_LDADD=$(COMMON_LDADD)

menubutton_SOURCES=menubutton.c
menubutton_LDADD=$(COMMON_LDADD)

colorbutton_SOURCES=colorbutton.c
colorbutton_LDADD=$(COMMON_LDADD)

spinner_SOURCES=spinner.c
spinner_LDADD=$(COMMON_LDADD)

spinbox_SOURCES=spinbox.c
spinbox_LDADD=$(COMMON_LDADD)

progressbar_SOURCES=progressbar.c
progressbar_LDADD=$(COMMON_LDADD)

panel_SOURCES=panel.c
panel_LDADD=$(COMMON_LDADD)

edit_SOURCES=edit.c
edit_LDADD=$(COMMON_LDADD)

container_SOURCES=container.c
container_LDADD=$(COMMON_LDADD)

propsheet_SOURCES=propsheet.c
propsheet_LDADD=$(COMMON_LDADD)

scrollview_SOURCES=scrollview.c
scrollview_LDADD=$(COMMON_LDADD)

listview_SOURCES=listview.c
listview_LDADD=$(COMMON_LDADD)

iconview_SOURCES=iconview.c
iconview_LDADD=$(COMMON_LDADD)

listbox_SOURCES=listbox.c
listbox_LDADD=$(COMMON_LDADD)

trackbar_SOURCES=trackbar.c
trackbar_LDADD=$(COMMON_LDADD)

scrollbar_SOURCES=scrollbar.c
scrollbar_LDADD=$(COMMON_LDADD)

timer_SOURCES=timer.c
timer_LDADD=$(COMMON_LDADD)

combobox_SOURCES=combobox.c
combobox_LDADD=$(COMMON_LDADD)

EXTRA_DIST=resmgr_demo.res ncs-window-types.h ncs-windows.h

