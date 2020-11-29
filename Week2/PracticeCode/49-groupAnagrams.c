/*
1、排序法：如果是字母异构词，排序后就是同一个词；
2、哈希表：关键在于如何构建映射关系，找到合适的哈希函数；
质数法，通过不同字母组合的乘积不同，将字符串与数字对应作为hash的key
用C语言写hash感觉好难，之前没用过malloc这些对内存操作的库函数
*/

// 排序+hash
#define MAXSIZE 10000
#define STRSIZE 1000

struct SortArray 
{
    int index;
    char str[STRSIZE];
};
int StrCompare(const void *a, const void *b)
{
    struct SortArray temp1 = *(struct SortArray *)a;
    struct SortArray temp2 = *(struct SortArray *)b;
    return strcmp(temp1.str, temp2.str);
}
int Cmp(const void *a, const void *b)
{
    return *(char *)a - *(char *)b;
}
char ***groupAnagrams(char **strs, int strsSize, int *returnSize, int **returnColumnSizes)
{
    *returnSize = 0;
    if ((strs == NULL) || (strsSize == 0)) {
        return NULL;
    }
    char ***ret = (char ***)malloc(sizeof(char **) * strsSize);
    *returnColumnSizes = (int *)malloc(sizeof(int) * MAXSIZE);
    struct SortArray *ans = (struct SortArray *)malloc(sizeof(struct SortArray) * strsSize);
    memset(ans, 0, sizeof(struct SortArray) * strsSize);
    int i, j;
    for (i = 0; i < strsSize; i++) {
        ans[i].index = i;
        strcpy(ans[i].str, strs[i]);
        qsort(ans[i].str, strlen(strs[i]), sizeof(ans[i].str[0]), Cmp);
    }
    qsort(ans, strsSize, sizeof(ans[0]), StrCompare);
    int left = 0;
    int right = 0;
    int index = 0;
    int curLen;
    while (right <= strsSize) 
    {
        if ((right == strsSize) || (strcmp(ans[right].str, ans[left].str) != 0)) 
        {
            curLen = right - left;
            ret[*returnSize] = (char **)malloc(sizeof(char *) * curLen);
            (*returnColumnSizes)[*returnSize] = curLen;
            index = 0;
            for (i = left; i < right; i++) 
            {
                ret[*returnSize][index++] = strs[ans[i].index];
            }
            (*returnSize)++;
            left = right;
        }
        right++;
    }
    return ret;
}

// 利用C语言的uthash.h库
#define KEYLEN 26
typedef struct 
{
    uint8_t key[KEYLEN];
    int listSize;
    char **list;
    UT_hash_handle hh;
} Map;

void create_freq_map(char *str, uint8_t *fm) 
{
   for(int i = 0; str[i] != 0; i++) 
   {
        fm[str[i] - 'a']++;
   }
}

char *** groupAnagrams(char ** strs, int strsSize, int* returnSize, int** returnColumnSizes)
{
    Map *map = NULL, *elem = NULL, *tmp;
    uint8_t freqMap[KEYLEN];
    char ***result = NULL;
    int mapIndx = 0;

    if (strsSize <= 0) 
    {
        return NULL;
    }
    for (int i = 0; i < strsSize; i++) 
    {
        memset(freqMap, 0, sizeof(freqMap));
        create_freq_map(strs[i], freqMap);
        HASH_FIND(hh, map, freqMap, KEYLEN, elem);
        if (!elem) 
        {
            elem = malloc(sizeof(Map));
            memcpy(elem->key, freqMap, KEYLEN);
            elem->listSize = 1;
            elem->list = malloc(sizeof(char **));
            elem->list[0] = strs[i];
            HASH_ADD(hh, map, key, KEYLEN, elem);
        } else 
        {
            elem->listSize++;
            elem->list = realloc(elem->list, elem->listSize * sizeof(char **));
            elem->list[elem->listSize - 1] = strs[i];
        }
    }
    *returnSize = HASH_COUNT(map);
    result = malloc(*returnSize * sizeof(char **));
    *returnColumnSizes = malloc(*returnSize * sizeof(int));
    HASH_ITER(hh, map, elem, tmp) 
    {
        (*returnColumnSizes)[mapIndx] = elem->listSize;
        result[mapIndx++] = elem->list;
    }
    HASH_ITER(hh, map, elem, tmp) 
    {
        free(elem);
    }
    return result;
}