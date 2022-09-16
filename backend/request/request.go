package request

import (
	"fmt"
	"io/ioutil"
	"net/http"
)

/**
 *	GET request for a given URL.
 *
 *	@param url - The URL.
 *	@return A byte array and error containing the result and error message (if any).
 */
func HttpRequest(url string) ([]byte, error) {
	// Send request to API
	res, err := http.Get(url)
	if err != nil {
		fmt.Print("Error fetching! ", err)
		return []byte{}, err
	}

	// Attempt to decode
	data, err := ioutil.ReadAll(res.Body)
	if err != nil {
		fmt.Print("Error decoding! ", err)
		return []byte{}, err
	}

	res.Body.Close()
	return data, nil
}
