/* Your code here! */
#include "dsets.h"

void DisjointSets::addelements(int num) {
    // Add nodes to list num number of times
    for (int i = 0; i<num; i++) {
        nodes_.push_back(-1);
    }
}

int DisjointSets::find(int elem) {
    int node = nodes_[elem];

    // Check if element is root or not (negative value or not)
    if (node<=-1) {
        return elem;
    }

    // If not negative value, so through vector until you get to the root
    while (0<=nodes_[node]) {
        node = nodes_[node];
    }

    // Update the elem spot with the root node
    nodes_[elem] = node;
    return node;
}

void DisjointSets::setunion(int a, int b) {
    // Get the values of a and b
    int nodea = find(a);
    int nodeb = find(b);

    // Edge case if equal
    if (nodea==nodeb) return;

    // Compare their respective sizes and update accordingly to MP specifications
    int size = nodes_[nodea] + nodes_[nodeb];
    if (nodes_[nodea]<=nodes_[nodeb]) {
        nodes_[nodea] = size;
        nodes_[nodeb] = nodea;
    } else {
        nodes_[nodeb] = size;
        nodes_[nodea] = nodeb; 
    }
}

int DisjointSets::size(int elem) {
    // Find element in vector and return its value (positive size)
    int size = find(elem);
    return -1*nodes_[size];
}
