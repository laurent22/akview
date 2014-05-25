#!/bin/bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

cd $SCRIPT_DIR/../build-MultiViewer-Qt_5_2_1-Release

echo "Running macdeployqt..."
/usr/local/Cellar/qt5/5.2.1/bin/macdeployqt MultiViewer.app -qmldir=/Users/laurent/Docs/PROGS/C++/mv/src

echo "Copying own plugins..."
mkdir -p MultiViewer.app/Contents/PlugIns/multiviewer
cp ../plugins/release/* MultiViewer.app/Contents/PlugIns/multiviewer

echo "Fixing plugin lib paths..."
for FILE in MultiViewer.app/Contents/PlugIns/multiviewer/*.dylib; do
	echo "Processing $FILE..."
	install_name_tool -change "/usr/local/opt/qt5/lib/QtWidgets.framework/Versions/5/QtWidgets" "@executable_path/../Frameworks/QtWidgets.framework/Versions/5/QtWidgets" $FILE
	install_name_tool -change "/usr/local/opt/qt5/lib/QtGui.framework/Versions/5/QtGui" "@executable_path/../Frameworks/QtGui.framework/Versions/5/QtGui" $FILE
	install_name_tool -change "/usr/local/opt/qt5/lib/QtCore.framework/Versions/5/QtCore" "@executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore" $FILE
done

echo "Copying FreeImage lib..."
cp "/usr/local/lib/libfreeimage.dylib" MultiViewer.app/Contents/PlugIns/multiviewer/
install_name_tool -change "/usr/local/lib/libfreeimage.dylib" "@executable_path/libfreeimage.dylib" $FILE

echo "Copying version info..."
cp $SCRIPT_DIR/version/Info.plist MultiViewer.app/Contents

open .