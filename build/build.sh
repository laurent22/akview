#!/bin/bash

TARGET=$1
LAUNCH_AFTER_BUILD=$2

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
QMAKE=/usr/local/Cellar/qt5/5.3.0/bin/qmake

fn_usage() {
	echo "Usage: build.sh <target>"
	echo "<target> can be 'debug', 'release', 'clean-debug', 'clean-release'"
}

fn_exitOnError() {
	ERROR_CODE=$1
	if [ "$ERROR_CODE" != "0" ]; then
		echo "Ended with error code $ERROR_CODE"
		exit $ERROR_CODE
	fi
}

if [ "$TARGET" == "clean-debug" ]; then
	rm -rf $SCRIPT_DIR/../build-debug
	exit $?
elif [ "$TARGET" == "clean-release" ]; then
	rm -rf $SCRIPT_DIR/../build-release
	exit $?
fi

if [ "$TARGET" != "debug" ] && [ "$TARGET" != "release" ]; then
	fn_usage
	exit 1
fi

BUILD_DIR=$SCRIPT_DIR/../build-$TARGET

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

if [ "$TARGET" == "debug" ]; then
	$QMAKE ../src/MultiViewer.pro -r -spec macx-clang CONFIG+=x86_64 CONFIG+=debug CONFIG+=declarative_debug CONFIG+=qml_debug
elif [ "$TARGET" == "release" ]; then
	$SCRIPT_DIR/version/CreateVersionInfo
	$QMAKE ../src/MultiViewer.pro -r -spec macx-clang CONFIG+=x86_64
fi

fn_exitOnError $?

make

fn_exitOnError $?

if [ "$LAUNCH_AFTER_BUILD" == "1" ]; then
	open MultiViewer.app
fi