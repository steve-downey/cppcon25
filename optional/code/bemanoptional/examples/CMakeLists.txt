# cmake-format: off
# examples/CMakeLists.txt -*-makefile-*-
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
# cmake-format: on

set(BEMAN_OPTIONAL_LIBRARY "beman_optional")

include(GNUInstallDirs)

# List of all buildable examples.
set(EXAMPLES
    base_derived_cast
    concept_checks
    optional_ref
    pythagorean_triples
    range_loop
    sample
    std_vs_beman
)

foreach(example ${EXAMPLES})
    # Add example executable.
    add_executable(beman_optional_${example} "")

    # Add example source file.
    target_sources(beman_optional_${example} PRIVATE ${example}.cpp)

    # Link example with the library.
    target_link_libraries(beman_optional_${example} "${BEMAN_OPTIONAL_LIBRARY}")

    # Install .
    install(
        TARGETS
            beman_optional_${example}
            COMPONENT
            beman_optional_examples
            DESTINATION
            ${CMAKE_INSTALL_BINDIR}
    )
endforeach()
