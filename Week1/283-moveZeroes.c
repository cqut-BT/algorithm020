void moveZeroes(int* nums, int numsSize)
{
    int fast = 0;
    int slow = 0;
	
    for (fast = 0; fast < numsSize; fast++) 
    {
        if (nums[fast] != 0) 
        {
            if (fast > slow) 
            {
	nums[slow] = nums[fast];
                nums[fast] = 0;
            }
            slow++;
        } 
    }
}
