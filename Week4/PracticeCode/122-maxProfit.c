int maxProfit(int* prices, int pricesSize){
    int max = 0, i;

    for(i=0;i<pricesSize-1;i++)
    {
        max += (prices[i+1]-prices[i])>0?(prices[i+1]-prices[i]):0;
    }

    return max;
}