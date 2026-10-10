# Full featured printf.
add_library(
        printf
        EXCLUDE_FROM_ALL
        printf/printf.c
        printf/printf.h
)
target_compile_options(
        printf
        PRIVATE
        -O3
        -Wno-everything
)
target_link_options(printf PRIVATE "/debug:none")

# Berkeley SoftFloat 3
include("${CMAKE_CURRENT_SOURCE_DIR}/softfloat_sources.cmake")
add_library(
        softfloat
        EXCLUDE_FROM_ALL
        ${SOFTFLOAT_SOURCES}
)
target_include_directories(
        softfloat
        PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/berkeley-softfloat-3/source/include"
        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/berkeley-softfloat-3/source/8086"
        "${CMAKE_CURRENT_SOURCE_DIR}/berkeley-softfloat-3/build/Linux-386-GCC"
)
target_compile_options(
        softfloat
        PRIVATE
        -O3
        -fno-strict-aliasing
        -Wno-everything
        -DSOFTFLOAT_ROUND_ODD
        -DINLINE_LEVEL=5
        -DSOFTFLOAT_FAST_DIV32TO16
        -DSOFTFLOAT_FAST_DIV64TO32
)
target_link_options(softfloat PRIVATE "/debug:none")

# Berkeley TestFloat 3
include("${CMAKE_CURRENT_SOURCE_DIR}/testfloat_sources.cmake")
add_library(
        testfloat
        EXCLUDE_FROM_ALL
        ${TESTFLOAT_SOURCES}
)
target_include_directories(
        testfloat
        PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/berkeley-testfloat-3/source"
        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/berkeley-softfloat-3/source/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/berkeley-testfloat-3/build/Linux-386-GCC"
)
target_compile_options(
        testfloat
        PUBLIC
        -DFLOAT16
        -DFLOAT64
        -DEXTFLOAT80
        -DFLOAT128
        -DFLOAT_ROUND_ODD
        -DLONG_DOUBLE_IS_EXTFLOAT80
        PRIVATE
        -O3
        -fno-strict-aliasing
        -Wno-everything
)
target_link_libraries(testfloat PUBLIC softfloat)
target_link_options(testfloat PRIVATE "/debug:none")
