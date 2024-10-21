/**
 * @file list.cpp
 * Doubly Linked List (MP 3).
 */

#include <iostream>

template <class T>
List<T>::List() { 
  // @TODO: graded in mp_lists part 1
    head_ = NULL;
    tail_ = NULL;
    length_ = 0;
}

/**
 * Returns a ListIterator with a position at the beginning of
 * the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::begin() const {
  // @TODO: graded in mp_lists part 1
  return List<T>::ListIterator(head_);
}

/**
 * Returns a ListIterator one past the end of the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::end() const {
  // @TODO: graded in mp_lists part 1
  return List<T>::ListIterator(tail_->next);
}


/**
 * Destroys all dynamically allocated memory associated with the current
 * List class.
 */
template <typename T>
void List<T>::_destroy() {
  /// @todo Graded in mp_lists part 1
  while (head_!=NULL) {
    ListNode *tmp = head_->next;
    delete head_;
    head_ = tmp;
  }

  head_ = NULL;
  tail_ = NULL;
  length_ = 0;
}

/**
 * Inserts a new node at the front of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertFront(T const & ndata) {
  /// @todo Graded in mp_lists part 1
  ListNode * newNode = new ListNode(ndata);
  newNode -> next = head_;
  newNode -> prev = NULL;
  
  if (head_ != NULL) {
    head_ -> prev = newNode;
  }
  if (tail_ == NULL) {
    tail_ = newNode;
  }

  head_ = newNode;

  length_++;

}

/**
 * Inserts a new node at the back of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertBack(const T & ndata) {
  /// @todo Graded in mp_lists part 1
  ListNode * newNode = new ListNode(ndata);
  newNode -> next = NULL;
  newNode -> prev = tail_;
  
  if (tail_ != NULL) {
    tail_ -> next = newNode;
  }
  if (head_ == NULL) {
    head_ = newNode;
  }

  tail_ = newNode;

  length_++;

}

/**
 * Helper function to split a sequence of linked memory at the node
 * splitPoint steps **after** start. In other words, it should disconnect
 * the sequence of linked memory after the given number of nodes, and
 * return a pointer to the starting node of the new sequence of linked
 * memory.
 *
 * This function **SHOULD NOT** create **ANY** new List or ListNode objects!
 *
 * This function is also called by the public split() function located in
 * List-given.hpp
 *
 * @param start The node to start from.
 * @param splitPoint The number of steps to walk before splitting.
 * @return The starting node of the sequence that was split off.
 */
template <typename T>
typename List<T>::ListNode * List<T>::split(ListNode * start, int splitPoint) {
  /// @todo Graded in mp_lists part 1
  ListNode * curr = start;

  if (splitPoint<=0 || splitPoint>length_) {
    return head_;
  }

  for (int i = 0; i < splitPoint; i++) {
    curr = curr->next;
  }

  curr->prev->next = NULL;
  curr->prev = NULL;

  return curr;

}

/**
 * Modifies the List using the waterfall algorithm.
 * Every other node (starting from the second one) is removed from the
 * List, but appended at the back, becoming the new tail. This continues
 * until the next thing to be removed is either the tail (**not necessarily
 * the original tail!**) or NULL.  You may **NOT** allocate new ListNodes.
 * Note that since the tail should be continuously updated, some nodes will
 * be moved more than once.
 */
template <typename T>
void List<T>::waterfall() {
  /// @todo Graded in part 1
  int index = 0;
  ListNode *curr = head_;
  ListNode *tmp = curr;
  while (tmp!=tail_) {
    if (index%2!=0) {
      tmp = curr->prev;
      curr->prev->next = curr->next;
      curr->next->prev = curr->prev;
      tail_->next = curr;
      curr->prev = tail_;
      curr->next = NULL;
      tail_ = curr;
      index--;
    } else {
      index++;
    }
    tmp = tmp->next;
    curr = tmp;
  }
}


/**
 * Reverses the current List.
 */
template <typename T>
void List<T>::reverse() {
  reverse(head_, tail_);
}

/**
 * Helper function to reverse a sequence of linked memory inside a List,
 * starting at startPoint and ending at endPoint. You are responsible for
 * updating startPoint and endPoint to point to the new starting and ending
 * points of the rearranged sequence of linked memory in question.
 *
 * @param startPoint A pointer reference to the first node in the sequence
 *  to be reversed.
 * @param endPoint A pointer reference to the last node in the sequence to
 *  be reversed.
 */
