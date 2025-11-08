#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include "bt_utils.h"

static BTNode *find_val(BTNode *root, int val) {
    BTNode *cur = root;
    while (cur) {
        if (val == cur->data) return cur;
        cur = (val < cur->data) ? cur->left : cur->right;
    }
    return NULL;
}

static int is_bst(BTNode *n, int lo, int hi) {
    if (!n) return 1;
    if (!(lo <= n->data && n->data < hi)) return 0;
    return is_bst(n->left, lo, n->data) && is_bst(n->right, n->data, hi);
}

static int size(BTNode *n) {
    if (!n) return 0;
    return 1 + size(n->left) + size(n->right);
}

static void inorder_fill(BTNode *n, int *out, int *i) {
    if (!n) return;
    inorder_fill(n->left, out, i);
    out[(*i)++] = n->data;
    inorder_fill(n->right, out, i);
}

static int nondecreasing(const int *a, int n) {
    for (int i = 1; i < n; ++i)
        if (a[i - 1] > a[i]) return 0;
    return 1;
}

static void test_insert_basic_shape(void) {
    BTNode *root = NULL;
    int vals[] = {8, 3, 10, 1, 6, 14, 4, 7, 13};
    for (int i = 0; i < 9; ++i) insert(&root, vals[i]);
    assert(root && root->data == 8);
    assert(root->left && root->left->data == 3);
    assert(root->right && root->right->data == 10);
    assert(size(root) == 9);

    insert(&root, 6);
    BTNode *n6 = find_val(root, 6);
    assert(n6 && n6->right && n6->right->data == 6);

    BTNode *n7 = find_val(root, 7);
    assert(n7 && n7->p && n7->p->data == 6);
    assert(is_bst(root, INT_MIN, INT_MAX));

    release_tree(root);
}

static void test_inorder_sorted(void) {
    BTNode *root = NULL;
    int vals[] = {50, 20, 70, 10, 30, 60, 80, 30, 20, 75};
    for (int i = 0; i < 10; ++i) insert(&root, vals[i]);
    int arr[64], k = 0;
    inorder_fill(root, arr, &k);
    assert(k == size(root));
    assert(nondecreasing(arr, k));

    int c20 = 0, c30 = 0;
    for (int i = 0; i < k; ++i) {
        if (arr[i] == 20) ++c20;
        if (arr[i] == 30) ++c30;
    }
    assert(c20 == 2 && c30 == 2);
    assert(is_bst(root, INT_MIN, INT_MAX));
    release_tree(root);
}

static void test_minimum_and_successor(void) {
    BTNode *root = NULL;
    int vals[] = {15, 6, 3, 2, 4, 7, 13, 9, 18, 17, 20};
    for (int i = 0; i < 11; ++i) insert(&root, vals[i]);
    BTNode *mn = minimum(root);
    assert(mn && mn->data == 2);

    BTNode *n6 = find_val(root, 6);
    assert(n6 && successor(n6) && successor(n6)->data == 7);
    BTNode *n13 = find_val(root, 13);
    assert(n13 && successor(n13) && successor(n13)->data == 15);
    BTNode *n20 = find_val(root, 20);
    assert(n20 && successor(n20) == NULL);
    release_tree(root);
}

static void test_empty_tree_edges(void) {
    BTNode *root = NULL;
    assert(size(root) == 0);
    assert(is_bst(root, INT_MIN, INT_MAX));
    bfs(root);
    dfs(root, INORDER);
    release_tree(root);
}

int main(void) {
    test_insert_basic_shape();
    test_inorder_sorted();
    test_minimum_and_successor();
    test_empty_tree_edges();
    printf("All tests passed.\n");
    return 0;
}
