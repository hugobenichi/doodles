package main

import "fmt"

/*
	minimum testable feature
		read hardcoded directory, print dates of content
		do it recursively
		compute target copy path
		prepare dirs and do copy
	minimum viable feature 1
		actually do the copy (in dry mode)

	feature 2
		compare two directories and propose deletion of missing matching files

	feature 3
		take into account prefix number and prepend it to the files (fuji 108, ...)

	feature 4
		add configuration file for reading src, target dirs

	feature 5
		add scanners for detecting work to do
*/

const (
	ConfigPath = "~/.picassist"
)

type Config struct {
	Input      string
	Staging    string
	DateFormat string
}

func ReadConfigOrDie() Config {

}

func main() {
	fmt.Println("hw")
}
