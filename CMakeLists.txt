cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

project(xevent CXX C)

option(XENABLE_CODE_COVERAGE "Enable code coverage" OFF)
option(XENABLE_TESTS "Enable building tests" OFF)
option(XENABLE_VERBOSE_DBG "Enable verbose dbg" OFF)
option(XENABLE_EXCEPTION_AS_ERROR "Treat exception as xerror in debug mode" OFF)
option(BUILD_METRICS "build metrics" ON)
option(XDISABLE_RATELIMIT "Disable rate limit" OFF)
option(XENABLE_CONFIG_CHECK, "Enable config check when node boot" OFF)
option(XENABLE_MOCK_ZEC_STAKE, "Enable mocking stake for ZEC" OFF)

set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (NOT CMAKE_CXX_COMPILER)
    message(FATAL_ERROR "C++ Compiler not found")
endif()

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

string(TOLOWER ${CMAKE_BUILD_TYPE} XBUILD_TYPE)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    if (NOT CMAKE_TOOLCHAIN_FILE)
        message(FATAL_ERROR "CMAKE_TOOLCHAIN_FILE property not defined. Set it in CMakeSettings.json file.")
    endif()

    if (NOT EXISTS ${CMAKE_TOOLCHAIN_FILE})
        message(FATAL_ERROR "CMAKE_TOOLCHAIN_FILE not found.")
    endif()
endif()

#=============================================================================
# for git version
#==============================================================================
add_definitions(
    -DBUILD_USER_LIB # for -lcommon
    -D__TRACKING_PBFT_PROCESS__
    -DTEST_FOR_ELECTION
    -DGOSSIP_WITH_WROUTER
)

#==============================================================================
# In order to set default executable and library binaries' location,
# one solution, by setting
# variables (CMAKE_RUNTIME_OUTPUT_DIRECTORY / CMAKE_LIBRARY_OUTPUT_DIRECTORY)
# or properties (RUNTIME_OUTPUT_DIRECTORY / LIBRARY_OUTPUT_DIRECTORY),
# doesn't work, althrough CMake documentation says it's the recommonded way.
# Fall back to use EXECUTABLE_OUTPUT_PATH / LIBRARY_OUTPUT_PATH.
#==============================================================================
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
#set_property(DIRECTORY PROPERTY RUNTIME_OUTPUT_DIRECTORY ${XTOPCHAIN_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE})
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR/lib})
#set_property(DIRECTORY PROPERTY LIBRARY_OUTPUT_DIRECTORY ${XTOPCHAIN_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})

set(EXECUTABLE_OUTPUT_PATH ${XTOPCHAIN_BINARY_DIR}/bin/${CMAKE_SYSTEM_NAME}/)
set(LIBRARY_OUTPUT_PATH ${XTOPCHAIN_BINARY_DIR}/lib/${CMAKE_SYSTEM_NAME}/)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang OR ${CMAKE_CXX_COMPILER_ID} STREQUAL AppleClang OR ${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    # set common options for Clang or GNUCXX

    add_compile_options(
        $<$<STREQUAL:$<TARGET_PROPERTY:LINKER_LANGUAGE>,CXX>:-fexceptions>  # https://gcc.gnu.org/onlinedocs/gcc-7.3.0/gcc/Code-Gen-Options.html#index-fexceptions
        $<$<STREQUAL:$<TARGET_PROPERTY:LINKER_LANGUAGE>,CXX>:-frtti>        # https://gcc.gnu.org/onlinedocs/gcc-7.3.0/gcc/C_002b_002b-Dialect-Options.html#index-fno-rtti
    )

    # common compiling options
    add_compile_options(
        -fno-strict-aliasing
        -fthreadsafe-statics
        -pthread
        -fstack-protector-strong
        -fno-short-enums
        -fPIC
    )

    if (ADDRESS_SANITIZER)
        add_compile_options(-fsanitize=address)
    endif()

    # set warnings
    add_compile_options(
        -Wall               # https://gcc.gnu.org/onlinedocs/gcc-4.8.5/gcc/Warning-Options.html#index-Wall-263
#        -Wextra             # https://gcc.gnu.org/onlinedocs/gcc-4.8.5/gcc/Warning-Options.html#index-Wextra-266
#        -Wpedantic          # https://gcc.gnu.org/onlinedocs/gcc-4.8.5/gcc/Warning-Options.html#index-Wpedantic-261
#        -Wconversion        # https://gcc.gnu.org/onlinedocs/gcc-4.8.5/gcc/Warning-Options.html#index-Wconversion-426
#        -Wempty-body
        -Wfatal-errors
#        -Wshadow
#        -Wzero-as-null-pointer-constant
        -Wunreachable-code
        -Wno-missing-field-initializers
        -Werror=return-type
        -Werror=write-strings
        -Werror=unused-label
        -Werror=switch
    )

    add_definitions(-D_GNU_SOURCE)

    if (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang OR ${CMAKE_CXX_COMPILER_ID} STREQUAL AppleClang)
        # add_compile_options(-Wabstract-final-class -Wshift-overflow -Wint-to-void-pointer-cast)
    endif()

    if (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
        # add_compile_options(-Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override)
        # add_compile_options(-fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free)  // for tcmalloc
    endif()

    #==============================================================================
    # Althrough CMake documentation (3.6.3) says add_compile_options supports
    # generator expressions, the actual result is -buggy-.
    # add_compile_options doesn't fully working with generator expressions.
    #
    # $<$<OR:?1,?2>:...>
    # for OR expression, do not insert any whitespace character between ?1 and ?2.
    #==============================================================================
    # options for Debug
    #add_compile_options($<$<OR:$<CONFIG:Debug>,$<CONFIG:debug>>:-g2 -ggdb -O0 -fno-omit-frame-pointer>)
    set_property(DIRECTORY APPEND PROPERTY COMPILE_OPTIONS $<$<OR:$<CONFIG:Debug>,$<CONFIG:debug>>:-g3 -ggdb -O0 -fno-omit-frame-pointer>)
    set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<OR:$<CONFIG:Debug>,$<CONFIG:debug>>:DEBUG _DEBUG>)

    # options for Release & RelWithDebInfo
    # add_compile_options($<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>,$<CONFIG:release>>:-fomit-frame-pointer -ffunction-sections -fdata-sections>)
    set_property(DIRECTORY APPEND PROPERTY COMPILE_OPTIONS $<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>,$<CONFIG:release>>:-ffunction-sections -fdata-sections>)
    set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>,$<CONFIG:release>>:NDEBUG>)

    # options for Release
    add_compile_options($<$<OR:$<CONFIG:Release>,$<CONFIG:release>>:-g0>)

    # options for RelWithDebInfo
    add_compile_options($<$<CONFIG:RelWithDebInfo>:-g1>)
    add_compile_options($<$<CONFIG:RelWithDebInfo>:-ggdb>)

elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)

    # common compile options
    add_compile_options(
        /Zc:auto,forScope,inline,wchar_t
        /Zc:externConstexpr   # https://docs.microsoft.com/en-us/cpp/build/reference/zc-externconstexpr
        /Zc:implicitNoexcept  # https://docs.microsoft.com/en-us/cpp/build/reference/zc-implicitnoexcept-implicit-exception-specifiers
        /Zc:noexceptTypes     # https://docs.microsoft.com/en-us/cpp/build/reference/zc-noexcepttypes
        /Zc:referenceBinding  # https://docs.microsoft.com/en-us/cpp/build/reference/zc-referencebinding-enforce-reference-binding-rules
        /Zc:rvalueCast        # https://docs.microsoft.com/en-us/cpp/build/reference/zc-rvaluecast-enforce-type-conversion-rules
        /Zc:sizedDealloc      # https://docs.microsoft.com/en-us/cpp/build/reference/zc-sizeddealloc-enable-global-sized-dealloc-functions
        /Zc:strictStrings     # https://docs.microsoft.com/en-us/cpp/build/reference/zc-strictstrings-disable-string-literal-type-conversion
        /Zc:threadSafeInit    # https://docs.microsoft.com/en-us/cpp/build/reference/zc-threadsafeinit-thread-safe-local-static-initialization
        /Zc:throwingNew       # https://docs.microsoft.com/en-us/cpp/build/reference/zc-throwingnew-assume-operator-new-throws
        /Zc:trigraphs-        # https://docs.microsoft.com/en-us/cpp/build/reference/zc-trigraphs-trigraphs-substitution
        /Zc:__cplusplus       # https://docs.microsoft.com/en-us/cpp/build/reference/zc-cplusplus
        /permissive-          # https://blogs.msdn.microsoft.com/vcblog/2016/11/16/permissive-switch/
                              # https://blogs.msdn.microsoft.com/vcblog/2017/09/11/two-phase-name-lookup-support-comes-to-msvc/
                              # https://docs.microsoft.com/en-us/cpp/build/reference/permissive-standards-conformance
        /volatile:iso         # https://docs.microsoft.com/en-us/cpp/build/reference/volatile-volatile-keyword-interpretation
        /GS
        /Zi
        /Gm-                  # Disable Minimal Rebuild
        # /sdl                 # Adds recommended Security Development Lifecycle (SDL) checks
        /EHsc
        /fp:precise
        /errorReport:prompt
        /GF                   # Eliminate Duplicate Strings
        /Gd                   # Specifies the __cdecl calling convention for all functions except C++ member functions and functions that are marked __stdcall, __fastcall, or __vectorcall.
        /diagnostics:caret    # https://docs.microsoft.com/en-us/cpp/build/reference/diagnostics-compiler-diagnostic-options
        /doc
        /wd4634               # XML document comment target: cannot be applied: reason
        /utf-8                # https://docs.microsoft.com/en-us/cpp/build/reference/utf-8-set-source-and-executable-character-sets-to-utf-8
        /WL                   # https://docs.microsoft.com/en-us/cpp/build/reference/wl-enable-one-line-diagnostics
    )

    if (CMAKE_CXX_FLAGS MATCHES "/W[0-3]")
        string(REGEX REPLACE "/W[0-3]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        add_compile_options(/W4)
    endif()

    add_definitions(
        -D_SCL_SECURE_NO_WARNINGS
        -D_CRT_SECURE_NO_WARNINGS
        -DNOMINMAX
        -D_WIN32_WINNT=0x0600   # Vista and above
        -D_WINDOWS
        -D_UNICODE
        -DUNICODE)

    # options for Debug
    set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<OR:$<CONFIG:Debug>,$<CONFIG:debug>>:_DEBUG DEBUG>)

    # options for Release & RelWithDebInfo
    # /O2 equivalent to /Og /Oi /Ot /Oy /Ob2 /Gs /GF /Gy
    # https://docs.microsoft.com/en-us/cpp/build/reference/o1-o2-minimize-size-maximize-speed
    # set_property(DIRECTORY APPEND PROPERTY COMPILE_OPTIONS $<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>:/O2>)
    add_compile_options($<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>,$<CONFIG:release>>:/O2>)
    set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<OR:$<CONFIG:Release>,$<CONFIG:release>>:NDEBUG>)
