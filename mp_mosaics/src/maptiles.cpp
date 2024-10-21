/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>

#include "maptiles.h"

using namespace std;


Point<3> convertToXYZ(LUVAPixel pixel) {
    return Point<3>( pixel.l, pixel.u, pixel.v );
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
    /**
     * @todo Implement this function!
     */
    MosaicCanvas* canvas = new MosaicCanvas(theSource.getRows(), theSource.getColumns());
    vector<Point<3>> points;
    map<Point<3>, TileImage*> map;
    for (auto it = theTiles.begin(); it!=theTiles.end(); ++it) {
        LUVAPixel pixel = it->getAverageColor();
        Point<3> point = convertToXYZ(pixel);
        points.push_back(point);
        map[point] = &*it;
    }

    KDTree<3> tree(points);
    for (int i = 0; i<theSource.getRows(); i++) {
        for (int j = 0; j<theSource.getColumns(); j++) {
            LUVAPixel pixel = theSource.getRegionColor(i, j);
            Point<3> point = convertToXYZ(pixel);
            Point<3> nearest = tree.findNearestNeighbor(point);
            TileImage* tile = map[nearest];
            canvas->setTile(i, j, tile);
        }
    }

    return canvas;
}

