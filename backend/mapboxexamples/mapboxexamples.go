package mapboxexamples

import (
	"fmt"
	"testproject"
)

/**
 *	Gets the URL to fetch the raster tiles of a given area.
 *
 *	@param zoom - Zoom.
 *	@param x - X-position of the frame.
 *	@param y - Y-position of the frame.
 *
 *	@see https://docs.mapbox.com/api/maps/raster-tiles/ for a description of the API endpoint used.
 *	@see https://wiki.openstreetmap.org/wiki/Slippy_map_tilenames for a description of the coordinate system.
 *	@see https://docs.mapbox.com/help/glossary/zoom-level/ for a description of the zoom.
 *
 *	@return URL to get the raster tiles.
 */
func GetRasterTilesURL(zoom int, x int, y int) string {
	return fmt.Sprintf(
		`https://api.mapbox.com/v4/%s/%d/%d/%d%s%s?access_token=%s`,
		"mapbox.satellite", //"mapbox.mapbox-streets-v8",
		zoom,
		x,
		y,
		"@2x",    // 2x quality
		".jpg90", // .jpg with 90% quality
		testproject.MAPTOKEN,
	)
}

/**
 *	Gets the URL to fetch the static image of a given area.
 *
 *	@param zoom - Zoom.
 *	@param x - X-position of the frame.
 *	@param y - Y-position of the frame.
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
		`https://api.mapbox.com/styles/v1/mapbox/%s/static/[%f,%f,%f,%f]/400x400?access_token=%s`,
		"streets-v11",
		longitude,
		latitude,
		longitude+45.0/zoom, // A little arbitrary...
		latitude+45.0/zoom,  // ...Should sort this out later
		testproject.MAPTOKEN,
	)
}
