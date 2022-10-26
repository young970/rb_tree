#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree* p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *NIL = (node_t *)calloc(1, sizeof(node_t));
  NIL->color = RBTREE_BLACK;  // 초기값 (property 3)
  p->nil = NIL;   // 초기값
  p->root = NIL;  // 초기값
  return p;
}
void delete_node_t(rbtree *t, node_t *p){
  if (p != t->nil){
  delete_node_t(t, p ->left);
  delete_node_t(t, p ->right);
  free(p);
  }
  p = NULL;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  if (t ->root != t->nil)
    delete_node_t(t, t->root);
  free(t->nil);
  free(t);
}

static void left_rotate(rbtree *t, node_t* x){
  node_t* y = x->right;
  x->right = y->left;
  if (y->left != t->nil)
    y->left->parent = x;
  y->parent = x->parent;
  if (x->parent == t->nil)
    t->root = y;
  else if (x== x->parent->left)
    x->parent->left = y;
  else 
    x->parent->right = y;
  y->left = x;
  x->parent = y;
}

static void right_rotate(rbtree *t, node_t* x){
  node_t* y = x->left;
  x->left = y->right;
  if (y->right != t->nil)
    y->right->parent = x;
  y->parent = x->parent;
  if (x->parent == t->nil)
    t->root = y;
  else if (x== x->parent->right)
    x->parent->right = y;
  else 
    x->parent->left = y;
  y->right = x;
  x->parent = y;
}

void rb_insert_fixup(rbtree* t, node_t* n){
  node_t* y;
  while (n->parent->color ==RBTREE_RED )
  {
    if (n->parent == n->parent->parent->left){
      y = n->parent->parent->right;
      if (y->color == RBTREE_RED){
        n->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        n->parent->parent->color = RBTREE_RED;
        n = n->parent->parent;
      }
      else{
        if (n ==n->parent->right){
          n = n->parent;
          left_rotate(t, n);
        }
        n->parent->color = RBTREE_BLACK;
        n->parent->parent->color = RBTREE_RED;
        right_rotate(t, n->parent->parent);
      }
    }
    else{
      y = n->parent->parent->left;
      if (y->color == RBTREE_RED){
        n->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        n->parent->parent->color = RBTREE_RED;
        n = n->parent->parent;
      }
      else{
        if (n ==n->parent->left){
          n = n->parent;
          right_rotate(t, n);
        }
        n->parent->color = RBTREE_BLACK;
        n->parent->parent->color = RBTREE_RED;
        left_rotate(t, n->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t* y = t->nil;
  node_t* x = t->root;
  node_t* n = (node_t*)calloc(1, sizeof(node_t));
  while (x != t->nil)
  {
    y = x;
    if (key < x->key)
      x = x->left;
    else
      x = x->right;
  }
  n->parent = y;
  n->key = key;
  if (y == t->nil)
    t->root = n;
  else if (n->key < y->key)
    y->left = n;
  else 
    y->right = n;
  n->left = t->nil;
  n->right = t->nil;
  n ->color = RBTREE_RED;
  rb_insert_fixup(t, n);

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t* p = t->root;
  node_t* NIL = t->nil;
  while(p != NIL){
    if (p->key == key)
      return p;
    if (p->key < key)
      p = p->right;
    else
      p = p->left;
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t* p = t->root;
  node_t* NIL = t->nil;
  while(p->left != NIL){
    p = p->left;
  }
  return p;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t* p = t->root;
  node_t* NIL = t->nil;
  while(p->right != NIL){
    p = p->right;
  }
  return p;
}

void RB_Transplant(rbtree *t, node_t *u, node_t *v) {
  if (u->parent == t->nil)
    t->root = v;
  else if (u == u->parent->left)
    u->parent->left = v;
  else 
    u->parent->right = v;
  v->parent = u->parent;
}

node_t *dtree_min(const rbtree *t, node_t *n) {
  // TODO: implement find
  node_t* NIL = t->nil;
  while(n->left != NIL){
    n = n->left;
  }
  return n;
}

void rb_delete_fixup(rbtree* t, node_t* x) {
  node_t* w;
  while ((x != t->root) && (x->color == RBTREE_BLACK)) {
    if (x == x->parent->left){
      w = x->parent->right;
      if (w->color == RBTREE_RED) {
        w ->color = RBTREE_BLACK;
        x ->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }
      if ((w->left->color == RBTREE_BLACK) && (w->right->color == RBTREE_BLACK))
      {
        w ->color = RBTREE_RED;
        x = x->parent;
      }
      else
        {
          if (w->right->color == RBTREE_BLACK) {
            w ->left ->color = RBTREE_BLACK;
            w-> color = RBTREE_RED;
            right_rotate(t, w);
            w = x->parent->right;
          }
          w->color = x->parent->color;
          x->parent->color = RBTREE_BLACK;
          w->right->color = RBTREE_BLACK;
          left_rotate(t, x->parent);
          x = t->root;
        }
    }
    else {
      w = x->parent->left;
      if (w->color == RBTREE_RED) {
        w ->color = RBTREE_BLACK;
        x ->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }
      if ((w->right->color == RBTREE_BLACK) && (w->left->color == RBTREE_BLACK))
      {
        w ->color = RBTREE_RED;
        x = x->parent;
      }
      else
        {
          if (w->left->color == RBTREE_BLACK) {
            w ->right ->color = RBTREE_BLACK;
            w-> color = RBTREE_RED;
            left_rotate(t, w);
            w = x->parent->left;
          }
          w->color = x->parent->color;
          x->parent->color = RBTREE_BLACK;
          w->left->color = RBTREE_BLACK;
          right_rotate(t, x->parent);
          x = t->root;
        }
    }
  }
    x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  node_t *x;
  node_t *y = p;
  color_t y_c = y->color;
  if (p->left == t->nil){
    x = p->right;
    RB_Transplant(t, p, p->right);
  }
  else if (p->right == t->nil){
    x = p->left;
    RB_Transplant(t, p, p->left);
  }
  else{
    y = dtree_min(t, p->right);
    y_c = y->color;
    x = y->right;
    if (y->parent == p)
      x->parent = y;
    else{
      RB_Transplant(t, y, y->right);
      y->right = p->right;
      y->right->parent = y;
    }
    RB_Transplant(t, p, y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }
  if (y_c == RBTREE_BLACK)
    rb_delete_fixup(t, x);
  free(p);
  return 0;
}
int inorder(const rbtree *t,node_t* p, key_t *arr, const size_t n, int i){
  if (p == t->nil || i >= n)
    return i;
  i = inorder(t, p->left, arr, n, i);
  arr[i++] = p ->key;
  i = inorder(t, p->right, arr, n, i);
  return i;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  int i = 0;
  inorder(t, t->root, arr, n, i);
  return 0;
}
