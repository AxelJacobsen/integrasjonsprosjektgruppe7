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
	//data, err := request.HttpRequest(mapboxexamples.GetStaticmapURL(1, -77.043686, 38.892035))

	// Oslo city
	zoom := 12.0
	lon := 10.757933
	lat := 59.911491

	xtile, ytile := mapboxexamples.CoordsToSlippys(zoom, lon, lat)

	data, err := request.HttpRequest(mapboxexamples.GetRasterTilesURL(zoom, xtile, ytile))

	if err != nil {
		fmt.Print("Error: ", err)
		return
	}

	ioutil.WriteFile("google_logo.png", data, 0666)
	//ioutil.WriteFile("vectortiles.vector", data, 0666)
}
