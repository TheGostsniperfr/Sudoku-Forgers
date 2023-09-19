int findOtsuSigma(int* histogram){
    int sum1 = 0;

    for (int i = 0; i < 256; i++)
    {       
       sum1 += histogram[i] * i;
    }
    
}