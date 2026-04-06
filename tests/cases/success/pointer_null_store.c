int main() {
  char *argv[3];
  argv[0] = "echo";
  argv[1] = "ok";
  argv[2] = 0;
  return argv[2] != 0;
}
