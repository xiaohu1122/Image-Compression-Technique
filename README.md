# image compression technique
## CPSC221-PA3-2019W1
### Goals and Overview

In this PA (Programming Assignment) you will:
learn about algorithmic art and a possible image compression technique.
learn about space partitioning trees called Quad Trees.
learn about balanced versions of Quad Trees.
learn to design more complex recursive algorithms.

### Inspiration and Background
The inspiration for this assignment came from a structure used for surface approximation and an article about an artist whose work recreates classic portraits with a blocky effect. The exact algorithm he used is not given in the article, but we will create similar images using a strategy for image representation that underlies some lossy image compression algorithms.

The two images below illustrate the result of this assignment. Note that the image on the right sacrifices color detail in squares that do not contain much color variability in the original image, but maintains detail by using smaller squares in areas of the original image containing lots of variability.



In specifying the algorithm we make several conventional assumptions, illustrated below:

The origin, position (0,0), is upper-left pixel of the image.
Squares are typically specified either by a pair of points at its upper left and lower right corners, or by an upper left point, together with dimensions (this assignment uses the latter).
The upper left corner of a square is the one nearest to the origin.
Image locations are typically specified as (x,y), where x is a horizontal offset, and y is a vertical offset.

### QuadTrees
A Quad tree is a 4-ary tree whose nodes represent square-shaped regions of an image. The root represents the entire image. A node’s four children (if it has children) represent the four equal-sized squares in the northwest, northeast, southwest, and southeast corners of the node’s square. Every node also contains

a pointer to its parent
a pixel that is the average color of the pixels in its square and the sum of the variance of these pixels from the average.
the upper-left coordinates of its square
the size of the square

### Part 1. Building a QuadTree (60% of PA3)
The constructors for a QTree take an image and a leaf bound as parameters. The QTree starts as a single root node whose square is the largest power-of-two-sized square that fits within the image and whose origin is the upper-left corner of the image.

Splits are performed to decrease the color variability across the leaf squares. The variability measurement we use is the sum of the variance of the pixels in the square. See var.cpp. We split the QTree leaf with the highest variance using a priority queue that holds all the leaves. You may use C++’s priority_queue or write your own. The resulting four new children are leaves that are added to the queue. Splitting continues while the number of leaves is less than the leaf bound.

Specifications for each function you write are contained in the given code. The list of functions here should serve as a checklist for completing this first part of the assignment.

In class QTree:
void clear()
void copy(const toqutree & other)
QTree(PNG & imIn, int leafBound, RGBAPixel frameColor, bool bal=false)
QTree(PNG & imIn, int leafBound, bool bal=false)
bool write(string const & fileName)
void split(Node *t)
bool isLeaf( Node *t )
### Part 2. Building a balanced Quadtree (40% of PA3)
A quad tree is balanced if for every leaf square, the leaf squares that share a side with it have nearly the same size as it, either twice its size or half its size. The quad tree in the figure above is not balanced, but this one is:


By supplying a boolean flag to the QTree constructor, you can specify that you wish to construct a balanced QTree rather than a regular QTree. This requires modifying the split function since splitting a QTree leaf may cause the balance condition to be violated. For example, splitting the small red square in the figure above would force the pink square to its left and the yellow square above to split. Those splits would then cause the dark red square to split. This cascading sequence of splits looks potentially costly, but at most two leaves of any given size in the QuadTree will be forced to split in this sequence.

The way to find which leaves are forced to split is to use the same-sized neighbor functions NNbr(Node *t), SNbr(Node *t), ENbr(Node *t), and WNbr(Node *t) for getting the north, south, east, and west neighbors of node t that are the same size as t in the QTree. If we split the node x, then we need to split any same-sized neighbor of the parent t of x that shares a side with x and is a leaf. In our example of splitting the small red square x, which is the north-west child of its parent t, we need to split NNbr(t) (the yellow node) and WNbr(t) (the pink node) because both share a side with x (since x is the north-west child of its parent) and both are leaves.

To complete this part of the assignment, you should complete (or modify) the following functions:

In class QTree:
QTree(PNG & imIn, int leafBound, RGBAPixel frameColor, bool bal=false)
QTree(PNG & imIn, int leafBound, bool bal=false)
void split(Node *t)
Node *NNbr(Node * t)
Node *SNbr(Node * t)
Node *ENbr(Node * t)
Node *WNbr(Node * t)
Implementation Constraints and Advice
We will be grading your work on functionality, efficiency, and memory use.

Start by implementing the QTree without balance. After this works, you can go on to including the balance option. The code we wrote for the neighbor functions and the split routine are recursive.

We’ve defined the Node class for you, and a few other necessary memory management functions, but everything else is left to you. Since we will be grading both the .h and .cpp files, you are welcome to add helper functions to the class (and you should!).
