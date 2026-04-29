// Author: KnOFCA

int write(int, char *, int);
int fork(void);
int wait(int *);
int getpid(void);
void exit(int);

int g_base = 7;
int g_scale = 3;
int g_data[6] = {2, 4, 6, 8, 10, 12};
int g_zero[4];

int compute_mix(int *buf, int n, int bias) {
  int i = 0;
  int total = 0;
  while (i < n) {
    total = total + buf[i] * (i + 1);
    total = total + bias;
    i = i + 1;
  }
  return total;
}

void fill_local(int *buf, int n, int seed) {
  int i = 0;
  while (i < n) {
    buf[i] = seed + i * g_scale;
    i = i + 1;
  }
}

int check_parent_state(void) {
  if (g_base != 7) return 0;
  if (g_scale != 3) return 0;
  if (g_data[0] != 2) return 0;
  if (g_data[3] != 8) return 0;
  if (g_zero[0] != 0) return 0;
  if (g_zero[3] != 0) return 0;
  return 1;
}

int child_one_code(void) {
  int local[5];
  int total;

  fill_local(local, 5, 1);
  total = compute_mix(local, 5, 0);
  total = total + g_data[2];

  g_base = 99;
  g_data[1] = 77;
  local[0] = 55;

  if (total == 100) {
    return 13;
  }
  return 91;
}

int child_two_code(void) {
  int local[4];
  int total;

  fill_local(local, 4, 2);
  total = compute_mix(local, 4, 1);
  total = total + g_base + g_zero[1];

  g_scale = 11;
  g_zero[2] = 5;
  local[3] = 44;

  if (total == 69) {
    return 29;
  }
  return 92;
}

int main() {
  int before;
  int pid1;
  int pid2;
  int status1;
  int status2;
  int got13;
  int got29;
  int parent_local[3];

  parent_local[0] = 1;
  parent_local[1] = 2;
  parent_local[2] = 3;

  before = getpid();
  if (before <= 0) {
    write(1, "RCC_PROC FAIL\n", 14);
    exit(1);
  }

  pid1 = fork();
  if (pid1 < 0) {
    write(1, "RCC_PROC FAIL\n", 14);
    exit(1);
  }
  if (pid1 == 0) {
    exit(child_one_code());
  }

  pid2 = fork();
  if (pid2 < 0) {
    write(1, "RCC_PROC FAIL\n", 14);
    exit(1);
  }
  if (pid2 == 0) {
    exit(child_two_code());
  }

  if (pid1 == pid2) {
    write(1, "RCC_PROC FAIL\n", 14);
    exit(1);
  }

  if (!check_parent_state()) {
    write(1, "RCC_PROC FAIL\n", 14);
    exit(1);
  }

  if (parent_local[0] != 1 || parent_local[1] != 2 || parent_local[2] != 3) {
    write(1, "RCC_PROC FAIL\n", 14);
    exit(1);
  }

  status1 = -1;
  status2 = -1;
  if (wait(&status1) < 0) {
    write(1, "RCC_PROC FAIL\n", 14);
    exit(1);
  }
  if (wait(&status2) < 0) {
    write(1, "RCC_PROC FAIL\n", 14);
    exit(1);
  }

  got13 = 0;
  got29 = 0;
  if (status1 == 13) got13 = 1;
  if (status2 == 13) got13 = 1;
  if (status1 == 29) got29 = 1;
  if (status2 == 29) got29 = 1;

  if (!got13 || !got29) {
    write(1, "RCC_PROC FAIL\n", 14);
    exit(1);
  }

  if (!check_parent_state()) {
    write(1, "RCC_PROC FAIL\n", 14);
    exit(1);
  }

  if (wait(&status1) != -1) {
    write(1, "RCC_PROC FAIL\n", 14);
    exit(1);
  }

  write(1, "RCC_PROC OK\n", 12);
  exit(0);
  return 0;
}