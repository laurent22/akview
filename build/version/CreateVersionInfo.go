package main

import (
	"./osext"
	"log"
	"io/ioutil"
	"path/filepath"
	"strings"
	"strconv"
)

func readFileToString(filePath string) (string, error) {
	b, err := ioutil.ReadFile(filePath)
	if err != nil { return "", err }
	return string(b), nil	
}

func main() {
	versionInfoBasePath, err := osext.Executable()
	if err != nil { panic("Cannot get executable path.") }
	versionInfoBasePath = filepath.Dir(versionInfoBasePath)
	
	versionFilePath := versionInfoBasePath + "/version.txt"
	resourceTemplateFilePath := versionInfoBasePath + "/version_template.h"
	resourceFilePath := filepath.Dir(filepath.Dir(versionInfoBasePath)) + "/src/version.h"
	infoPlistTemplateFilePath := versionInfoBasePath + "/Info_template.plist"
	infoPlistFilePath := versionInfoBasePath + "/Info.plist"
	
	versionText, err := readFileToString(versionFilePath)
	if err != nil { panic("Could not read version number from " + versionFilePath) }

	resourceText, err := readFileToString(resourceTemplateFilePath)
	if err != nil { panic("Could not read resource template " + resourceTemplateFilePath) }

	splittedVersion := strings.Split(versionText, ".")
	
	buildNumber, err := strconv.Atoi(splittedVersion[len(splittedVersion) - 1])
	if err != nil { panic("Build number is not a number.") }

	buildNumber += 1
	buildNumberS := strconv.Itoa(buildNumber)
	splittedVersion[len(splittedVersion) - 1] = buildNumberS
	
	newVersionNumber := strings.Join(splittedVersion, ".")
	newVersionNumberComa := strings.Replace(newVersionNumber, ".", ",", -1)
	
	log.Println("Old version number: " + versionText)
	log.Println("New version number: " + newVersionNumber)
	
	resourceText = strings.Replace(resourceText, "____VERSION_COMA____", newVersionNumberComa, -1)
	resourceText = strings.Replace(resourceText, "____VERSION____", newVersionNumber, -1)
	
	err = ioutil.WriteFile(resourceFilePath, []byte(resourceText), 0700)
	if err != nil { panic("Cannot write to file " + resourceFilePath) }
	
	log.Println("Updated version info: " + resourceFilePath)
	
	err = ioutil.WriteFile(versionFilePath, []byte(newVersionNumber), 0700)
	if err != nil { panic("Cannot write to file " + versionFilePath) }
	
	
	
	
	resourceText, err = readFileToString(infoPlistTemplateFilePath)
	if err != nil { panic("Could not read Info.plist " + infoPlistTemplateFilePath) }
	
	resourceText = strings.Replace(resourceText, "____VERSION____", newVersionNumber, -1)
	err = ioutil.WriteFile(infoPlistFilePath, []byte(resourceText), 0700)
	if err != nil { panic("Cannot write to file " + infoPlistFilePath) }
	
	log.Println("Updated version info: " + infoPlistFilePath)
}