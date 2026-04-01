int write(int, char *, int);
void exit(int);

int main() {
  write(1, "RCC_PING OK\n", 12);
  exit(0);
  return 0;
}
