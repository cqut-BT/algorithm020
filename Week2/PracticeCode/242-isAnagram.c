/* 两个字符串是否互为字母异位词必须满足以下两个条件：
*  1.两个字符串的长度相等；
*  2.每个字符串中含有的字符相同、相同字符的个数也相同。
*/

/* 通过用一个哈希表在遍历字符串时，将其中一个字符串中每个字符出现的次数自加，
*  将另一个字符串中的自减，最后判断哈希表中所有元素是否为 0 即可。
*  类似于先将数据压入栈中，后弹出栈，看栈是否为空。
*/
#define MAXNUM 26       // 题目已告知可以假设字符串只包含小写字母，所以最大下标为 'z' - 'a'

bool isAnagram(char * s, char * t)
{
    int lenS = 0;
    int lenT = 0;
    /* 构建一个数组，记录字符串中每个字符出现的次数 */
    char hash[MAXNUM] = {0};

    /* 边界判断，看字符串是否为空，为空的话也不能用 strlen 求其长度 */
    if (s == NULL && t == NULL) 
    {
        return true;
    } 
    else if (s == NULL && t != NULL || s != NULL && t == NULL) 
    {
        return false;
    }

    /* 获取两个非空字符串的长度，长度不相等肯定不互为字母异位词 */
    lenS = strlen(s);
    lenT = strlen(t);
    if (lenS != lenT) 
    {
        return false;
    }

    for (int i = 0; i < lenT; ++i) 
    {
        /* 数组下标为 0 时，对应该字符串中的字符 a，依次类推 */
        hash[s[i] - 'a']++;
        hash[t[i] - 'a']--;
    }

    for (int i = 0; i < MAXNUM; ++i) 
    {
        if (hash[i] != 0) 
        {
            return false;
        }
    }

    return true;
}