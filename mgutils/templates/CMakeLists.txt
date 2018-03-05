list (APPEND sample_targets
    colorseldlg
    fileopendlg
    fontseldlg
    infoshowdlg
)

foreach(_sample_target  ${sample_targets})
    add_executable(${_sample_target} "${_sample_target}.c")
    target_link_libraries(${_sample_target} "${APP_LIBS}")
endforeach(_sample_target)

