
include_directories("${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/samples/src")

link_directories("${PROJECT_BINARY_DIR}/src")

set (TEST_APP_LIBS "-lmgncs${mGNCS_LIBSUFFIX} -lmgutils${mGNCS_LIBSUFFIX} ${MGNCS_APP_LIBRARIES}")

message(STATUS "The Test Applications link ${TEST_APP_LIBS}")

list(APPEND sample_targets
    button
    spinbox
    data_binding
    menubutton
    radiogroup
    helloworld_dlgbox
    propsheet
    listview
    wnd_template
    scrollview
    checkbutton
    data_source
    helloworld_domodal
    iconview
    listbox
    edit
    colorbutton
    monthcalender
    spinner
    trackbar
    scrollbar
    progressbar
    panel
    combobox
    container
    event_listener
    timer
    helloworld
    list
    )

foreach(_sample_target  ${sample_targets})
    add_executable(${_sample_target}2 "${_sample_target}.c")
    target_link_libraries(${_sample_target}2 "${TEST_APP_LIBS}")
endforeach(_sample_target)


