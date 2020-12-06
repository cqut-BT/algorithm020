/*
递归法：
1、前序遍历：根节点-左支-右支
2、中序遍历：左支-根节点-右支
3、从前序遍历数组中取得根节点,并计算左支节点数和右支节点数
4、递归处理左支、右支
*/

struct TreeNode* buildTree(int* preorder, int preorderSize, int* inorder, int inorderSize)
{
    int index = 0;

    if (preorderSize <= 0) 
    {
        return NULL;
    }

    struct TreeNode *root = (struct TreeNode *)malloc(sizeof(struct TreeNode));
    root->val = preorder[0];
    
    while (index < inorderSize && inorder[index] != preorder[0])
    {
        index++;
    } 

    root->left = buildTree(preorder + 1, index, inorder, index);
    root->right = buildTree(preorder + 1 + index, preorderSize - (index + 1), inorder + index + 1, inorderSize - (index + 1));

    return root;
}