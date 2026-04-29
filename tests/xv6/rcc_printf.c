// Author: KnOFCA

void fprintf(int fd, const char *fmt, ...);
void exit(int);

int main() {
  fprintf(2, "RCC_PRINTF %d %s\n", 7, "OK");
  exit(0);
  return 0;
}