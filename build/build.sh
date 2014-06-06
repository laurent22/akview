#!/bin/bash

ACTION=$1
LAUNCH_AFTER_BUILD=$2

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
QMAKE=/usr/local/Cellar/qt5/5.3.0/bin/qmake

fn_usage() {
	echo "Usage: build.sh <action>"
	echo "<action> can be 'debug', 'release', 'clean-debug', 'clean-release'"
}

fn_exitOnError() {
	ERROR_CODE=$1
	if [ "$ERROR_CODE" != "0" ] && [ "$ERROR_CODE" != "" ]; then
		echo "Ended with error code $ERROR_CODE"
		exit $ERROR_CODE
	fi
}

fn_runAction() {
	ACTION=$1
	BUILD_DIR=$2
	PROJECT_FILE=$3

	echo "Doing action: $ACTION"

	mkdir -p "$BUILD_DIR"
	cd "$BUILD_DIR"

	if [ "$ACTION" == "debug" ]; then
		$QMAKE $PROJECT_FILE -r -spec macx-clang CONFIG+=x86_64 CONFIG+=debug CONFIG+=declarative_debug CONFIG+=qml_debug
	elif [ "$ACTION" == "release" ]; then
		$QMAKE $PROJECT_FILE -r -spec macx-clang CONFIG+=x86_64
	fi

	fn_exitOnError $?

	make

	fn_exitOnError $?
}

if [ "$ACTION" == "clean-debug" ]; then
	rm -rf $SCRIPT_DIR/../build-debug
	rm -rf $SCRIPT_DIR/../plugins/build-MvDeleteFile-debug
	rm -rf $SCRIPT_DIR/../plugins/build-MvJpegTools-debug
	rm -rf $SCRIPT_DIR/../plugins/build-MvReveal-debug
	exit
elif [ "$ACTION" == "clean-release" ]; then
	rm -rf $SCRIPT_DIR/../build-release
	rm -rf $SCRIPT_DIR/../plugins/build-MvDeleteFile-release
	rm -rf $SCRIPT_DIR/../plugins/build-MvJpegTools-release
	rm -rf $SCRIPT_DIR/../plugins/build-MvReveal-release
	exit
fi

if [ "$ACTION" != "debug" ] && [ "$ACTION" != "release" ]; then
	fn_usage
	exit 1
fi

# First, build all the plugins
fn_runAction $ACTION $SCRIPT_DIR/../plugins/build-MvDeleteFile-$ACTION $SCRIPT_DIR/../plugins/MvDeleteFile/MvDeleteFilePlugin.pro
fn_runAction $ACTION $SCRIPT_DIR/../plugins/build-MvJpegTools-$ACTION $SCRIPT_DIR/../plugins/MvJpegTools/MvJpegToolPlugin.pro
fn_runAction $ACTION $SCRIPT_DIR/../plugins/build-MvReveal-$ACTION $SCRIPT_DIR/../plugins/MvReveal/MvRevealPlugin.pro

$SCRIPT_DIR/../plugins/copyplugins.sh

# Then build the application
if [ "$ACTION" == "release" ]; then
	$SCRIPT_DIR/version/CreateVersionInfo
fi
fn_runAction $ACTION $SCRIPT_DIR/../build-$ACTION ../src/MultiViewer.pro

if [ "$LAUNCH_AFTER_BUILD" == "1" ]; then
	open -a $BUILD_DIR/MultiViewer.app ~/Desktop/test/DSC_1.jpg
fi