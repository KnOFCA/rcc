// Author: KnOFCA

int getpid(void);
int write(int, char *, int);
void exit(int);

int main() {
  int pid = getpid();
  if (pid > 0) {
    write(1, "RCC_SYS OK\n", 11);
    exit(0);
  }

  write(1, "RCC_SYS FAIL\n", 13);
  exit(1);
  return 0;
}