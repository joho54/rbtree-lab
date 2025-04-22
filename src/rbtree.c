#include "rbtree.h"

#include <stdlib.h>

#include <stdio.h>
#include <string.h>

#define INDENT_STEP 4

////////////////////////////////////////////////////////////////////////
////////////////// auxilary functions //////////////////////////////////
////////////////////////////////////////////////////////////////////////

static void print_node_ascii(const node_t *node, const node_t *nil, int indent)
{
  if (node == nil)
    return;

  if (node->right != nil)
    print_node_ascii(node->right, nil, indent + INDENT_STEP);

  for (int i = 0; i < indent; i++)
    printf(" ");

  // 노드 출력: [keyR] or [keyB]
  printf("[%d%s]\n", node->key, node->color == RBTREE_RED ? "R" : "B");

  if (node->left != nil)
    print_node_ascii(node->left, nil, indent + INDENT_STEP);
}

void print_rbtree_ascii(const rbtree *t)
{
  printf("\n=== RB Tree ASCII View ===\n");
  if (!t || !t->root || t->root == t->nil)
  {
    printf("[empty]\n");
    return;
  }
  print_node_ascii(t->root, t->nil, 0);
  printf("===========================\n\n");
}

void print_rbtree_ascii(const rbtree *t);

void insert_fixup(rbtree *t, node_t *z)
{
  while (z->parent->color == RBTREE_RED)
  {
    if (z->parent == z->parent->parent->left)
    {
      node_t *y = z->parent->parent->right;
      if (y->color == RBTREE_RED)
      {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
        // case 실행 후 포인터는 제자리여서 루프가 끝남. (z의 부모는 그냥 블랙으로 고정)
      }
      else
      {
        if (z == z->parent->right) // case 2. z.p는 레드, y가 블랙, z는 오른쪽 자식
        {
          z = z->parent;     // 포인터를 올리고,
          left_rotate(t, z); // 왼쪽 회전
        }
        z->parent->color = RBTREE_BLACK; // case 3
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
      }
    }
    else // 위와 똑같은 버전
    {
      node_t *y = z->parent->parent->left;
      if (y->color == RBTREE_RED)
      {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
        // case 실행 후 포인터는 제자리여서 루프가 끝남. (z의 부모는 그냥 블랙으로 고정)
      }
      else
      {
        if (z == z->parent->left)
        {
          z = z->parent;
          right_rotate(t, z); // 왼쪽 회전
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

void left_rotate(rbtree *t, node_t *x)
{
  node_t *y = x->right;
  x->right = y->left;
  if (y->left != t->nil)
  {
    y->left->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == t->nil)
  {
    t->root = y;
  }
  else if (x == x->parent->left)
  {
    x->parent->left = y;
  }
  else
  {
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
}

void right_rotate(rbtree *t, node_t *x)
{
  node_t *y = x->left;
  x->left = y->right;
  if (y->right != t->nil)
  {
    y->right->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == t->nil)
  {
    t->root = y;
  }
  else if (x == x->parent->left)
  {
    x->parent->left = y;
  }
  else
  {
    x->parent->right = y;
  }
  y->right = x;
  x->parent = y;
}

void rb_transplant(rbtree *t, node_t *u, node_t *v)
{
  if (u->parent == t->nil)
    t->root = v;
  else if (u == u->parent->left)
    u->parent->left = v;
  else
    u->parent->right = v;
  v->parent = u->parent;
}

node_t *tree_minimum(rbtree *t, node_t *x)
{
  while (x->left != t->nil)
    x = x->left;
  return x;
}

////////////////////////////////////////////////////////////////////////
////////////////// auxilary functions //////////////////////////////////
////////////////////////////////////////////////////////////////////////

rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  p->nil = malloc(sizeof(node_t));
  p->nil->color = RBTREE_BLACK;
  p->root = p->nil;
  return p;
}

void delete_rbtree(rbtree *t)
{
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  // TODO: implement insert
  node_t *x = t->root; // node being comopared with z
  node_t *y = t->nil;  // y will be parent of z
  node_t *z = malloc(sizeof(node_t));
  z->key = key;
  while (x != t->nil) // descend until reaching the sentinel
  {
    y = x;
    if (z->key < x->key)
      x = x->left;
    else
      x = x->right;
  }
  z->parent = y; // found the location - insert z with parent y

  if (y == t->nil)
    t->root = z; // tree t was empty
  else if (z->key < y->key)
    y->left = z;
  else
    y->right = z;
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;
  insert_fixup(t, z);
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
  // // TODO: implement find
  node_t *cur = t->root;
  while (cur != t->nil)
  {
    if (key == cur->key) return cur;
    else if (key < cur->key) cur = cur->left;
    else cur = cur->right;
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t)
{
  // TODO: implement find
  node_t *cur = t->root;
  node_t *prev = cur;
  while (cur != t->nil)
  {
    prev = cur;
    cur = cur->left;
  }
  
  return prev;
}

node_t *rbtree_max(const rbtree *t)
{
  // TODO: implement find
  return t->root; 
}

int rbtree_erase(rbtree *t, node_t *z)
{
  node_t *y = z;
  color_t y_original_color = y->color;
  node_t *x;
  if (z->left == t->nil)
  {
    x = z->right;
    rb_transplant(t, z, z->right);
  }
  else if (z->right == t->nil)
  {
    x = z->left;
    rb_transplant(t, z, z->left);
  }
  else
  {
    y = tree_minimum(t, z->right);
    y_original_color = y->color;
    x = y->right;
    if (y != z->right)
    { // y가 z의 1세대 자식이 아닐 경우.
      rb_transplant(t, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    else // y가 1세대 자식이라면 x는 그냥 nil임. nil의 부모 포인터를 y로 지정
    {
      x->parent = y;
    }
    rb_transplant(t, z, y);
    // 이까지가 이식 과정.
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }
  if (y_original_color == RBTREE_BLACK)
    delete_fixup(t, x);
  return 0;
}

void delete_fixup(rbtree *t, node_t *x)
{
  while (x != t->root && x->color == RBTREE_BLACK)
  {
    if (x == x->parent->left) // is x a left child? (else, 그냥 대칭 수행하면 됨.)
    {
      node_t *w = x->parent->right; // w is x's sibling
      if (w->color == RBTREE_RED)   // case 1.
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK)
      {
        w->color = RBTREE_RED; // 
        x = x->parent; 
      }
      else
      {
        if (w->right->color == RBTREE_BLACK)
        {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t, w);
          w = x->parent->right;
        }
        w->color = x->parent->color; // case 4;.
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;
      }
    }
    else
    {
      node_t *w = x->parent->left; // w is x's sibling
      if (w->color == RBTREE_RED)   // case 1.
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK)
      {
        w->color = RBTREE_RED; 
        x = x->parent; 
      }
      else
      {
        if (w->left->color == RBTREE_BLACK)
        {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t, w);
          w = x->parent->left;
        }
        w->color = x->parent->color; // case 4
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
    
  }
  x->color = RBTREE_BLACK;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  // array의 메모리 공간은 이 함수를 부르는 쪽에서 준비하고 그 크기를 n으로 알려줍니다.
  // 따로 준비할 필요는 없겠네.
  // TODO: implement to_array
  return 0;
}
