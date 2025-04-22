#include "rbtree.h"

int main(int argc, char *argv[]) {
    rbtree *tree = new_rbtree();

    for (size_t i = 0; i < 50; i++)
    {
        rbtree_insert(tree, i);
    }
    
    rbtree_insert(tree, -100);
    print_rbtree_ascii(tree);  

    node_t *min =  rbtree_min(tree);
    printf("min value here: %d \n", min->key);
}   