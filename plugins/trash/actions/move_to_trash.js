if (system.os == "osx") {
	system.exec("osascript", ["-l", "AppleScript", "-e", "tell application \"Finder\" to move POSIX file \"" + input.filePath + "\" to trash"]);
} else if (system.os == "windows") {
	// TODO
}