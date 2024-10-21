
#include "cs225/PNG.h"
#include "FloodFilledImage.h"
#include "Animation.h"

#include "ImageTraversal.h"

#include "colorPicker/RainbowColorPicker.h"
#include "colorPicker/GradientColorPicker.h"
#include "colorPicker/GridColorPicker.h"
#include "colorPicker/SolidColorPicker.h"
#include "colorPicker/MyColorPicker.h"

using namespace cs225;

int main() {

  // @todo [Part 3]
  // - The code below assumes you have an Animation called `animation`
  // - The code provided below produces the `myFloodFill.png` file you must
  //   submit Part 3 of this assignment -- uncomment it when you're ready.
  
  PNG png;       png.readFromFile("../data/i.png");
  FloodFilledImage image(png);
  Traversals::ImageTraversal dfs(png, Point(40,40), 0.05, Traversals::dfs_add, Traversals::dfs_pop);
  MyColorPicker myColor;    
  image.addFloodFill( dfs, myColor );
  Animation animation = image.animate(1000);


  PNG lastFrame = animation.getFrame( animation.frameCount() - 1 );
  lastFrame.writeToFile("myFloodFill.png");
  animation.write("myFloodFill.gif");
  


  return 0;
}
