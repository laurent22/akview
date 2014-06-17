#!/bin/bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
mkdir -p $SCRIPT_DIR/debug/
mkdir -p $SCRIPT_DIR/release/
rsync -a --exclude "debug/" --exclude "release/" $SCRIPT_DIR/ $SCRIPT_DIR/debug/
rsync -a --exclude "debug/" --exclude "release/" $SCRIPT_DIR/ $SCRIPT_DIR/release/