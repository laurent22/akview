function buildForm() {
	var form = [];
	var e;

	e = ui.newFormElement("select", "unit", "Unit");
	e.options = [
		{ title: "Pixels", value: "pixels" },
		{ title: "Percent", value: "percent" }
	];
	e.value = plugin.setting("resize/unit", "pixels");
	form.push(e);

	e = ui.newFormElement("text", "width", "Width");
	e.validator = "int";
	form.push(e);

	e = ui.newFormElement("text", "height", "Height");
	e.validator = "int";
	form.push(e);

	e = ui.newFormElement("checkbox", "preserveAspectRatio", "Preserve aspect ratio", "If this option is selected, enter either the width or height. The other dimension will be automatically calculated.");
	e.value = plugin.setting("resize/preserveAspectRatio", true);
	form.push(e);

	return form;
}

function main() {
	var formElements = buildForm();

	while (true) {
		var result = ui.form(formElements, "Resize");
		if (!result) break;

		var w = result.width.trim() == "" ? null : Number(result.width.trim());
		var h = result.height.trim() == "" ? null : Number(result.height.trim());

		if (w === null && h === null) {
			ui.messageBox("No width or height specified.");
			continue;
		}

		if (result.unit == "pixels" && (w === null || h === null)) {
			ui.messageBox("Both width and height must be specified.");
			continue;
		}

		if ((w !== null && w < 1) || (h !== null && h < 1)) {
			ui.messageBox("Width and height must be greated than 1.");
			continue;
		}

		var imageSize = input.imageSize;

		if (result.unit == "pixels") {
			// nothing to do
		} else if (result.unit == "percent") {
			if (w) w = Math.round(imageSize.width * (w / 100));
			if (h) h = Math.round(imageSize.height * (h / 100));
		} else {
			throw new Exception("invalid unit: " + result.unit);
		}

		if (result.preserveAspectRatio) {
			var ratio = imageSize.width / imageSize.height;
			if (w === null) {
				w = Math.round(h * ratio);
			} else {
				h = Math.round(w / ratio);
			}
		}

		application.pushUndoState();
		system.exec("mogrify -resize " + w + "x" + h + "! " + input.escapedFilePath);

		for (var n in result) {
			if (n == "width" || n == "height") continue;
			plugin.setSetting("resize/" + n, result[n]);
		}

		break;
	}
}

main();