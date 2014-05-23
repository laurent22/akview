#!/bin/bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

cd $SCRIPT_DIR/../build-MultiViewer-Qt_5_2_1-Release

echo "Running macdeployqt..."
/usr/local/Cellar/qt5/5.2.1/bin/macdeployqt MultiViewer.app -qmldir=/Users/laurent/Docs/PROGS/C++/mv/src

echo "Copying own plugins..."
mkdir -p MultiViewer.app/Contents/PlugIns/multiviewer
cp ../plugins/release/* MultiViewer.app/Contents/PlugIns/multiviewer

echo "Copying version info..."
cp $SCRIPT_DIR/version/Info.plist MultiViewer.app/Contents

open .

# echo "Moving to Applications folder..."
# rm -rf /Applications/MultiViewer.app
# mv MultiViewer.app /Applications/