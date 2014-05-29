#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

FREEIMAGE_LIB_PATH=/usr/local/lib/libfreeimage.dylib
QT5_LIB_BASE_PATH=/usr/local/opt/qt5/lib
MACDEPLOYQT_PATH=/usr/local/Cellar/qt5/5.3.0/bin/macdeployqt
APP_RELEASE_DIR=$SCRIPT_DIR/../build-MultiViewer-Qt_5_3_0-Release

function updateLibPath {
	if [[ -n "$(otool -L $1 | grep $2)" ]]; then
		echo "Updating $2 path..."
		install_name_tool -change "$3" "$4" $1
	fi
}  

cd $APP_RELEASE_DIR

if [[ ! -d MultiViewer.app ]]; then
	echo "Cannot find MultiViewer.app"
	exit 1	
fi

echo "Running macdeployqt..."
$MACDEPLOYQT_PATH MultiViewer.app -qmldir=$SCRIPT_DIR/../src

echo "Copying own plugins..."
mkdir -p MultiViewer.app/Contents/PlugIns/multiviewer
cp ../plugins/release/* MultiViewer.app/Contents/PlugIns/multiviewer

echo "Copying FreeImage lib..."
cp "$FREEIMAGE_LIB_PATH" MultiViewer.app/Contents/PlugIns/multiviewer/

echo "Fixing plugin lib paths..."
for FILE in MultiViewer.app/Contents/PlugIns/multiviewer/*.dylib; do
	if [[ -n "$(echo $FILE | grep libfreeimage.dylib)" ]]; then
		echo "Skipping $FILE..."
		continue
	fi
	
	echo "Processing $FILE..."
	
	updateLibPath $FILE "QtWidgets" "$QT5_LIB_BASE_PATH/QtWidgets.framework/Versions/5/QtWidgets" "@executable_path/../Frameworks/QtWidgets.framework/Versions/5/QtWidgets"
	updateLibPath $FILE "QtGui" "$QT5_LIB_BASE_PATH/QtGui.framework/Versions/5/QtGui" "@executable_path/../Frameworks/QtGui.framework/Versions/5/QtGui"
	updateLibPath $FILE "QtCore" "$QT5_LIB_BASE_PATH/QtCore.framework/Versions/5/QtCore" "@executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore"
	updateLibPath $FILE "libfreeimage.dylib" "$FREEIMAGE_LIB_PATH" "@executable_path/libfreeimage.dylib"
done

echo "Copying version info..."
cp $SCRIPT_DIR/version/Info.plist MultiViewer.app/Contents

rm -rf ../MultiViewer.app
mv MultiViewer.app ..
open ..