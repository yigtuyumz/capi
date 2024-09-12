#!/usr/bin/env bash

# ~/.indent.pro
# --break-after-boolean-operator          // Do not prefer to break long lines before boolean operators.
# --braces-on-if-line                     // Put braces on line with if, etc.
# --cuddle-else                           // Cuddle else and preceding '}'.
# --cuddle-do-while                       // Cuddle while of do {} while; and preceding '}'.
# --line-length80                         // Set maximum line length for non-comment lines to n.
# --case-indentation4                     // Case label indent of n spaces.
# --space-after-cast                      // Put a space after a cast operator.
# --braces-on-struct-decl-line            // Put braces on struct declaration line.
# --blank-lines-before-block-comments     // Force blank lines before block comments.
# --no-space-after-function-call-names    // Do not put space after the function in function calls.
# --blank-lines-after-declarations        // Force blank lines after the declarations.
# --swallow-optional-blank-lines          // Swallow optional blank lines.
# --indent-level4                         // Set indentation level to n spaces.
# --no-tabs                               // Use spaces instead of tabs.
# --leave-preprocessor-space              // Leave space between '#' and preprocessor directive.
# --preprocessor-indentation1             // Specify the indentation for preprocessor conditional statements.

set -e

cd ./src

indent *.c
indent *.h
find . -name "*.*~" -exec rm {} \;
