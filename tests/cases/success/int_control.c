int main() {
  int x = 1;
  int y = 2;
  while (x < 7) {
    y = y + x;
    x = x + 1;
  }
  if (y > 20) {
    return y;
  }
  return 0;
}
