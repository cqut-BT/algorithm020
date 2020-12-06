/*
分治、回溯
最近公共祖先 其实就是两个节点向根节点探索，第一次相遇的节点就是 最近公共祖先节点。

如果 root 是 p q 的最近公共祖先节点，那么 p q 必然都在 root 的子树中，
1、p q 分别在 root 的左右子树
2、p 或者 q 为根节点， 另一个节点是其子节点。
*/

struct TreeNode* lowestCommonAncestor(struct TreeNode* root, struct TreeNode* p, struct TreeNode* q) 
{
    // recursion terminator 递归终止条件
    if (NULL == root || root == p || root == q)
        return root;

    // prepare data 处理当前层逻辑

    // conquer subproblems 下探到下一层，处理更细节的子问题
    // 1、以 root.left 为根节点，寻找 最近公共祖先
    struct TreeNode *left = lowestCommonAncestor(root->left, p, q);
    // 2、以 root.right 为根节点，寻找 最近公共祖先
    struct TreeNode *right = lowestCommonAncestor(root->right, p, q);

    // process and generate the final result 整合返回的子结果，得到最终结果
    if (NULL != left && NULL != right)
    {
        return root;
    }    
    if (left != NULL) 
    {
        return left;
    }

    // revert the current level states 返回最终结果
    return right;
}