// Author: KnOFCA

int write(int, char *, int);
void exit(int);

int pick9(int a, int b, int c, int d, int e, int f, int g, int h, int i) {
  return i - a;
}

int main() {
  if (pick9(1, 2, 3, 4, 5, 6, 7, 8, 9) == 8) {
    write(1, "RCC_CALL9 OK\n", 13);
    exit(0);
  }

  write(1, "RCC_CALL9 FAIL\n", 15);
  exit(1);
  return 0;
}