else()
    message(FATAL_ERROR "Not supported C++ Compiler: " ${CMAKE_CXX_COMPILER_ID})
endif()


if (${CMAKE_SYSTEM_NAME} STREQUAL Linux OR ${CMAKE_SYSTEM_NAME} STREQUAL Darwin)
#  find_package(PkgConfig REQUIRED)
endif()

message(STATUS "CMAKE_BUILD_TYPE:" ${CMAKE_BUILD_TYPE})
message(STATUS "CMAKE_SYSTEM_NAME:" ${CMAKE_SYSTEM_NAME})
message(STATUS "CMAKE_CXX_COMPILER_ID:" ${CMAKE_CXX_COMPILER_ID})
message(STATUS "XENABLE_CODE_COVERAGE:" ${XENABLE_CODE_COVERAGE})
message(STATUS "XENABLE_TESTS:" ${XENABLE_TESTS})
message(STATUS "XENABLE_VERBOSE_DBG:" ${XENABLE_VERBOSE_DBG})
message(STATUS "BUILD_METRICS:" ${BUILD_METRICS})
message(STATUS "ADDRESS_SANITIZER:" ${ADDRESS_SANITIZER})


find_package(Threads REQUIRED)

include_directories(include)  # xtopcom build warning will not print
include_directories(SYSTEM depends/include)
include_directories(SYSTEM depends/include)

link_directories(${LIBRARY_OUTPUT_PATH})
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_compile_options(
        # -Wl,-z,relro
        # -Wl,-z,noexecstack
        # -Wl,--no-undefined
        # -Wl,-z,now
        -Wno-unused-function
        -Wno-unused-value
        -Wno-unused-variable
    )
    link_directories(depends/libs/linux)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    add_compile_options(
        -Wno-unused-private-field
        -Wno-inconsistent-missing-override
        # -Wno-constexpr-not-const
        -Wno-c++14-extensions
        -Wno-unused-parameter
        -Wno-sign-conversion
        -Wno-delete-non-virtual-dtor
        -Wno-sign-compare
        -Wno-conversion
        # -Wno-missing-declarations
        # -Wno-conversion
        # -Wno-float-conversion
        -Wno-unused-variable
        -Wno-reorder
        -Wno-pragma-once-outside-header
        -Wno-shadow
        # -Wno-tautological-constant-out-of-range-compare
        # -Wno-shift-op-parentheses
        -Wno-unreachable-code
        -Wno-pessimizing-move
        -Wno-gnu-zero-variadic-macro-arguments
        -Wno-return-type
        -Wno-unused-function
        -Wno-shorten-64-to-32
        -Wno-unused-value
        -Wno-c++11-narrowing
        -Wno-unused-label
        -Wno-unused-lambda-capture
    )
    link_directories(depends/libs/macos)
endif()

# add_subdirectory(src)
aux_source_directory(./src xcons_src)
add_executable(xevent ${xcons_src})

if (XENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
