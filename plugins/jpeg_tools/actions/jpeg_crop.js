var r = input.selectionRect;
if (r) {
	application.pushUndoState();
	system.exec("jpegtran -crop " + r.width + "x" + r.height + "+" + r.x + "+" + r.y + " -outfile " + input.escapedFilePath + " " + input.escapedFilePath);
}