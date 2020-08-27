/**
 *
 * Balanced Quad Tree (pa3)
 *
 * This file will be used for grading.
 *
 */

#include "QTree.h"

// Return the biggest power of 2 less than or equal to n
int biggestPow2(int n) {
  if( n < 1 ) return 0;
  int v = 1;
  while( v <= n ) v <<= 1;
  return v >> 1;
}

QTree::Node::Node(PNG & im, pair<int,int> ul, int sz, Node *par)
  :upLeft(ul),size(sz),parent(par),nw(NULL),ne(NULL),sw(NULL),se(NULL)
{
  var = varAndAvg(im,ul,size,avg);
}

QTree::~QTree(){
  clear();
}

QTree::QTree(const QTree & other) {
  copy(other);
}


QTree & QTree::operator=(const QTree & rhs){
  if (this != &rhs) {
    clear();
    copy(rhs);
  }
  return *this;
}


QTree::QTree(PNG & imIn, int leafB, RGBAPixel frameC, bool bal)
  : leafBound(leafB), balanced(bal), drawFrame(true), frameColor(frameC)
{
    /* YOUR CODE HERE */
    im = imIn;
    int widthM = biggestPow2(imIn.width());
    int hgtM = biggestPow2(imIn.height());
    int subSize = 0;
    if(widthM <= hgtM){
      subSize = widthM;
    }else{
      subSize = hgtM;
    }
    Node* n = new Node(imIn, pair<int,int>(0,0), subSize, NULL);
    q.push(n);
    numLeaf = 1;
    Node* maxV = n;

    while(numLeaf < leafBound&& (!q.empty()) && maxV->size >= 2){
      q.pop();
      split(maxV);
      while (!isLeaf(q.top()))
      {
        q.pop();
      }
      maxV = q.top();
  }
    root = n;

}


void QTree::split(Node *t) {
  /* YOUR CODE HERE */

  t->nw = new Node(this->im,t->upLeft, (t->size)/2, t);
  t->ne = new Node(this->im,pair<int,int>((t->upLeft.first + t->size/2), t->upLeft.second), (t->size)/2, t);
  t->sw = new Node(this->im,pair<int,int>(t->upLeft.first, (t->upLeft.second + t->size/2)), (t->size)/2, t);
  t->se = new Node(this->im,pair<int,int>((t->upLeft.first + t->size/2),(t->upLeft.second + t->size/2)), (t->size)/2, t);

  q.push(t->nw);
  q.push(t->ne);
  q.push(t->sw);
  q.push(t->se);

  numLeaf = numLeaf + 3;

  // FOR BALANCED QTREES-------------------------------------------------
  // A split might cause one or two nbrs of the parent of t to split
  // to maintain balance.  Note that these two nbrs exist (unless they're
  // not in the image region) because the current set of leaves are
  // balanced.
  // if( t is a NW (or NE or SW or SE) child ) then we need to check that
  // the North and West (or North and East or South and West or
  // South and East) nbrs of t->parent have children. If they don't
  // we need to split them.
  if(balanced == true){
    if(t->parent != NULL){
      if(t == t->parent->nw){

        Node* np = NNbr(t->parent);
        Node* wp = WNbr(t->parent);
        if(isLeaf(np)){
          split(np);
        }
        if(isLeaf(wp)){
          split(wp);
        }
      }
      else if(t == t->parent->ne){

        Node* np = NNbr(t->parent);
        Node* ep = ENbr(t->parent);
        if(isLeaf(np)){
          split(np);
          np = NULL;
        }
        if(isLeaf(ep)){
          split(ep);

        }
      }
    else if(t == t->parent->se){

        Node* sp = SNbr(t->parent);
        Node* ep = ENbr(t->parent);
          if(isLeaf(sp)){
            split(sp);
          }
          if(isLeaf(ep)){
            split(ep);
          }
        }
    else if(t == t->parent->sw){

        Node* sp = SNbr(t->parent);
        Node* wp = WNbr(t->parent);
        if(isLeaf(sp)){
          split(sp);
        }
        if(isLeaf(wp)){
          split(wp);
        }
      }
    }
  }
}



QTree::QTree(PNG & imIn, int leafB, bool bal)
  : leafBound(leafB), balanced(bal), drawFrame(false)
{
  /* YOUR CODE HERE */
  im = imIn;
  int widthM = biggestPow2(imIn.width());
  int hgtM = biggestPow2(imIn.height());
  int subSize = 0;
  if(widthM <= hgtM){
    subSize = widthM;
  }else{
    subSize = hgtM;
  }

  Node* n = new Node(imIn, pair<int,int>(0,0), subSize, NULL);

  q.push(n);
  numLeaf = 1;
  Node* maxV = n;

  while(numLeaf < leafBound && maxV->size >= 2 && (!q.empty())){
    q.pop();
    split(maxV);
    while (!isLeaf(q.top()))
      {
        q.pop();
      }
    maxV = q.top();
}
  root = n;
}


bool QTree::isLeaf( Node *t ) {
  if (t == NULL) return false;
  return (t->nw == NULL && t->ne == NULL && t->sw == NULL && t->se == NULL);
}




/* NNbr(t)
 * return the same-sized quad tree node that is north of Node t.
 * return NULL if this node is not in the QTree.
 */
