int main() {
  char buf[4];
  char *p = buf;
  buf[1] = 'A';
  p[2] = 'B';
  return buf[1] + p[2];
}
