#!/bin/bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
mkdir -p $SCRIPT_DIR/debug/
mkdir -p $SCRIPT_DIR/release/
find $SCRIPT_DIR/build-*-Debug -name "*.dylib" -exec cp {} $SCRIPT_DIR/debug/ \;
find $SCRIPT_DIR/build-*-Release -name "*.dylib" -exec cp {} $SCRIPT_DIR/release/ \;