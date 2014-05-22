import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Dialogs 1.0

ApplicationWindow {
	FileDialog {
		id: fileDialog
		title: "Please choose a file"
		onAccepted: {
			console.log("You chose: " + fileDialog.fileUrls)
			mainWindow.sourceSelected(fileDialog.fileUrl)
		}
		onRejected: {
			console.log("Canceled")
		}

	}

	Action {
		id: openAction
		text: "&Open"
		shortcut: "Ctrl+O"
		onTriggered: fileDialog.open()
		tooltip: "Open an image"
	}

	id: mainWindow
	objectName: "mainWindow"
	visible: true
	width: 640
	height: 480
	title: qsTr("Hello World")

	signal keypressed(int key)
	signal sourceSelected(string source)

	Rectangle {
		anchors.right: parent.right
		anchors.left: parent.left
		anchors.top: parent.top
		anchors.bottom: parent.bottom
		color: "black"
	}

	Image {
		id: image
		objectName: "image"
		fillMode: Image.PreserveAspectFit
		anchors.right: parent.right
		anchors.left: parent.left
		anchors.top: parent.top
		anchors.bottom: parent.bottom
		focus: true
		smooth: true
		Keys.onPressed: {
			mainWindow.keypressed(event.key)
		}
	}

}
