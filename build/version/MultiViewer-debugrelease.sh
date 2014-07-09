#!/bin/sh

# We need to launch the application this way in order
# to be able to access the full system environment
# from the GUI app.

SCRIPT_DIR=$(dirname "$0")
source ~/.bash_profile
logger "$SCRIPT_DIR/MultiViewer-debugrelease"
exec "$SCRIPT_DIR/MultiViewer-debugrelease" $@