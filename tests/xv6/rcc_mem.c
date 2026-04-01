int write(int, char *, int);
void exit(int);

char gbuf[] = "abc";
char gzero[4];

int main() {
  char buf[4];
  char *p = buf;

  buf[1] = 'A';
  p[2] = 'B';

  if (gbuf[1] == 'b') {
    if (gzero[0] == 0) {
      if (buf[1] == 'A') {
        if (p[2] == 'B') {
          write(1, "RCC_MEM OK\n", 11);
          exit(0);
        }
      }
    }
  }

  write(1, "RCC_MEM FAIL\n", 13);
  exit(1);
  return 0;
}
