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


// 纯C hash leetcode上用时最短的写法
typedef struct node Node;
struct node
{
    void* key;
    void* value;
    int hash;
    Node* next;
};

typedef struct hash_map
{
    int size;
    int length;
    Node** data;
    int(*hash_code)(void*);
    int(*equals)(void*,void*);
}HashMap;

/*
    扩容
*/
void resize(HashMap* hashMap)
{
    int oldSize = hashMap->size;
    int newSize = oldSize * 2;
    Node** newCup = (Node**)malloc(sizeof(Node*) * newSize);
    memset(newCup, 0, sizeof(Node*) * newSize);
    //拷贝到老数组
    for (int j = 0, size = hashMap->size; j < size; j++)
    {
        Node* e = hashMap->data[j];
        hashMap->data[j] = NULL;
        if (e == NULL)
        {
            continue;
        }
        if (e->next == NULL)
        {
            newCup[e->hash & (newSize - 1)] = e;
        }
        Node* loTail = NULL;
        Node* hiTail = NULL;
        Node* loHead = NULL;
        Node* hiHead = NULL;
        Node* next;
        do
        {
            next = e->next;
            //放在高位
            if ((e->hash & oldSize) != 0)
            {
                if (hiTail == NULL)
                {
                    hiHead = e;
                }
                else
                {
                    hiTail->next = e;
                }
                hiTail = e;
            }
            else
            {
                if (loHead == NULL)
                {
                    loHead = e;
                }
                else
                {
                    loTail->next = e;
                }
                loTail = e;
            }
        } while ((e = next) != NULL);
        newCup[j + oldSize] = hiHead;
        newCup[j] = loHead;
        if (hiTail != NULL)
            hiTail->next = NULL;
        if (loTail != NULL)
            loTail->next = NULL;
    }
    free(hashMap->data);
    hashMap->data = newCup;
    hashMap->size = newSize;
}

Node* createNode(int hash, void* key, void* value, Node* next)
{
    Node* node = (Node*)malloc(sizeof(Node));
    node->hash = hash;
    node->key = key;
    node->value = value;
    node->next = NULL;
    return node;
}

void free_node(Node* node)
{
    if (node == NULL)
        return;
    free_node(node->next);
    node->key != NULL ? free(node->key) : NULL;
    node->value != NULL ? free(node->value) : NULL;
    free(node);
}

void free_map0(HashMap* hashMap,int is_free)
{
    for (int i = 0, size = hashMap->size; i < size; i++)
    {
        if (!is_free)
        {
            hashMap->data[i] = NULL;
            continue;
        }
        Node* e = hashMap->data[i];
        if (e != NULL && e->next == NULL)
        {
            e->key != NULL ? free(e->key) : NULL;
            e->value != NULL ? free(e->value) : NULL;
            free(e);
        }
        else
            free_node(e);
    }
}

/*
    free
*/
void free_map(HashMap* hashMap,int is_free)
{
    free_map0(hashMap,is_free);
    free(hashMap->data);
    free(hashMap);
}

/*
    get
*/
void* get(HashMap* hashMap, void* key)
{
    int hash = hashMap->hash_code(key);
    int index = (hashMap->size - 1) & hash;
    Node* node = hashMap->data[index];
    if (node == NULL)
        return NULL;
    if (node->next == NULL)
        return hashMap->equals(node->key,key) ? node->value : NULL;
    Node* next;
    do
    {
        next = node->next;
        if (hashMap->equals(node->key,key))
            return node->value;
    } while ((node = next) != NULL);
    return NULL;
}

/*
    contains
*/
int contains(HashMap* hashMap, void* key)
{
    int hash = hashMap->hash_code(key);
    int index = (hashMap->size - 1) & hash;
    Node* node = hashMap->data[index];
    if (node == NULL)
        return 0;
    if (node->next == NULL)
        return hashMap->equals(node->key,key) ? 1 : 0;
    Node* next;
    do
    {
        next = node->next;
        if (hashMap->equals(node->key,key))
            return 1;
    } while ((node = next) != NULL);
    return 0;
}

/*
    新增
*/
void put(HashMap* hashMap,void* key, void* value)
{
    if (hashMap == NULL)
        return;
    if (hashMap->length == hashMap->size * 0.75)
        resize(hashMap);
    int hash = hashMap->hash_code(key);
    int index = hash & (hashMap->size - 1);
    Node** nodeIndex = &hashMap->data[index];
    //如果该槽位无元素则直接插入
    if (*nodeIndex == NULL)
    {
        *nodeIndex = createNode(hash, key, value, NULL);
        hashMap->length++;
        return;
    }
    Node* e = *nodeIndex;
    Node* next = e;
    do
    {
        e = next;
        next = e->next;
        if (hashMap->equals(e->key,key))
        {
            e->value = value;
            return;
        }
    } while (next != NULL);
    e->next = createNode(hash, key, value, NULL);
    hashMap->length++;
}

/*
    clean
 */
void clean(HashMap* hashMap,int is_free)
{
    free_map0(hashMap,is_free);
}

/*
    创建hashMap,需指定计算hashcode和equals函数
*/
HashMap* newHashMap(int(*hash_code)(void*),int(*equals)(void*,void*))
{
    HashMap* hashMap = (HashMap*)malloc(sizeof(HashMap));
    hashMap->size = 16;
    hashMap->length = 0;
    hashMap->data = (Node**)malloc(sizeof(Node*) * hashMap->size);
    hashMap->hash_code = hash_code;
    hashMap->equals = equals;
    memset(hashMap->data, 0, sizeof(Node*) * hashMap->size);
    return hashMap;
}

static int equals(void* a,void* b)
{
    return (*(int*)a) == (*(int*)b);
}

static int hashCode(void* a)
{
    return *((int*)a);
}

static int str_hashCode(char* key)
{
    long long hash = 0;
    while (*key)
    {
        hash += pow(*key, 4);
        key++;
    }
    hash ^= hash>>16;
    return (int)hash;
}

typedef struct list
{
    int size;
    char** arr;
}List;

List* newList()
{
    List* list = malloc(sizeof(List));
    list->size = 0;
    list->arr = (char**)malloc(sizeof(char*) * 100);
    return list;
}

char *** groupAnagrams(char** strs, int strsSize, int* returnSize, int** returnColumnSizes)
{
    char*** res = (char***)malloc(sizeof(char**) * strsSize);
    *returnSize = 0;
    int* cache[10000] = {0};
    int cache_idx = 0;
    *returnColumnSizes = (int*)malloc(sizeof(int) * 10000);
    HashMap* map = newHashMap(hashCode, equals);
    for (int i = 0; i < strsSize; i++)
    {
        //计算hash
        int hash = str_hashCode(strs[i]);
        int* hash_p = (int*)malloc(sizeof(int));
        *hash_p = hash;
        List* value = (List*)get(map, hash_p);
        if (value == NULL)
        {
            cache[cache_idx++] = hash_p;
            value = newList();
            value->arr[value->size++] = strs[i];
            put(map, hash_p, value);
        }
        else
        {
            free(hash_p);
            value->arr[value->size++] = strs[i];
        }
    }
    for (int i = 0; i < cache_idx; i++)
    {
        List* value = (List*)get(map, cache[i]);
        res[i] = value->arr;
        (*returnColumnSizes)[i] = value->size;
        (*returnSize)++;
    }
    free_map(map, 1);
    return res;
}