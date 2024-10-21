#include <cmath>
#include <iterator>
#include <iostream>

#include "cs225/HSLAPixel.h"
#include "cs225/PNG.h"
#include "Point.h"

#include "ImageTraversal.h"

namespace Traversals {
  /**
  * Calculates a metric for the difference between two pixels, used to
  * calculate if a pixel is within a tolerance.
  *
  * @param p1 First pixel
  * @param p2 Second pixel
  * @return the difference between two HSLAPixels
  */
  double calculateDelta(const HSLAPixel & p1, const HSLAPixel & p2) {
    double h = fabs(p1.h - p2.h);
    double s = p1.s - p2.s;
    double l = p1.l - p2.l;

    // Handle the case where we found the bigger angle between two hues:
    if (h > 180) { h = 360 - h; }
    h /= 360;

    return sqrt( (h*h) + (s*s) + (l*l) );
  }
  
  /**
  * Adds a Point for the bfs traversal to visit at some point in the future.
  * @param work_list the structure which stores the list of points which need to be visited by the traversal
  * @param point the point to be added
  */
  void bfs_add(std::deque<Point> & work_list, const Point & point) {
    /** @todo [Part 1] */
    work_list.push_back(point);
  }

  /**
  * Adds a Point for the dfs traversal to visit at some point in the future.
  * @param work_list the structure which stores the list of points which need to be visited by the traversal
  * @param point the point to be added
  */
  void dfs_add(std::deque<Point> & work_list, const Point & point) {
    /** @todo [Part 1] */
    work_list.push_front(point);
  }

  /**
  * Removes and returns the current Point in the bfs traversal
  * @param work_list the structure which stores the list of points which need to be visited by the traversal
  */
  Point bfs_pop(std::deque<Point> & work_list) {
    /** @todo [Part 1] */
    Point out = work_list.front();
    work_list.pop_front();
    return out;
  }

  /**
  * Removes and returns the current Point in the dfs traversal
  * @param work_list the structure which stores the list of points which need to be visited by the traversal
  */
  Point dfs_pop(std::deque<Point> & work_list) {
    /** @todo [Part 1] */
    Point out = work_list.front();
    work_list.pop_front();
    return out;
  }

  /**
  * Initializes a ImageTraversal on a given `png` image,
  * starting at `start`, and with a given `tolerance`.
  * @param png The image this traversal is going to traverse
  * @param start The start point of this traversal
  * @param tolerance If the current point is too different (difference larger than tolerance) with the start point,
  * it will not be included in this traversal
  * @param traversal_add a function pointer to an implemented function which takes in a reference to the work list
  * and the point to be added
  * @param traversal_pop a function pointer to an implemented function which takes in a reference to the work list
  * and returns the next point to be processed in the traversal
  */
  ImageTraversal::ImageTraversal(const PNG & png, const Point & start, double tolerance, add_function traversal_add, pop_function traversal_pop) {  
    /** @todo [Part 1] */
    png_ = png;
    start_ = start;
    tolerance_ = tolerance;
    add_ = traversal_add;
    pop_ = traversal_pop;
  }

  /**
  * Returns an iterator for the traversal starting at the first point.
  */
  ImageTraversal::Iterator ImageTraversal::begin() {
    /** @todo [Part 1] */
    return ImageTraversal::Iterator(this);
  }

  /**
  * Returns an iterator for the traversal one past the end of the traversal.
  */
  ImageTraversal::Iterator ImageTraversal::end() {
    /** @todo [Part 1] */
    return ImageTraversal::Iterator();
  }

  /**
  * Default iterator constructor.
  */
  ImageTraversal::Iterator::Iterator() {
    /** @todo [Part 1] */
    end = true;
  }

  ImageTraversal::Iterator::Iterator(ImageTraversal* image) {
    // Copy over travaersal data to iterator for use
    start = image->start_;
    curr = image->start_;
    tolerance = image->tolerance_;
    png = image->png_;
    add = image->add_;
    pop = image->pop_;
    add(work_list, start);
    end = false;
    visited.resize(png.width());
    for(unsigned i = 0; i<visited.size(); i++) {
      visited[i].resize(png.height());
      for (unsigned j = 0; j<visited[i].size(); j++) {
        visited[i][j] = false;
      }
    }
  }


  /**
  * Iterator increment opreator.
  *
  * Advances the traversal of the image.
  */
  ImageTraversal::Iterator & ImageTraversal::Iterator::operator++() {
    /** @todo [Part 1] */
    // Check if iterator is at the end
    if (end) {
      return *this;
    }

    // Initialize varialbes for function
    curr = pop(work_list);
    visited[curr.x][curr.y] = true;
    Point right(curr.x+1, curr.y);
    Point bottom(curr.x, curr.y+1);
    Point left(curr.x-1, curr.y);
    Point top(curr.x, curr.y-1);

    // Check neighbors in corresponding order
    if (valid(right)) {
      if (!visited[right.x][right.y]) {
        add(work_list, right);
      }
    }
    if (valid(bottom)) {
      if (!visited[bottom.x][bottom.y]) {
        add(work_list, bottom);
      }
    }
    if (valid(left)) {
      if (!visited[left.x][left.y]) {
        add(work_list, left);
      }
    }
    if (valid(top)) {
      if (!visited[top.x][top.y]) {
        add(work_list, top);
      }
    }

    // Get rid of visited points
    while (work_list.size() && visited[work_list.front().x][work_list.front().y]) {
      pop(work_list);
    }

    if (work_list.size()==0) {
      end = true;
      curr = Point();
      return *this;
    }

    // Get correct point
    curr = work_list.front();
    return *this;
  }

  /**
  * Iterator accessor opreator.
  *
  * Accesses the current Point in the ImageTraversal.
  */
  Point ImageTraversal::Iterator::operator*() {
    /** @todo [Part 1] */
    return curr;
  }

  /**
  * Iterator inequality operator.
  *
  * Determines if two iterators are not equal.
  */
  bool ImageTraversal::Iterator::operator!=(const ImageTraversal::Iterator &other) {
    /** @todo [Part 1] */
    if (end!=other.end) {
      return true;
    }
    return false;
  }

  bool ImageTraversal::Iterator::valid(Point point) {
    // Check if point is within range of image
    if (point.x<0 || point.x>=png.width() || point.y<0 || point.y>=png.height()) {
      return false;
    } 

    // Calculate delta and see if its within the tolerance
    double delta = calculateDelta(png.getPixel(start.x, start.y), png.getPixel(point.x, point.y));
    if (delta>=tolerance) {
      return false;
    }
    return true;
  }
}