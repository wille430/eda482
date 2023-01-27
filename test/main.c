#include <stdio.h>
#include <string.h>

struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

char * tree2str(struct TreeNode* root){
    char buffer[1024];

    // Val
    char strVal[20];
    sprintf(strVal, "%d", root->val);
    strcat(buffer, strVal);

    // Left val
    strcat(buffer, "(");
    if (root->left) {
        strcat(buffer, tree2str(root->left));
    }
    strcat(buffer, ")");

    // Right val
    strcat(buffer, "(");
    if (root->right) {
        strcat(buffer, tree2str(root->right));
    }
    strcat(buffer, ")");
    return buffer;
}

int main(int argc, char **argv)
{
	struct TreeNode t;
    t.val = 10;
    t.left = NULL;
    t.right = NULL;
    printf(tree2str(&t));
	return 0;
}

