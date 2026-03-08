int main()
{
    int arr[5] = {1, 2, 3, 4, 5};
    int *p = arr;

    // 指针算术
    p++;
    p--;
    p += 2;
    int diff = p - arr;
}