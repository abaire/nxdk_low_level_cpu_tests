include(XBEUtils REQUIRED)

find_package(NXDK REQUIRED)
find_package(NXDK_SDL2 REQUIRED)
find_package(Threads REQUIRED)

configure_file(configure.h.in configure.h)

# ---------------------------------------------------------------------------
# Compiler Options Macros
# ---------------------------------------------------------------------------

macro(set_opt_compile_and_link_options TARGET_NAME)
    target_compile_options(
            "${TARGET_NAME}"
            PRIVATE
            -O3
            -fno-strict-aliasing
            -Wall
            $<$<COMPILE_LANGUAGE:CXX>:-Wno-builtin-macro-redefined>
            -D_USE_MATH_DEFINES
    )
    target_link_options(
            "${TARGET_NAME}"
            PRIVATE
            "/debug:none"
    )
endmacro()

macro(set_compile_and_link_options TARGET_NAME)
    if (CMAKE_BUILD_TYPE MATCHES Debug)
        target_compile_options(
                "${TARGET_NAME}"
                PRIVATE
                -g
                -gdwarf-4
                -O0
                -Wall
                -fstandalone-debug
                $<$<COMPILE_LANGUAGE:CXX>:-Wno-builtin-macro-redefined>
                -D_USE_MATH_DEFINES
        )
        target_link_options(
                "${TARGET_NAME}"
                PRIVATE
                "/debug:full"
        )
    else ()
        set_opt_compile_and_link_options("${TARGET_NAME}")
    endif ()
endmacro()

# ---------------------------------------------------------------------------
# Test Artifact Generation Macro
# Usage: create_nxdk_test_artifact(target_name "XBE Title" source1.cpp source2.cpp ...)
# ---------------------------------------------------------------------------

macro(create_test_xiso TARGET_NAME XBE_TITLE)
    add_executable(
            ${TARGET_NAME}
            ${ARGN}
    )

    split_debug(${TARGET_NAME})

    set_compile_and_link_options(${TARGET_NAME})

    target_include_directories(
            ${TARGET_NAME}
            PRIVATE
            "${CMAKE_CURRENT_SOURCE_DIR}"
            "${CMAKE_CURRENT_BINARY_DIR}"
            "${CMAKE_SOURCE_DIR}/third_party"
    )

    target_link_libraries(
            ${TARGET_NAME}
            PRIVATE
            test_util
            NXDK::NXDK
            NXDK::NXDK_CXX
            NXDK::SDL2
            pbkitplusplus
            PRIVATE
            printf
    )

    set(XBE_TARGET "${TARGET_NAME}_xiso")

    add_xbe(
            ${XBE_TARGET} "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.exe"
            TITLE "${XBE_TITLE}"
            RESOURCE_ROOTS
            "${CMAKE_SOURCE_DIR}/resources"
            RESOURCE_DIRS
            "${CMAKE_SOURCE_DIR}/resources"
    )

    add_xiso(${TARGET_NAME}_xiso ${XBE_TARGET})

endmacro()


# ---------------------------------------------------------------------------
# Test Support Library
# ---------------------------------------------------------------------------

add_library(
        test_util
        STATIC
        test_util.cpp
        test_util.h
)
set_compile_and_link_options(test_util)
target_include_directories(
        test_util
        PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_SOURCE_DIR}/third_party"
)
target_link_libraries(
        test_util
        PUBLIC
        NXDK::NXDK
        NXDK::NXDK_CXX
        pbkitplusplus
        printf
)


# ---------------------------------------------------------------------------
# Target Definitions
# ---------------------------------------------------------------------------

# fpu_test
create_test_xiso(
        fpu_test
        "FPU test"
        fpu_main.cpp
        logger.cpp
        logger.h
        test_suite.h
        fpu_tests/fpu_test_common.cpp
        fpu_tests/fpu_test_common.h
        fpu_tests/test_fpu_basic_arithmetic.cpp
        fpu_tests/test_fpu_basic_arithmetic.h
        fpu_tests/test_fpu_x87_precision_control.cpp
        fpu_tests/test_fpu_x87_precision_control.h
)

# The x87 precision-control test uses Intel-syntax inline assembly.
set_source_files_properties(
        fpu_tests/test_fpu_x87_precision_control.cpp
        PROPERTIES
        COMPILE_OPTIONS
        -masm=intel
)
