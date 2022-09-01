package main

import (
	"fmt"
	"net/http"
	"io/ioutil"

	//"context"
	//"encoding/json"
)

func HttpRequest(url string) ([]byte, error) {
	// Send request to API
	res, err := http.Get(url)
	if err != nil {
		return []byte{}, err
	}

	// Attempt to decode
	data, err := ioutil.ReadAll(res.Body)
	if err != nil {
		fmt.Print("Error decoding!")
		return []byte{}, err
	}

	res.Body.Close()
	return data, nil
}


func GetUrl(username string, mapToken string) string {
	//return "https://api.mapbox.com/styles/v1/"+username+"/"+styleId+"/static/"+overlay+"/"+longitude+","+latitude+","+zoom+","+bearing+",{pitch}|{bbox}|{auto}/{width}x{height}{@2x}"
	//return "https://api.mapbox.com/styles/v1/mapbox/light-v10/static/pin-s-l+000(-87.0186,32.4055)/-87.0186,32.4055,14/500x300?access_token=pk.eyJ1IjoiaW50ZWdyYXNqb243IiwiYSI6ImNsN2o1NTNyczA0a2gzb3F6dmdmZTFoa20ifQ.0wQrTe8UJWK0KSS3s82loA"
	//return `https://covidtrackerapi.bsg.ox.ac.uk/api/v2/stringency/actions/NO/12-12-2021/`
	return `https://api.mapbox.com/styles/v1/mapbox/light-v10/static/pin-s-l+000(-87.0186,32.4055)/-87.0186,32.4055,14/500x300?access_token=pk.eyJ1IjoiaW50ZWdyYXNqb243IiwiYSI6ImNsN2o1NTNyczA0a2gzb3F6dmdmZTFoa20ifQ.0wQrTe8UJWK0KSS3s82loA`
}
 

func main() {
	data, err := HttpRequest(GetUrl("integrasjon7", "pk.eyJ1IjoiaW50ZWdyYXNqb243IiwiYSI6ImNsN2o1NTNyczA0a2gzb3F6dmdmZTFoa20ifQ.0wQrTe8UJWK0KSS3s82loA"))

	if err != nil {
		fmt.Print("Error: ", err)
		return
	}

	ioutil.WriteFile("google_logo.png", data, 0666)
}