/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>
#include <deque>

using namespace std;

template <int Dim>
bool smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim)
{
    /**
     * @todo Implement this function!
     */
    if (first[curDim]==second[curDim]) {
      return first < second;
    } else {
      return first[curDim] < second[curDim];
    }

}

template <int Dim>
bool shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential)
{
    /**
     * @todo Implement this function!
     */

    double curr = 0.0;
    double rep = 0.0;
    for (int i = 0; i < Dim; i++) {
      curr+=(target[i]-currentBest[i])*(target[i]-currentBest[i]);
      rep+=(target[i]-potential[i])*(target[i]-potential[i]);
    }
    
    if (curr==rep) {
      return potential < currentBest;
    } else {
      return curr > rep;
    }

}

template <int Dim>
bool shouldReplace_dim(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential, int curDim)
{
    /**
     * @todo Implement this function!
     */
    double curr = 0.0;
    double rep = 0.0;
    for (int i = 0; i < Dim; ++ i) {
      curr+=(target[i]-currentBest[i])*(target[i]-currentBest[i]);
      if (i==curDim) {
        rep+=(target[i]-potential[i])*(target[i]-potential[i]);
      }
    }

    return curr >= rep;
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
    /**
     * @todo Implement this function!
     */

    points = newPoints;
    if(points.empty()==true){
      root = NULL;
      return;
    }
    root = construct_help(0, newPoints.size()-1, 0);
    size = 1;
}

template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim>& other) {
  /**
   * @todo Implement this function!
   */
  root = copy_(root, other.root);
  size = other.size;
}

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
  /**
   * @todo Implement this function!
   */
  delete_(root);
  root = copy_(root, rhs.root);
  size = rhs.size;
  return *this;
}

template <int Dim>
KDTree<Dim>::~KDTree() {
  /**
   * @todo Implement this function!
   */
  delete_(root);
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */
    return find_helper(query, root, 0);
}

template <typename RandIter>
RandIter partition(RandIter start, RandIter end) {
  assert(start!=end);
  auto pivot = end;
  --pivot;
  auto index = start;
  for (auto i = start; i!=pivot; ++i) {
    if (*i<=*pivot) {
      std::swap(*i, *index);
      ++index;
    }
  }
  std::swap(*index, *pivot);
  return index;
}

template <typename RandIter, typename Comparator>
void select(RandIter start, RandIter end, RandIter k, Comparator cmp)
{
    /**
     * @todo Implement this function!
     */    
    auto a = start;
    auto b = end;
    auto index = partition(a, b);
    if (index==k) {
      return;
    }
    if (k<index) { 
      select(a, index--, k, cmp);
    } else {
      select(index++, b, k, cmp);
    }
}

template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::copy_(KDTreeNode* root, KDTreeNode* other) {
  if (other!=NULL) {
    root = new KDTreeNode(other->point);
  } else {
    root = NULL;
    return root;
  }
  if (other->left!=NULL) {
    root->left = copy(root->left, other->left);
  }
  if (other->right!=NULL) {
    root->right = copy(root->right, other->right);
  }
  return root;
}

template <int Dim>
void  KDTree<Dim>::delete_(KDTreeNode* root) {
  if (root==NULL) {
    return;
  }
  if (root->left!=NULL) {
    delete_(root->left);
  }
  if (root->right!=NULL) {
    delete_(root->right);
  }
  delete root;
}

template <int Dim>
Point<Dim> KDTree<Dim>::find_helper(const Point<Dim>& target, KDTreeNode* node, int curDim) const {
  if (node==NULL) {
    double max_point[Dim];
    for (int i = 0; i<Dim; i++) {
      max_point[i] = 10000000000000000;
    }
    Point<Dim> point(max_point);
    return point;
  }
  Point<Dim> point, tmp;
  if (smallerDimVal(target, node->point, curDim) && node->left!=NULL) {
    point = find_helper(target, node->left, (curDim+1)%Dim);
    if (shouldReplace_dim(target, point, node->point, curDim)) {
      tmp = find_helper(target, node->right, (curDim+1)%Dim);
      if (shouldReplace(target, point, node->point)) {
        point = node->point;
      }
      if (shouldReplace(target, point, tmp)) {
        point = tmp;
      }
    }
  } else {
    point = find_helper(target, node->right, (curDim+1)%Dim);
    if (shouldReplace_dim(target, point, node->point, curDim)) {
      tmp = find_helper(target, node->left, (curDim+1)%Dim);
      if (shouldReplace(target, point, node->point)) {
        point = node->point;
      }
      if (shouldReplace(target, point, tmp)) {
        point = tmp;
      }
    }
  }
  return point;
}

template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::construct_help(int start, int end, int curDim) {
  if (start<=end) {
    int median = ((start+end)/2);
    quickselect(start, end, median, curDim);
    KDTreeNode* node = new KDTreeNode(points[median]);
    node->left = construct_help(start, median-1, (curDim+1)%Dim);
    node->right = construct_help(median+1, end, (curDim+1)%Dim);
    return node;
    size++;
  } 
  return NULL;
}

template <int Dim>
int KDTree<Dim>::partition(int start, int end, int k, int curDim) {
  Point<Dim> value = points[k];
  int index = start;
  std::swap(points[k], points[end]);
  for (int i = start; i<end; i++) {
    if (smallerDimVal(points[i], value, curDim)) {
      std::swap(points[index], points[i]);
      index++;
    } 
  }
  std::swap(points[end], points[index]);
  return index;
}

template <int Dim>
void KDTree<Dim>::quickselect(int start, int end, int k, int curDim) {
  while (1) {
    int index = partition(start, end, k, curDim);
    if (index==k) {
      return;
    }
    if (k<index) {
      end = index - 1;
    } else {
      start = index + 1;
    }
  }
}
