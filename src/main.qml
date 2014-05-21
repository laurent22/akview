import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Dialogs 1.0

ApplicationWindow {
	FileDialog {
		id: fileDialog
		title: "Please choose a file"
		onAccepted: {
			console.log("You chose: " + fileDialog.fileUrls)
			image.source = fileDialog.fileUrl
			//Qt.quit()
		}
		onRejected: {
			console.log("Canceled")
			//Qt.quit()
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

	menuBar: MenuBar {
		Menu {
			title: qsTr("File")
			MenuItem {
				text: qsTr("Exit")
				onTriggered: Qt.quit();
			}
		}
	}

	Image {
		id: image
		objectName: "image"
		fillMode: Image.PreserveAspectFit
		anchors.right: parent.right
		anchors.left: parent.left
		anchors.top: parent.top
		anchors.bottom: parent.bottom
		source: "file:///Users/laurent/Desktop/CH_12_05_2014.jpg"
		focus: true
		Keys.onPressed: {
			mainWindow.keypressed(event.key)
		}
	}
}
