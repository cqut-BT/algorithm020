int visited[100];
int **result = NULL;
int *resultColSize = NULL;
int count;
int *g_pBuf = NULL;
int DFS(int start, int max, int index, int end)
{
    // 满足条件后保存一个结果
    if(index == end)
    {
        result[count] = malloc(end<<2);
        memcpy(result[count], g_pBuf, end << 2);
        count++;
        return 0;
    }
    // 组合问题，只要考虑继续往大增长即可，排列问题则调换位置后的结果也是要考虑的
    for(int i = start; i <= max && (end - index - 1) <= (max - start); i++) 
    {
//        if(visited[i] == 0){  // 因为是组合问题，只考虑往大增长，因此不应该遇到之前已经visited的点了
            g_pBuf[index] = i;
//            visited[i] = 1;
            DFS(i + 1, max, index + 1, end);
//            visited[i] = 0;  //回溯恢复了现场
//        }
    }
    return 0;
}

int** combine(int n, int k, int* returnSize, int** returnColumnSizes)
{
    g_pBuf = (int *)malloc(k * sizeof(int));
    result = (int **)malloc(10000*sizeof(int*));
    *returnColumnSizes = (int *)malloc(10000*sizeof(int));

    count = 0;
    memset(visited, 0, sizeof(visited));

    DFS(1, n, 0, k);

    *returnSize = count;
    for(int i = 0; i < count; i++)
    {
        (*returnColumnSizes)[i] = k;
    }
    return result;
}