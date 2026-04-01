int getpid(void);
int write(int, char *, int);
void exit(int);

int main() {
  int pid = getpid();
  write(1, "x", 1);
  exit(pid);
  return 0;
}
