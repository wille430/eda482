
void memset(int * ptr, int value, int num)
{
    for (int i = 0; i < num; i++)
    {
        *(ptr+i) = value;
    }
}

void memcpy(int * ptr, int * src, int num)
{
    for (int i = 0; i < num; i++)
    {
        *(ptr+i) = *(src+i);
    }
}