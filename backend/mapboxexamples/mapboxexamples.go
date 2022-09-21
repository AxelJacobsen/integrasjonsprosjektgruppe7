package mapboxexamples

import (
	"fmt"
	"math"
	"testproject"
)

/**
 *	Converts slippys map coordinates to normal coordinates (longitude, latitude).
 *
 *	@param zoom - Zoom.
 *	@param xtile - X-coordinate for slippys map.
 *	@param ytile - Y-coordinate for slippys map.
 *
 *	@see https://wiki.openstreetmap.org/wiki/Slippy_map_tilenames#Tile_numbers_to_lon./lat.
 *
 *	@return longitude, latitude
 */
func SlippysToCoords(zoom float64, xtile float64, ytile float64) (float64, float64) {
	// Convert
	n := math.Pow(2, zoom)
	longitude := xtile/n*360.0 - 180.0
	latitude := math.Atan(math.Sinh(math.Pi*(1.0-2.0*ytile/n))) * 180.0 / math.Pi

	// Restrict to mapbox's boundries
	if longitude > 180 {
		longitude = 180
	} else if longitude < -180 {
		longitude = -180
	}
	if latitude > 85.0511 {
		latitude = 85.0511
	} else if latitude < -85.0511 {
		latitude = -85.0511
	}

	// Return
	return longitude, latitude
}

/**
 *	Converts normal coordinates (longitude, latitude) to slippys map coordinates (xtile, ytile).
 *
 *	@param zoom - Zoom.
 *	@param longitude - The longitude.
 *	@param latitude - The latitude.
 *
 *	@see https://wiki.openstreetmap.org/wiki/Slippy_map_tilenames#Lon./lat._to_tile_numbers_2
 *
 *	@return xtile, ytile
 */
func CoordsToSlippys(zoom float64, longitude float64, latitude float64) (float64, float64) {
	latitude_rad := latitude / 180.0 * math.Pi
	n := math.Pow(2, zoom)
	xtile := n * ((longitude + 180.0) / 360.0)
	ytile := n * (1 - (math.Log(math.Tan(latitude_rad)+1.0/math.Cos(latitude_rad)) / math.Pi)) / 2.0
	return xtile, ytile
}

/**
 *	Gets the URL to fetch the raster tiles of a given area.
 *
 *	@param zoom - Zoom.
 *	@param xtile - X-coordinate (slippys).
 *	@param ytile - Y-coordinate (slippys).
 *
 *	@see https://docs.mapbox.com/api/maps/raster-tiles/ for a description of the API endpoint used.
 *	@see https://wiki.openstreetmap.org/wiki/Slippy_map_tilenames for a description of the coordinate system.
 *	@see https://docs.mapbox.com/help/glossary/zoom-level/ for a description of the zoom.
 *
 *	@return URL to get the raster tiles.
 */
func GetRasterTilesURL(zoom float64, xtile float64, ytile float64) string {
	return fmt.Sprintf(
		`https://api.mapbox.com/v4/%s/%f/%f/%f%s%s?access_token=%s%s`,
		"mapbox.satellite", //"mapbox.mapbox-streets-v8",
		zoom,
		xtile,
		ytile,
		"@2x",    // 2x quality
		".jpg90", // .jpg with 90% quality
		testproject.MAPTOKEN,
		`&addlayer={"id":"road-overlay","type":"line","source":"composite","source-layer":"road","filter":["==",["get","class"],"motorway"],"paint":{"line-color":"%23ff0000","line-width":5}}`,
	)
}

/**
 *	Gets the URL to fetch the static image of a given area.
 *
 *	@param zoom - Zoom.
 *	@param longitude - X-coordinate.
 *	@param latitude - Y-coordinate.
 *
 *	@see https://docs.mapbox.com/api/maps/raster-tiles/ for a description of the API endpoint used.
 *	@see https://wiki.openstreetmap.org/wiki/Slippy_map_tilenames for a description of the coordinate system.
 *	@see https://docs.mapbox.com/help/glossary/zoom-level/ for a description of the zoom.
 *
 *	@return URL to get the raster tiles.
 */
func GetStaticmapURL(zoom float64, longitude float64, latitude float64) string {
	return fmt.Sprintf(
		//`https://api.mapbox.com/styles/v1/mapbox/%s/static/pin-s-l+000(-87.0186,32.4055)/-87.0186,32.4055,14/500x300?access_token=%s`, Example with streets etc
		//`https://api.mapbox.com/styles/v1/mapbox/%s/static/-77.043686,38.892035,15.25,0,60/400x400?access_token=%s"`, Doesnt work!
		`https://api.mapbox.com/styles/v1/mapbox/%s/static/%f,%f,%f/512x512?access_token=%s`,
		"streets-v11",
		longitude,
		latitude,
		zoom,
		testproject.MAPTOKEN,
	)
}

/**
 *	Gets the URL to fetch the vector tiles of a given area.
 */
func GetVectorTilesURL() string {
	return fmt.Sprintf(
		`https://api.mapbox.com/v4/mapbox.mapbox-streets-v8/12/1171/1566.mvt?style=mapbox://styles/mapbox/streets-v11@00&access_token=%s`,
		testproject.MAPTOKEN,
	)
}
