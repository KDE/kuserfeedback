set(uf_cli_srcs
    main.cpp
)

add_executable(userfeedbackctl ${uf_cli_srcs})
target_compile_features(userfeedbackctl PRIVATE cxx_generic_lambdas)
target_link_libraries(userfeedbackctl KUserFeedbackConsole)
install(TARGETS userfeedbackctl ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
