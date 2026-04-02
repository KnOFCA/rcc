int write(int, char *, int);
int read(int, char *, int);
int open(char *, int);
int close(int);
int unlink(char *);
void exit(int);

int O_RDONLY = 0;
int O_RDWR = 2;
int O_CREATE = 512;
int O_TRUNC = 1024;

char path[] = "rcc_file_tmp.txt";
char header[] = "HEAD:";
char trailer[] = ":TAIL\n";

void build_payload(char *buf, int n) {
  int i = 0;
  while (i < n) {
    if (i % 4 == 0) {
      buf[i] = 'k' + (i % 11);
    } else if (i % 4 == 1) {
      buf[i] = '0' + (i % 10);
    } else if (i % 4 == 2) {
      buf[i] = 'A' + (i % 26);
    } else {
      buf[i] = '_';
    }
    i = i + 1;
  }
}

int same_bytes(char *a, char *b, int n) {
  int i = 0;
  while (i < n) {
    if (a[i] != b[i]) return 0;
    i = i + 1;
  }
  return 1;
}

int write_all(int fd, char *buf, int n) {
  int off = 0;
  int m;
  while (off < n) {
    m = write(fd, buf + off, n - off);
    if (m <= 0) return -1;
    off = off + m;
  }
  return 0;
}

int read_all(int fd, char *buf, int n) {
  int off = 0;
  int m;
  while (off < n) {
    m = read(fd, buf + off, 9);
    if (m <= 0) return -1;
    off = off + m;
  }
  return off;
}

int main() {
  int fd;
  char payload[64];
  char expect[75];
  char got[75];
  int n;

  build_payload(payload, 64);

  unlink(path);
  fd = open(path, O_CREATE + O_RDWR + O_TRUNC);
  if (fd < 0) {
    write(1, "RCC_FILE FAIL\n", 14);
    exit(1);
  }

  if (write_all(fd, header, 5) < 0) {
    write(1, "RCC_FILE FAIL\n", 14);
    exit(1);
  }
  if (write_all(fd, payload, 13) < 0) {
    write(1, "RCC_FILE FAIL\n", 14);
    exit(1);
  }
  if (write_all(fd, payload + 13, 19) < 0) {
    write(1, "RCC_FILE FAIL\n", 14);
    exit(1);
  }
  if (write_all(fd, payload + 32, 32) < 0) {
    write(1, "RCC_FILE FAIL\n", 14);
    exit(1);
  }
  if (write_all(fd, trailer, 6) < 0) {
    write(1, "RCC_FILE FAIL\n", 14);
    exit(1);
  }
  if (close(fd) < 0) {
    write(1, "RCC_FILE FAIL\n", 14);
    exit(1);
  }

  fd = open(path, O_RDONLY);
  if (fd < 0) {
    write(1, "RCC_FILE FAIL\n", 14);
    exit(1);
  }

  n = read_all(fd, got, 75);
  if (n != 75) {
    write(1, "RCC_FILE FAIL\n", 14);
    exit(1);
  }
  if (read(fd, got, 1) != 0) {
    write(1, "RCC_FILE FAIL\n", 14);
    exit(1);
  }
  if (close(fd) < 0) {
    write(1, "RCC_FILE FAIL\n", 14);
    exit(1);
  }

  expect[0] = 'H';
  expect[1] = 'E';
  expect[2] = 'A';
  expect[3] = 'D';
  expect[4] = ':';
  n = 0;
  while (n < 64) {
    expect[5 + n] = payload[n];
    n = n + 1;
  }
  expect[69] = ':';
  expect[70] = 'T';
  expect[71] = 'A';
  expect[72] = 'I';
  expect[73] = 'L';
  expect[74] = '\n';

  if (!same_bytes(got, expect, 75)) {
    write(1, "RCC_FILE FAIL\n", 14);
    exit(1);
  }

  if (got[0] != 'H') {
    write(1, "RCC_FILE FAIL\n", 14);
    exit(1);
  }
  if (got[5] != payload[0]) {
    write(1, "RCC_FILE FAIL\n", 14);
    exit(1);
  }
  if (got[21] != payload[16]) {
    write(1, "RCC_FILE FAIL\n", 14);
    exit(1);
  }
  if (got[68] != payload[63]) {
    write(1, "RCC_FILE FAIL\n", 14);
    exit(1);
  }
  if (got[74] != '\n') {
    write(1, "RCC_FILE FAIL\n", 14);
    exit(1);
  }

  if (unlink(path) < 0) {
    write(1, "RCC_FILE FAIL\n", 14);
    exit(1);
  }
  if (open(path, O_RDONLY) >= 0) {
    write(1, "RCC_FILE FAIL\n", 14);
    exit(1);
  }

  write(1, "RCC_FILE OK\n", 12);
  exit(0);
  return 0;
}
