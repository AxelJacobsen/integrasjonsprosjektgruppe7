package main

import (
	"fmt"
	"io/ioutil"

	//"context"
	//"encoding/json"
	"testproject/mapboxexamples"
	request "testproject/request"
)

func main() {
	data, err := request.HttpRequest(mapboxexamples.GetRasterTilesURL(0, 0, 0))
	//data, err := request.HttpRequest(mapboxexamples.GetStaticmapURL(1, -77.043686, 38.892035))

	if err != nil {
		fmt.Print("Error: ", err)
		return
	}

	ioutil.WriteFile("google_logo.png", data, 0666)
}
