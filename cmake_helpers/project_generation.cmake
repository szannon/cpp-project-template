# Requires CMake 3.19+ for cmake_language(DEFER).

function(_pg_collect_targets _directory _output)
    get_property(_excluded
        DIRECTORY "${_directory}"
        PROPERTY EXCLUDE_FROM_ALL
    )

    if(_excluded)
        set("${_output}" "" PARENT_SCOPE)
        return()
    endif()

    get_property(_targets
        DIRECTORY "${_directory}"
        PROPERTY BUILDSYSTEM_TARGETS
    )

    set(_included)

    foreach(_target IN LISTS _targets)
        get_property(_excluded
            TARGET "${_target}"
            PROPERTY EXCLUDE_FROM_ALL
        )

        if(NOT _excluded)
            list(APPEND _included "${_target}")
        endif()
    endforeach()

    get_property(_subdirs
        DIRECTORY "${_directory}"
        PROPERTY SUBDIRECTORIES
    )

    foreach(_subdir IN LISTS _subdirs)
        _pg_collect_targets("${_subdir}" _nested)
        list(APPEND _included ${_nested})
    endforeach()

    list(REMOVE_DUPLICATES _included)
    set("${_output}" "${_included}" PARENT_SCOPE)
endfunction()


function(_pg_finalize)
    get_property(_name GLOBAL PROPERTY _PG_TARGET_NAME)
    if(NOT _name OR NOT TARGET "${_name}")
        message(FATAL_ERROR
            "Project generation target was not found: '${_name}'")
    endif()

    _pg_collect_targets("${CMAKE_SOURCE_DIR}" _targets)
    list(REMOVE_ITEM _targets "${_name}")

    if(_targets)
        add_dependencies("${_name}" ${_targets})
    endif()
endfunction()


function(project_generation)
    cmake_parse_arguments(ARG
        ""
        "NAME"
        "COPY"
        ${ARGN}
    )

    if(NOT ARG_NAME)
        message(FATAL_ERROR "project_generation requires NAME")
    endif()

    get_property(_registered
        GLOBAL PROPERTY _PG_REGISTERED
    )

    if(_registered)
        message(FATAL_ERROR
            "project_generation may only be called once")
    endif()

    if(TARGET "${ARG_NAME}")
        message(FATAL_ERROR
            "Target '${ARG_NAME}' already exists")
    endif()

    list(LENGTH ARG_COPY _count)
    math(EXPR _remainder "${_count} % 2")

    if(_remainder)
        message(FATAL_ERROR
            "COPY requires source/destination pairs")
    endif()

    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles")

    set(_script
        "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${ARG_NAME}-copy.cmake"
    )

    file(WRITE "${_script}" [=[
if(NOT EXISTS "${PG_SOURCE}")
    message(FATAL_ERROR "Copy source does not exist: ${PG_SOURCE}")
endif()

file(MAKE_DIRECTORY "${PG_DESTINATION}")
file(COPY "${PG_SOURCE}" DESTINATION "${PG_DESTINATION}")
]=])

    set(_commands)

    if(_count GREATER 0)
        math(EXPR _last "${_count} - 1")

        foreach(_i RANGE 0 ${_last} 2)
            math(EXPR _j "${_i} + 1")
            list(GET ARG_COPY ${_i} _source)
            list(GET ARG_COPY ${_j} _destination)

            list(APPEND _commands
                COMMAND "${CMAKE_COMMAND}"
                    "-DPG_SOURCE=${_source}"
                    "-DPG_DESTINATION=${_destination}"
                    -P "${_script}"
            )
        endforeach()
    endif()

    add_custom_target("${ARG_NAME}" ALL
        ${_commands}
        VERBATIM
    )

    set_property(GLOBAL PROPERTY _PG_REGISTERED TRUE)
    set_property(GLOBAL PROPERTY _PG_TARGET_NAME "${ARG_NAME}")
    cmake_language(DEFER
        DIRECTORY "${CMAKE_SOURCE_DIR}"
        CALL _pg_finalize
    )
endfunction()