template <typename T>
void List<T>::reverse(ListNode *& startPoint, ListNode *& endPoint) {
  /// @todo Graded in mp_lists part 2
  if (startPoint==endPoint || startPoint==NULL || endPoint==NULL) {
    return;
  }

  ListNode* start = startPoint->prev;
  ListNode* curr = startPoint;
  ListNode* end = endPoint->next;
  while (curr!=endPoint) {
    ListNode* tmp = curr->next;
    curr->next = curr->prev;
    curr->prev = tmp;
    curr = tmp;
  }

  startPoint->next = endPoint->next;
  endPoint->next = endPoint->prev;
  endPoint->prev = start;

  ListNode* tmp = endPoint;
  endPoint = startPoint;
  startPoint = tmp;

  if (start!=NULL) {
    start->next = startPoint;
  }

  if (end!=NULL) {
    end->prev = endPoint;
  }
}

/**
 * Reverses blocks of size n in the current List. You should use your
 * reverse( ListNode * &, ListNode * & ) helper function in this method!
 *
 * @param n The size of the blocks in the List to be reversed.
 */
template <typename T>
void List<T>::reverseNth(int n) {
  /// @todo Graded in mp_lists part 2
  int groups = length_/n;
  ListNode* start = head_;
  ListNode* end = head_;

  while (groups>0) {
    for (int j = 0; j<(n-1); j++) {
      end = end->next;
    }

    if (start==head_) {
      reverse(head_, end);
    } else if (end==tail_) {
      reverse(start, tail_);
    } else {
      reverse(start, end);
    }

    start = end->next;
    end = end->next;
    groups--;
  } 

  if ((length_%n)!=0) {
    reverse(start, tail_);
  }
}


/**
 * Merges the given sorted list into the current sorted list.
 *
 * @param otherList List to be merged into the current list.
 */
template <typename T>
void List<T>::mergeWith(List<T> & otherList) {
    // set up the current list
    head_ = merge(head_, otherList.head_);
    tail_ = head_;

    // make sure there is a node in the new list
    if (tail_ != NULL) {
        while (tail_->next != NULL)
            tail_ = tail_->next;
    }
    length_ = length_ + otherList.length_;

    // empty out the parameter list
    otherList.head_ = NULL;
    otherList.tail_ = NULL;
    otherList.length_ = 0;
}

/**
 * Helper function to merge two **sorted** and **independent** sequences of
 * linked memory. The result should be a single sequence that is itself
 * sorted.
 *
 * This function **SHOULD NOT** create **ANY** new List objects.
 *
 * @param first The starting node of the first sequence.
 * @param second The starting node of the second sequence.
 * @return The starting node of the resulting, sorted sequence.
 */
template <typename T>
typename List<T>::ListNode * List<T>::merge(ListNode * first, ListNode* second) {
  /// @todo Graded in mp_lists part 2
  ListNode *curr, *start;

  if (first==NULL) {
    return second;
  } else if (second==NULL) {
    return first;
  }

  if (first->data<second->data) {
    curr = first;
    start = first;
    first = first->next;
  } else {
    curr = second;
    start = second;
    second = second->next;
  }

  while (first!=NULL && second!=NULL) {
    if (first->data < second->data) {
      curr->next = first;
      first->prev = curr;
      first = first->next;
      curr = curr->next;
    } else {
      curr->next = second;
      second->prev = curr;
      second = second->next;
      curr = curr->next;
    }
  }

  if (first!=NULL) {
    curr->next = first;
    first->prev = curr;
  }

  if (second!=nullptr) {
    curr->next = second;
    second->prev = curr;
  }

  return start;
}

/**
 * Sorts a chain of linked memory given a start node and a size.
 * This is the recursive helper for the Mergesort algorithm (i.e., this is
 * the divide-and-conquer step).
 *
 * Called by the public sort function in List-given.hpp
 *
 * @param start Starting point of the chain.
 * @param chainLength Size of the chain to be sorted.
 * @return A pointer to the beginning of the now sorted chain.
 */
template <typename T>
typename List<T>::ListNode* List<T>::mergesort(ListNode * start, int chainLength) {
  /// @todo Graded in mp_lists part 2
  if (chainLength==0 || chainLength==1) {
    return start;
  }
  return merge(mergesort(split(start, chainLength-chainLength/2), chainLength/2), mergesort(start, chainLength-chainLength/2));
}
