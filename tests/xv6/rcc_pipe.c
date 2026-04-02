int write(int, char *, int);
int read(int, char *, int);
int close(int);
int pipe(int *);
int fork(void);
int wait(int *);
void exit(int);

void fill_pattern(char *buf, int n) {
  int i = 0;
  while (i < n) {
    if (i % 3 == 0) {
      buf[i] = 'a' + (i % 26);
    } else if (i % 3 == 1) {
      buf[i] = 'A' + (i % 26);
    } else {
      buf[i] = '0' + (i % 10);
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

int sum_bytes(char *buf, int n) {
  int i = 0;
  int total = 0;
  while (i < n) {
    total = total + buf[i];
    i = i + 1;
  }
  return total;
}

int child_run(int *down, int *up) {
  char recv[48];
  char ack[12];
  int got;
  int n;

  if (close(down[1]) < 0) return 70;
  if (close(up[0]) < 0) return 71;

  got = 0;
  while (got < 48) {
    n = read(down[0], recv + got, 7);
    if (n <= 0) return 72;
    got = got + n;
  }

  if (read(down[0], recv, 1) != 0) return 73;
  if (close(down[0]) < 0) return 74;

  if (recv[0] != 'a') return 75;
  if (recv[1] != 'B') return 76;
  if (recv[2] != '2') return 77;
  if (recv[45] != 't') return 78;
  if (recv[46] != 'U') return 79;
  if (recv[47] != '7') return 80;

  ack[0] = 'O';
  ack[1] = 'K';
  ack[2] = ':';
  ack[3] = '0' + (sum_bytes(recv, 16) % 10);
  ack[4] = '0' + (sum_bytes(recv + 16, 16) % 10);
  ack[5] = '0' + (sum_bytes(recv + 32, 16) % 10);
  ack[6] = ':';
  ack[7] = recv[5];
  ack[8] = recv[23];
  ack[9] = recv[41];
  ack[10] = '\n';
  ack[11] = 0;

  if (write(up[1], ack, 11) != 11) return 81;
  if (close(up[1]) < 0) return 82;
  return 0;
}

int main() {
  int down[2];
  int up[2];
  int pid;
  int status;
  char send[48];
  char expect[12];
  char got[12];
  int n;

  fill_pattern(send, 48);

  if (pipe(down) < 0) {
    write(1, "RCC_PIPE FAIL\n", 14);
    exit(1);
  }
  if (pipe(up) < 0) {
    write(1, "RCC_PIPE FAIL\n", 14);
    exit(1);
  }

  pid = fork();
  if (pid < 0) {
    write(1, "RCC_PIPE FAIL\n", 14);
    exit(1);
  }
  if (pid == 0) {
    exit(child_run(down, up));
  }

  if (close(down[0]) < 0) {
    write(1, "RCC_PIPE FAIL\n", 14);
    exit(1);
  }
  if (close(up[1]) < 0) {
    write(1, "RCC_PIPE FAIL\n", 14);
    exit(1);
  }

  if (write(down[1], send, 5) != 5) {
    write(1, "RCC_PIPE FAIL\n", 14);
    exit(1);
  }
  if (write(down[1], send + 5, 17) != 17) {
    write(1, "RCC_PIPE FAIL\n", 14);
    exit(1);
  }
  if (write(down[1], send + 22, 26) != 26) {
    write(1, "RCC_PIPE FAIL\n", 14);
    exit(1);
  }
  if (close(down[1]) < 0) {
    write(1, "RCC_PIPE FAIL\n", 14);
    exit(1);
  }

  n = read(up[0], got, 11);
  if (n != 11) {
    write(1, "RCC_PIPE FAIL\n", 14);
    exit(1);
  }
  if (read(up[0], got, 1) != 0) {
    write(1, "RCC_PIPE FAIL\n", 14);
    exit(1);
  }
  if (close(up[0]) < 0) {
    write(1, "RCC_PIPE FAIL\n", 14);
    exit(1);
  }

  expect[0] = 'O';
  expect[1] = 'K';
  expect[2] = ':';
  expect[3] = '0' + (sum_bytes(send, 16) % 10);
  expect[4] = '0' + (sum_bytes(send + 16, 16) % 10);
  expect[5] = '0' + (sum_bytes(send + 32, 16) % 10);
  expect[6] = ':';
  expect[7] = send[5];
  expect[8] = send[23];
  expect[9] = send[41];
  expect[10] = '\n';
  expect[11] = 0;

  if (!same_bytes(got, expect, 11)) {
    write(1, "RCC_PIPE FAIL\n", 14);
    exit(1);
  }

  status = -1;
  if (wait(&status) < 0) {
    write(1, "RCC_PIPE FAIL\n", 14);
    exit(1);
  }
  if (status != 0) {
    write(1, "RCC_PIPE FAIL\n", 14);
    exit(1);
  }

  write(1, "RCC_PIPE OK\n", 12);
  exit(0);
  return 0;
}
