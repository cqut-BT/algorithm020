int comp(const void* a, const void* b) 
{
    return (*(int*)a) - (*(int*)b);
}

int findContentChildren(int* g, int gSize, int* s, int sSize)
{
    if (gSize == 0 || sSize == 0) 
    {
        return 0;
    }

    qsort(g, gSize, sizeof(int), comp);
    qsort(s, sSize, sizeof(int), comp);
    
    int i = 0, j = 0;
    int ans = 0;
    while (i < gSize && j < sSize) 
    {
        if (g[i] <= s[j]) 
        {
            ans++;
            i++;       
        }
        j++;
    }

    return ans;
}