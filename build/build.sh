#!/bin/bash

##############################################################
# System-specific config
##############################################################

OS=linux

if [ "$OS" == "mac" ]; then
	QMAKE=/usr/local/Cellar/qt5/5.3.0/bin/qmake
	QMAKE_OPTIONS="-spec macx-clang CONFIG+=x86_64"
elif [ "$OS" == "linux" ]; then
	QMAKE=/opt/Qt5.4.0/5.4/gcc_64/bin/qmake
	QMAKE_OPTIONS="-spec linux-g++ CONFIG+=x86_64"
fi

##############################################################
# System-specific config
##############################################################

ACTION=$1
LAUNCH_AFTER_BUILD=$2

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
ROOT_DIR="$(dirname $SCRIPT_DIR)"

fn_usage() {
	echo "Usage: build.sh <action>"
	echo "<action> can be 'debug', 'release', 'debugrelease', 'clean-debug', 'clean-release'"
}

fn_exitOnError() {
	ERROR_CODE=$1
	if [ "$ERROR_CODE" != "0" ] && [ "$ERROR_CODE" != "" ]; then
		echo "Ended with error code $ERROR_CODE"
		exit $ERROR_CODE
	fi
}

fn_appName() {
	TYPE=$1
	OUTPUT=
	if [ "$TYPE" == "debug" ]; then
		OUTPUT="MultiViewer-debug"
	elif [ "$ACTION" == "release" ]; then
		OUTPUT="MultiViewer"
	elif [ "$ACTION" == "debugrelease" ]; then
		OUTPUT="MultiViewer"
	fi

	if [ "$OS" == "mac" ]; then
		OUTPUT="$OUTPUT.app"
	fi

	echo $OUTPUT
}

fn_runAction() {
	ACTION=$1
	BUILD_DIR=$2
	PROJECT_FILE=$3

	echo "Doing action: $ACTION"
	echo "Output: $(fn_appName $ACTION)"

	mkdir -p "$BUILD_DIR"
	cd "$BUILD_DIR"

	rm -rf $BUILD_DIR/$(fn_appName $ACTION)

	if [ "$ACTION" == "debug" ]; then
		$QMAKE $PROJECT_FILE -r $QMAKE_OPTIONS CONFIG+=debug
	elif [ "$ACTION" == "release" ]; then
		$QMAKE $PROJECT_FILE -r $QMAKE_OPTIONS
	elif [ "$ACTION" == "debugrelease" ]; then
		$QMAKE $PROJECT_FILE -r $QMAKE_OPTIONS CONFIG+=debug AK_IS_DEBUGRELEASE=1
	fi

	fn_exitOnError $?

	make

	fn_exitOnError $?

	if [ "$ACTION" == "debugrelease" ]; then
		mv $BUILD_DIR/$(fn_appName "debug") $BUILD_DIR/$(fn_appName "debugrelease")
	fi

	if [ "$OS" == "mac" ]; then
		if [ "$ACTION" == "debug" ]; then
			cp -v $SCRIPT_DIR/version/Info-debug.plist $BUILD_DIR/$(fn_appName $ACTION)/Contents/Info.plist
			cp -v $SCRIPT_DIR/version/MultiViewer-debug.sh $BUILD_DIR/$(fn_appName $ACTION)/Contents/MacOS/MultiViewer-debug.sh
		elif [ "$ACTION" == "release" ]; then
			cp -v $SCRIPT_DIR/version/Info.plist $BUILD_DIR/$(fn_appName $ACTION)/Contents
			cp -v $SCRIPT_DIR/version/MultiViewer.sh $BUILD_DIR/$(fn_appName $ACTION)/Contents/MacOS
		elif [ "$ACTION" == "debugrelease" ]; then
			cp -v $SCRIPT_DIR/version/Info-debugrelease.plist $BUILD_DIR/$(fn_appName $ACTION)/Contents/Info.plist
			cp -v $SCRIPT_DIR/version/MultiViewer-debugrelease.sh $BUILD_DIR/$(fn_appName $ACTION)/Contents/MacOS/MultiViewer-debugrelease.sh
			mv $BUILD_DIR/$(fn_appName $ACTION)/Contents/MacOS/MultiViewer-debug $BUILD_DIR/$(fn_appName $ACTION)/Contents/MacOS/MultiViewer-debugrelease
		fi
	fi
}

if [ "$ACTION" == "clean-debug" ]; then
	rm -rfv $ROOT_DIR/build-debug
	exit
elif [ "$ACTION" == "clean-release" ]; then
	rm -rfv $ROOT_DIR/build-release
	exit
fi

if [ "$ACTION" != "debug" ] && [ "$ACTION" != "release" ] && [ "$ACTION" != "debugrelease" ]; then
	fn_usage
	exit 1
fi

$ROOT_DIR/plugins/copyplugins.sh

if [ "$ACTION" == "debug" ] || [ "$ACTION" == "debugrelease" ]; then
	mkdir -p ~/.config/akview
	cd ~/.config/akview
	rm -f plugins
	ln -s $ROOT_DIR/plugins/debug plugins
	cd -
fi

# Then build the application
if [ "$ACTION" == "release" ] || [ "$ACTION" == "debugrelease" ]; then
	$SCRIPT_DIR/version/CreateVersionInfo
fi
fn_runAction $ACTION $ROOT_DIR/build-$ACTION $ROOT_DIR/src/MultiViewer.pro

if [ "$LAUNCH_AFTER_BUILD" == "1" ]; then
	# rm ~/Desktop/test\ -\ copie/*\~
	# rsync -a --size-only ~/Desktop/test/ ~/Desktop/test\ -\ copie/

	# open -a $BUILD_DIR/$(fn_appName $ACTION) "/Users/laurent/Docs/Photos/_afaire/Telephone/100ANDRO/DSC_2077.jpg"
	# open -a $BUILD_DIR/$(fn_appName $ACTION)
	# open -a $BUILD_DIR/$(fn_appName $ACTION) "/Users/laurent/Desktop/sort/DSC_2.jpg"
	# open -a $BUILD_DIR/$(fn_appName $ACTION)

	$BUILD_DIR/$(fn_appName $ACTION)
fi

# if [ "$ACTION" == "debugrelease" ]; then
# 	rm -rf $BUILD_DIR/../$(fn_appName $ACTION)
# 	mv $BUILD_DIR/$(fn_appName $ACTION) $BUILD_DIR/../
# 	open $BUILD_DIR/../
# fi