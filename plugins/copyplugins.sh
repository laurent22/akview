#!/bin/bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
find $SCRIPT_DIR/build-*-Debug -name "*.dylib" -exec cp {} $SCRIPT_DIR/debug/ \;
find $SCRIPT_DIR/build-*-Release -name "*.dylib" -exec cp {} $SCRIPT_DIR/release/ \;