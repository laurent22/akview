#!/bin/bash
cd ~/Docs/PROGS/C++/mv/build-MultiViewer-Qt_5_2_1-Release
/usr/local/Cellar/qt5/5.2.1/bin/macdeployqt MultiViewer.app -qmldir=/Users/laurent/Docs/PROGS/C++/mv/src
# Copy own plugins
mkdir -p MultiViewer.app/Contents/PlugIns/multiviewer
cp ../plugins/release/* MultiViewer.app/Contents/PlugIns/multiviewer