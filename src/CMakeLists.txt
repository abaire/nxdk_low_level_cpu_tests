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

# sse_test
create_test_xiso(
        sse_test
        "SSE test"
        sse_main.cpp
        logger.cpp
        logger.h
        test_suite.h
        sse_tests/sse_runner_case.h
        sse_tests/sse_test_common.cpp
        sse_tests/sse_test_common.h
        sse_tests/test_sse_min_max.cpp
        sse_tests/test_sse_min_max.h
        sse_tests/test_sse_comparisons.cpp
        sse_tests/test_sse_comparisons.h
        sse_tests/test_sse_approximations.cpp
        sse_tests/test_sse_approximations.h
        sse_tests/test_sse_mxcsr_ftz.cpp
        sse_tests/test_sse_mxcsr_ftz.h
)

# SSE test modules use Intel-syntax inline assembly and SSE instructions.
set_source_files_properties(
        sse_tests/test_sse_min_max.cpp
        sse_tests/test_sse_comparisons.cpp
        sse_tests/test_sse_approximations.cpp
        sse_tests/test_sse_mxcsr_ftz.cpp
        PROPERTIES
        COMPILE_OPTIONS
        "-masm=intel;-msse"
)

# mmx_test
create_test_xiso(
        mmx_test
        "MMX test"
        mmx_main.cpp
        logger.cpp
        logger.h
        test_suite.h
        mmx_tests/mmx_test_common.cpp
        mmx_tests/mmx_test_common.h
        mmx_tests/test_mmx_arithmetic.cpp
        mmx_tests/test_mmx_arithmetic.h
        mmx_tests/test_mmx_logical_compare_shift.cpp
        mmx_tests/test_mmx_logical_compare_shift.h
        mmx_tests/test_mmx_pack_unpack.cpp
        mmx_tests/test_mmx_pack_unpack.h
        mmx_tests/test_mmx_extensions.cpp
        mmx_tests/test_mmx_extensions.h
        mmx_tests/test_mmx_x87_aliasing.cpp
        mmx_tests/test_mmx_x87_aliasing.h
)

# MMX test modules use Intel-syntax inline assembly and MMX/SSE instructions.
set_source_files_properties(
        mmx_tests/test_mmx_arithmetic.cpp
        mmx_tests/test_mmx_logical_compare_shift.cpp
        mmx_tests/test_mmx_pack_unpack.cpp
        mmx_tests/test_mmx_extensions.cpp
        mmx_tests/test_mmx_x87_aliasing.cpp
        PROPERTIES
        COMPILE_OPTIONS
        "-masm=intel;-mmmx;-msse"
)


# testfloat_test
create_test_xiso(
        testfloat_test
        "TestFloat test"
        testfloat_main.cpp
        logger.cpp
        logger.h
        test_suite.h
        testfloat_tests/testfloat_test_common.cpp
        testfloat_tests/testfloat_test_common.h
        testfloat_tests/subjfloat_config.h
        testfloat_tests/subjfloat_x87.cpp
        testfloat_tests/test_testfloat_f32.cpp
        testfloat_tests/test_testfloat_f32.h
        testfloat_tests/test_testfloat_f64.cpp
        testfloat_tests/test_testfloat_f64.h
        testfloat_tests/test_testfloat_extf80.cpp
        testfloat_tests/test_testfloat_extf80.h
)

target_link_libraries(
        testfloat_test
        PRIVATE
        testfloat
        softfloat
)

target_include_directories(
        testfloat_test
        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/testfloat_tests"
        "${CMAKE_SOURCE_DIR}/third_party/berkeley-softfloat-3/source/include"
        "${CMAKE_SOURCE_DIR}/third_party/berkeley-testfloat-3/source"
        "${CMAKE_SOURCE_DIR}/third_party/berkeley-testfloat-3/build/Linux-386-GCC"
)

# TestFloat x87 module uses Intel-syntax inline assembly.
set_source_files_properties(
        testfloat_tests/subjfloat_x87.cpp
        PROPERTIES
        COMPILE_OPTIONS
        -masm=intel
)


