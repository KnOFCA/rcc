void* foo(int** a)
{
    return *a;
}

int main()
{
    int** a;
    foo(a);
    return 0;
}