QTree::Node * QTree::NNbr(Node *t) {
  /* YOUR CODE HERE */
 Node* n = NULL;
 if(t->parent == NULL){
   return NULL;
 }
 if (t == t->parent->sw){
   n = t->parent->nw;
 }
 else if(t == t->parent->se){
   n = t->parent->ne;
 }
 else if(t == t->parent->nw){
   Node* np = NNbr(t->parent);
   if(np != NULL){
     n = np->sw;
   }else{
     n = NULL;
   }
 }
 else if(t == t->parent->ne){
   Node* np = NNbr(t->parent);
   if(np != NULL){
     n = np->se;
   }else{
     n = NULL;
   }
 }
return n;
}

/* SNbr(t)
 * return the same-sized quad tree node that is south of Node t.
 * return NULL if this node is not in the QTree.
 */
QTree::Node * QTree::SNbr(Node *t) {
  /* YOUR CODE HERE */
  Node* n = NULL;
  if(t->parent == NULL){
    return NULL;
  }
  if (t == t->parent->nw){
    n = t->parent->sw;
  }
  else if(t == t->parent->ne){
    n = t->parent->se;
  }
  else if(t == t->parent->sw){
    Node* np = SNbr(t->parent);
    if(np!=NULL){
      n = np->nw;
    }else{
      n = NULL;
    }
  }
  else if(t == t->parent->se){
    Node* np = SNbr(t->parent);
    if(np!=NULL){
      n = np->ne;
    }else{
      n = NULL;
    }
  }
 return n;
}

/* ENbr(t)
 * return the same-sized quad tree node that is east of Node t.
 * return NULL if this node is not in the QTree.
 */
QTree::Node * QTree::ENbr(Node *t) {
  /* YOUR CODE HERE */
  Node* n = NULL;
  if(t->parent  == NULL){
    return NULL;
  }
  if (t == t->parent->nw){
    n = t->parent->ne;
  }
  else if(t == t->parent->sw){
    n = t->parent->se;
  }
  else if(t == t->parent->ne){
    Node* np = ENbr(t->parent);
    if(np != NULL){
      n = np->nw;
    }else{
      n = NULL;
    }
  }
  else if(t == t->parent->se){
    Node* np = ENbr(t->parent);
    if(np != NULL){
      n = np->sw;
    }else{
      n = NULL;
    }
  }
 return n;
}

/* WNbr(t)
 * return the same-sized quad tree node that is west of Node t.
 * return NULL if this node is not in the QTree.
 */
QTree::Node * QTree::WNbr(Node *t) {
  /* YOUR CODE HERE */
  Node* n = NULL;
  if(t->parent == NULL){
    return NULL;
  }
  if (t == t->parent->ne){
    n = t->parent->nw;
  }
  else if(t == t->parent->se){
    n = t->parent->sw;
  }
  else if(t == t->parent->nw){
    Node* np = WNbr(t->parent);
    if(np != NULL){
      n = np->ne;
    }else{
      n = NULL;
    }
  }
  else if(t == t->parent->sw){
    Node* np = WNbr(t->parent);
    if(np != NULL){
      n = np->se;
    }else{
      n = NULL;
    }
  }
  return n;
}

bool QTree::write(string const & fileName){
  /* YOUR CODE HERE */

  PNG image = PNG(root->size,root->size);
  render(root,image,drawFrame,frameColor);
  im = image;
  // include the following line to write the image to file.
  return(im.writeToFile(fileName));
}



void QTree::render(Node* root, PNG & img, bool drawFrame, RGBAPixel frameColor){
  if(root == NULL) return;
  if(isLeaf(root)){
    int size = root->size;
    if(!drawFrame){
    for(int i = root->upLeft.first; i < root->upLeft.first + size ; i++){
      for(int j = root->upLeft.second; j < root->upLeft.second + size; j++){
        RGBAPixel *p = img.getPixel(i,j);
        *p = root->avg;
     }
    }
   }else{
     for(int i = root->upLeft.first; i < root->upLeft.first + size ; i++){
      for(int j = root->upLeft.second; j < root->upLeft.second + size; j++){
        RGBAPixel *p = img.getPixel(i,j);
        if (i == root->upLeft.first ||
            i == root->upLeft.first + size - 1 ||
            j == root->upLeft.second ||
            j == root->upLeft.second + size - 1)
            {
              *p = frameColor;
            } else {
              *p = root->avg;
            }
     }
    }
   }
 }
  render(root->nw,img,drawFrame,frameColor);
  render(root->ne,img,drawFrame,frameColor);
  render(root->sw,img,drawFrame,frameColor);
  render(root->se,img,drawFrame,frameColor);
}

void QTree::clear() {
  /* YOUR CODE HERE */
  clear(root);
}
void QTree::clear(Node* root){
  if(root != NULL){
    clear(root->nw);
    clear(root->ne);
    clear(root->sw);
    clear(root->se);
    delete root;
    root = NULL;
  }
}



void QTree::copy(const QTree & orig) {
  /* YOUR CODE HERE */
  numLeaf = orig.numLeaf;
  im = orig.im;
  leafBound = orig.leafBound;
  balanced = orig.balanced;
  drawFrame = orig.drawFrame;
  frameColor = orig.frameColor;
  q = orig.q;
  root = copy(orig.root,im);
}

QTree::Node* QTree::copy(const Node* root, PNG im){
    Node* t = NULL;
    if(root != NULL){
      t = new Node(im,root->upLeft,root->size,root->parent);
      t->nw = copy(root->nw,im);
      t->ne = copy(root->ne,im);
      t->sw = copy(root->sw,im);
      t->se = copy(root->se,im);
  }
  return t;
}
