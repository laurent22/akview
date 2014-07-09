var format = [];
format.push("Resolution: %g");
format.push("Format: %m");
format.push("Resolution: %xx%y %U");
format.push("Compress: %C (%Q%%)");
format.push("Orientation: %[orientation]");

var ext = fileinfo.suffix(input.filePath).toLowerCase();

if (ext == "jpg" || ext == "jpeg" || ext == "tif" || ext == "tiff") {
	format.push("%[EXIF:*]");
}

var cmd = 'identify -format "' + format.join("\\n") + '" ' + input.escapedFilePath;
system.exec(cmd);
console.showLastOutput();