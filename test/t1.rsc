int *ptr;                      // 指针
int arr[10];                   // 数组
int *ptr_arr[5];               // 指针数组
int (*func_ptr)(int, int);     // 函数指针
int (*arr_ptr)[10];            // 数组指针
const int *const const_ptr = nullptr;    // 常量指针
int (*complex_arr[3])(int);    // 函数指针数组

/* ------------------- 表达式与运算符测试 ------------------- */
void test_expressions(void) {
    int a = 10, b = 20, c;
    float f = 3.14;
    char ch = 'A';

    // 算术运算
    c = a + b;
    c = a - b;
    c = a * b;
    c = a / b;
    c = a % b;

    // 关系与逻辑
    int cond = (a > b) && (f != 0) || (ch == 'A');

    // 位运算
    c = a & b;
    c = a | b;
    c = a ^ b;
    c = ~a;
    //c = a << 2;
    //c = a >> 1;

    // 赋值运算
    // a += b;
    // a -= b;
    // a *= b;
    // a /= b;
    // a %= b;
    // a &= b;
    // a |= b;
    // a ^= b;
    // a <<= 1;
    // a >>= 1;

    // 自增自减
    a++;
    ++a;
    b--;
    --b;

    // 条件运算符
    c = (a > b) ? a : b;

    // 逗号运算符
    c = (a++, b++, a + b);

    // 类型转换
    // f = (float)a;
    // c = (int)f;
}

/* ------------------- 控制流测试 ------------------- */
void test_control_flow(void) {
    int i = 0, j = 0;

    // if-else
    if (i > 0) {
        j = 1;
    } else if (i < 0) {
        j = -1;
    } else {
        j = 0;
    }

    // switch
    switch (i) {
        case 0:
            break;
        case 1:
            /* fallthrough */
        default:
            break;
    }

    // while
    while (i < 10) {
        i++;
    }

    // do-while
    do {
        i--;
    } while (i > 0);

    // for
    for (int j = 0; j < 10; j++) {
        if (j == 5) continue;
        if (j == 8) break;
    }

    // goto
    goto label;
label:
    i = 0;

    // return
    return;
}

/* ------------------- 数组与字符串测试 ------------------- */
void test_arrays(void) {
    int arr1[10];               // 一维数组
    int arr2[3][4];             // 多维数组
    char str1[] = "Hello";      // 字符串字面量
    char str2[10] = {'H', 'i'};

    // 数组访问
    arr1[0] = 100;
    arr2[1][2] = 200;
}

/* ------------------- 指针运算测试 ------------------- */
void test_pointers(void) {
    int arr[5] = {1, 2, 3, 4, 5};
    int *p = arr;

    // 指针算术
    p++;
    p--;
    p += 2;
    int diff = p - arr;

    // 解引用
    *p = 10;
    int val = *(p + 1);

    // 指针与数组
    p = &arr[2];
    val = p[1];   // 等价于 *(p+1)
}