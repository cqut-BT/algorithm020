// 1、暴力解法... 2、重复规律...

int removeDuplicates(int* nums, int numsSize)
{
    unsigned int i = 0, j = 1;
    
    if(numsSize < 2)
    {
        return numsSize;
    }

    while(j < numsSize)
    {
        if(nums[j++] > nums[i])
        {
            nums[++i] = nums[j - 1];
        }         
    }

    return (i + 1);
}

/* 
int removeDuplicates(int* nums, int numsSize)
{
    int j=0;
    int i=1;
    if(numsSize==0)
    {
        return 0;
    }
    for(i=1;i<numsSize;i++)
    {
        if(nums[i]!=nums[j])
        {
            j++;
            nums[j]=nums[i];
        }
    }
    return j+1;
}
*