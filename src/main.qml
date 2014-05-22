import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Dialogs 1.0

ApplicationWindow {
	id: mainWindow
	objectName: "mainWindow"
	visible: true
	width: 640
	height: 480

	signal keypressed(int key, string text, int modifiers)
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
			mainWindow.keypressed(event.key, event.text, event.modifiers)
		}
	}

}
