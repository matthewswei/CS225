/* Your code here! */
#include "maze.h"

SquareMaze::SquareMaze() {

}

void SquareMaze::makeMaze(int width, int height) {
    // Private member initalization
    width_ = width;
    height_ = height;
    set_.addelements(width_*height_);
    walls_ = std::vector<std::vector<std::pair<bool, bool>>> (width_, std::vector<std::pair<bool, bool>>(height_, {true, true}));

    // Generate wall
    while (set_.size(0)!=width_*height_) {
        int idx1 = rand() % (width_*height_-1);
        int x1 = idx1%width_, y1 = idx1/height_;
        int x2 = x1, y2 = y1;

        // Decide right or bottom wall
        int dir = rand()%2;
        if (dir==0) x2++; 
        else y2++;

        // Check for bounds
        int idx2 = y2*width_ + x2;
        if (x2>=width_ || y2>=height_) continue;
        if (set_.find(idx1)==set_.find(idx2)) continue;
        
        // Set walls
        if (dir==0) walls_[x1][y1].first = false;
        else walls_[x1][y1].second = false;
        set_.setunion(idx1, idx2);
    }
}

bool SquareMaze::canTravel(int x, int y, int dir) const {
    // Checks direction and if valid, return true
    if (dir==0) {
        return !walls_[x][y].first;
    } else if (dir==1) {
        return !walls_[x][y].second;
    } else if (dir==2 && x>0) {
        return !walls_[x-1][y].first;
    } else if (dir==3 && y>0) {
        return !walls_[x][y-1].second;
    }
    return false;
}

void SquareMaze::setWall(int x, int y, int dir, bool exists) {
    // Checks direction and if valid, sets boolean accordingly
    if (dir==0) {
        walls_[x][y].first = exists;
    } else {
        walls_[x][y].second = exists;
    }
}

std::vector<int> SquareMaze::solveMaze() {
    std::queue<std::pair<int, int>> q;
    std::vector<std::vector<int>> visited(width_, std::vector<int>(height_, -1));
    std::vector<std::vector<int>> prev = std::vector<std::vector<int>>(width_, std::vector<int>(height_, -1));
    std::pair<int, int> sol_path = {0, height_-1};
    std::vector<int> solution;
    q.push({0, 0});
    visited[0][0] = 0;

    // BFS implementation
    while (!q.empty()) {
        std::pair<int, int> point = q.front(); 
        q.pop();
        int x = point.first, y = point.second;
        for (int i = 0; i<4; i++) {
            int next_x = x, next_y = y;
            switch (i) {
                case 0:
                    next_x++;
                    break;
                case 1:
                    next_y++;
                    break;
                case 2:
                    next_x--;
                    break;
                case 3:
                    next_y--;
                    break;
                default:
                    break;
            }
            if (canTravel(x, y, i)==0 || visited[next_x][next_y]!=-1) {
                continue;
            }
            q.push({next_x, next_y});
            visited[next_x][next_y] = visited[x][y] + 1;
            prev[next_x][next_y] = i;
        }
    }

    // Find best path
    int check = 0;
    for (int i = 0; i<width_; i++) {
        if (check<visited[i][height_-1]) {
            sol_path = {i, height_-1};
            check = visited[i][height_-1];
        }
    }
    
    // Fill in solution vector to return
    while (sol_path.first!=0 || sol_path.second!=0) {
        int x = sol_path.first, y = sol_path.second;
        solution.insert(solution.begin(), prev[x][y]);
        switch (solution[0]) {
            case 0:
                sol_path = {x - 1, y};
                break;
            case 1:
                sol_path = {x, y - 1};
                break;
            case 2:
                sol_path = {x + 1, y};
                break;
            case 3:
                sol_path = {x, y + 1};
                break;
            default:
                break;
        }
    }
    return solution;
}

cs225::PNG* SquareMaze::drawMaze() const {
    cs225::PNG* png = new cs225::PNG(width_*10 + 1, height_*10 + 1);
    /*
        From Doxygen:
        First, create a new PNG. Set the dimensions of the PNG to (width*10+1,height*10+1). 
        where height and width were the arguments to makeMaze. Blacken the entire topmost 
        row and leftmost column of pixels, except the entrance (1,0) through (9,0). For 
        each square in the maze, call its maze coordinates (x,y). If the right wall exists,
        then blacken the pixels with coordinates ((x+1)*10,y*10+k) for k from 0 to 10. If 
        the bottom wall exists, then blacken the pixels with coordinates (x*10+k, (y+1)*10) 
        for k from 0 to 10.
    */

   // Blacken left-most and top-most column/row
    for (int i = 10; i<width_*10 + 1; i++) {
        png->getPixel(i, 0) = cs225::HSLAPixel(0, 0, 0);
    }
    for (int i = 0; i<height_*10 + 1; i++) {
        png->getPixel(0, i) = cs225::HSLAPixel(0, 0, 0); 
    }
    
    // Check for walls
    for (int i = 0; i<width_; i++) {
        for (int j = 0; j<height_; j++) {
            // Right wall check
            if (canTravel(i, j, 0)==0) {
                for (int k = 0; k<11; k++) {
                    png->getPixel((i+1)*10, j*10 + k) = cs225::HSLAPixel(0, 0, 0);
                }
            }
            // Bottom wall check
            if (canTravel(i, j, 1)==0) {
                for (int k = 0; k<11; k++) {
                    png->getPixel(i*10 + k, (j+1)*10) = cs225::HSLAPixel(0, 0, 0);
                }
            }
        }
    }
    return png;
}

cs225::PNG* SquareMaze::drawMazeWithSolution() {
    cs225::PNG* png = drawMaze();
    std::vector<int> solution = solveMaze();
    /*
        From Doxygen:
        This function calls drawMaze, then solveMaze; it modifies the PNG from drawMaze 
        to show the solution vector and the exit.

        Start at pixel (5,5). Each direction in the solution vector corresponds to a trail
        of 11 red pixels in the given direction. If the first step is downward, color pixels
        (5,5) through (5,15) red. (Red is 0,1,0.5,1 in HSLA). Then if the second step is right,
        color pixels (5,15) through (15,15) red. Then if the third step is up, color pixels 
        (15,15) through (15,5) red. Continue in this manner until you get to the end of the 
        solution vector, so that your output looks analogous the above picture.

        Make the exit by undoing the bottom wall of the destination square: call the destination
        maze coordinates (x,y), and whiten the pixels with coordinates (x*10+k, (y+1)*10) for k 
        from 1 to 9.
    */

    int x = 5, y = 5;

    // Fill in solution path (traverse through maze depending on solution direction, then fill pixel in)
    png->getPixel(x, y) = cs225::HSLAPixel(0, 1, 0.5, 1);
    for (int i = 0; i<(int)solution.size(); i++) {
        for (int k = 0; k<10; k++) {
            switch (solution[i]) {
                case 0:
                    x++;
                    break;
                case 1:
                    y++;
                    break;
                case 2:
                    x--;
                    break;
                case 3:
                    y--;
                    break;
                default:
                    break;
            }
            png->getPixel(x, y) = cs225::HSLAPixel(0, 1, 0.5, 1);
        }
    }

    // Make the exit by deleting bottom wall
    for (int k = 1; k<10; k++) {
        png->getPixel((x-5)+k, y+5) = cs225::HSLAPixel(0, 0, 1);
    }
    return png;
}
