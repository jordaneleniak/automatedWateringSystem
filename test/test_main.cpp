#define CATCH_CONFIG_MAIN
#include "../lib/catch/catch_amalgamated.hpp"

/*
===============================================================================
Unit Test Entry Point (Catch2)
===============================================================================

This file provides the main() entry point for all unit tests using Catch2.

-------------------------------------------------------------------------------
HOW TO RUN TESTS
-------------------------------------------------------------------------------

From project root:

    pio run -t clean
    rmdir /s /q .pio
    pio test -e test -v

-------------------------------------------------------------------------------
NOTES
-------------------------------------------------------------------------------

- All test files must be located directly under the /test directory
  for PlatformIO to detect them correctly.

- During development, tests may be organized into subfolders (e.g. math/,
  logic/, etc.), but they MUST be moved back to /test before running.

- Only ONE file should define:
      #define CATCH_CONFIG_MAIN

-------------------------------------------------------------------------------
CURRENT STATUS
-------------------------------------------------------------------------------

- mapFloat and clamp test suites implemented
- Extensive edge case coverage added
- Known issue: double-inverted range behavior in mapFloat is undefined

===============================================================================
*/