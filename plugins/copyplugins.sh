#!/bin/bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
mkdir -p $SCRIPT_DIR/debug/
mkdir -p $SCRIPT_DIR/release/
rsync -a $SCRIPT_DIR/jpeg_tools/ $SCRIPT_DIR/debug/jpeg_tools/
rsync -a $SCRIPT_DIR/jpeg_tools/ $SCRIPT_DIR/release/jpeg_tools/

# cp $SCRIPT_DIR/jpeg_auto_rotate/jpeg_auto_rotate.json $SCRIPT_DIR/release

# find $SCRIPT_DIR/build-*-debug -name "*.dylib" -exec cp {} $SCRIPT_DIR/debug/ \;
# find $SCRIPT_DIR/build-*-release -name "*.dylib" -exec cp {} $SCRIPT_DIR/release/ \;
# find $SCRIPT_DIR/Mv* -name "*.json" -exec cp {} $SCRIPT_DIR/debug/ \;
# find $SCRIPT_DIR/Mv* -name "*.json" -exec cp {} $SCRIPT_DIR/release/ \;
