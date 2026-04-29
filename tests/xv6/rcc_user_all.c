// Author: KnOFCA

int fork(void);
int wait(int *);
int pipe(int *);
int write(int, char *, int);
int read(int, char *, int);
int close(int);
int kill(int);
int exec(char *, char **);
int open(char *, int);
int mknod(char *, int, int);
int unlink(char *);
int link(char *, char *);
int mkdir(char *);
int chdir(char *);
int dup(int);
int getpid(void);
char *sbrk(int);
int sleep(int);
int uptime(void);
void exit(int);

char *strcpy(char *, char *);
char *memmove(char *, char *, int);
char *strchr(char *, char);
int strcmp(char *, char *);
void fprintf(int, char *, ...);
void printf(char *, ...);
char *gets(char *, int);
int strlen(char *);
char *memset(char *, int, int);
int atoi(char *);
int memcmp(char *, char *, int);
char *memcpy(char *, char *, int);
char *malloc(int);
void free(char *);

int O_RDONLY = 0;
int O_RDWR = 2;
int O_CREATE = 512;
int O_TRUNC = 1024;

char ok_msg[] = "RCC_USER_ALL OK\n";
char fail_msg[] = "RCC_USER_ALL FAIL\n";

char dir_name[] = "rcc_user_dir";
char file_name[] = "base.txt";
char link_name[] = "hard.txt";
char node_name[] = "rcc_node";
char echo_prog[] = "echo";
char exec_word[] = "EXEC";

void fail(void) {
  write(1, fail_msg, 18);
  exit(1);
}

int same_bytes(char *a, char *b, int n) {
  int i;
  i = 0;
  while (i < n) {
    if (a[i] != b[i]) return 0;
    i = i + 1;
  }
  return 1;
}

int read_full(int fd, char *buf, int want) {
  int got;
  int n;

  got = 0;
  while (got < want) {
    n = read(fd, buf + got, want - got);
    if (n <= 0) return -1;
    got = got + n;
  }
  return got;
}

void fill_bytes(char *buf, int n, int seed) {
  int i;

  i = 0;
  while (i < n) {
    if (i % 3 == 0) {
      buf[i] = 'a' + ((seed + i) % 26);
    } else if (i % 3 == 1) {
      buf[i] = 'A' + ((seed + i) % 26);
    } else {
      buf[i] = '0' + ((seed + i) % 10);
    }
    i = i + 1;
  }
}

int sum_bytes(char *buf, int n) {
  int i;
  int total;

  i = 0;
  total = 0;
  while (i < n) {
    total = total + buf[i];
    i = i + 1;
  }
  return total;
}

void test_basic_proc(void) {
  int pid;
  int child;
  int status;

  pid = getpid();
  if (pid <= 0) fail();

  child = fork();
  if (child < 0) fail();
  if (child == 0) {
    exit(23);
  }

  status = -1;
  if (wait(&status) != child) fail();
  if (status != 23) fail();
}

void test_kill(void) {
  int child;
  int status;

  child = fork();
  if (child < 0) fail();
  if (child == 0) {
    while (1) {
      sleep(100);
    }
  }

  if (kill(child) < 0) fail();
  status = 0;
  if (wait(&status) != child) fail();
}

void test_pipe_read_write_close_dup(void) {
  int p[2];
  int dupfd;
  char send[19];
  char got[19];

  fill_bytes(send, 19, 4);

  if (pipe(p) < 0) fail();
  dupfd = dup(p[0]);
  if (dupfd < 0) fail();
  if (close(p[0]) < 0) fail();

  if (write(p[1], send, 6) != 6) fail();
  if (write(p[1], send + 6, 13) != 13) fail();
  if (close(p[1]) < 0) fail();

  if (read_full(dupfd, got, 19) != 19) fail();
  if (read(dupfd, got, 1) != 0) fail();
  if (close(dupfd) < 0) fail();

  if (!same_bytes(send, got, 19)) fail();
}

void test_sbrk_sleep_uptime(void) {
  char *base;
  char *end;
  int before;
  int after;

  before = uptime();
  if (before < 0) fail();
  if (sleep(5) < 0) fail();
  after = uptime();
  if (after < before) fail();
  if (after - before < 5) fail();

  base = sbrk(64);
  end = sbrk(0);
  if (end != base + 64) fail();
  fill_bytes(base, 64, 9);
  if (base[0] == 0) fail();
  if (base[63] == 0) fail();
}

void test_fs_calls(void) {
  int fd;
  char send[18];
  char got[18];

  unlink(node_name);
  unlink(link_name);
  unlink(file_name);
  unlink(dir_name);

  if (mkdir(dir_name) < 0) fail();
  if (chdir(dir_name) < 0) fail();

  fill_bytes(send, 18, 12);
  fd = open(file_name, O_CREATE + O_RDWR + O_TRUNC);
  if (fd < 0) fail();
  if (write(fd, send, 18) != 18) fail();
  if (close(fd) < 0) fail();

  if (link(file_name, link_name) < 0) fail();

  fd = open(link_name, O_RDONLY);
  if (fd < 0) fail();
  if (read_full(fd, got, 18) != 18) fail();
  if (read(fd, got, 1) != 0) fail();
  if (close(fd) < 0) fail();
  if (!same_bytes(send, got, 18)) fail();

  if (mknod(node_name, 1, 1) < 0) fail();
  fd = open(node_name, O_RDWR);
  if (fd < 0) fail();
  if (close(fd) < 0) fail();

  if (unlink(node_name) < 0) fail();
  if (unlink(link_name) < 0) fail();
  if (unlink(file_name) < 0) fail();
  if (chdir("..") < 0) fail();
}

void test_exec(void) {
  int p[2];
  int child;
  int status;
  int outfd;
  char got[5];
  char expect[5];
  char *argv[3];
  char **argvp;

  if (pipe(p) < 0) fail();

  child = fork();
  if (child < 0) fail();
  if (child == 0) {
    if (close(p[0]) < 0) exit(91);
    if (close(1) < 0) exit(92);
    outfd = dup(p[1]);
    if (outfd != 1) exit(93);
    if (close(p[1]) < 0) exit(94);
    argv[0] = echo_prog;
    argv[1] = exec_word;
    argv[2] = 0;
    argvp = argv;
    exec(echo_prog, argvp);
    exit(95);
  }

  if (close(p[1]) < 0) fail();
  if (read_full(p[0], got, 5) != 5) fail();
  if (read(p[0], got, 1) != 0) fail();
  if (close(p[0]) < 0) fail();

  expect[0] = 'E';
  expect[1] = 'X';
  expect[2] = 'E';
  expect[3] = 'C';
  expect[4] = '\n';
  if (!same_bytes(got, expect, 5)) fail();

  status = -1;
  if (wait(&status) != child) fail();
  if (status != 0) fail();
}

void test_string_memory(void) {
  char a[32];
  char b[32];
  char c[32];
  char *p;

  memset(a, 0, 32);
  memset(b, 0, 32);
  memset(c, 0, 32);

  if (strcpy(a, "alpha42") != a) fail();
  if (strlen(a) != 7) fail();
  if (strcmp(a, "alpha42") != 0) fail();
  if (strcmp(a, "alpha43") == 0) fail();

  if (memcpy(b, a, 8) != b) fail();
  if (memcmp(a, b, 8) != 0) fail();

  p = strchr(b, 'h');
  if (p != b + 3) fail();
  if (strchr(b, 'z') != 0) fail();

  memset(c, 'x', 10);
  c[10] = 0;
  if (c[0] != 'x') fail();
  if (c[9] != 'x') fail();

  strcpy(c, "0123456789");
  if (memmove(c + 2, c, 8) != c + 2) fail();
  if (c[2] != '0') fail();
  if (c[3] != '1') fail();
  if (c[9] != '7') fail();

  if (atoi("2048") != 2048) fail();
}

void test_printf_fprintf(void) {
  int pout[2];
  int perr[2];
  int save1;
  int save2;
  char got1[4];
  char got2[4];

  if (pipe(pout) < 0) fail();
  if (pipe(perr) < 0) fail();

  save1 = dup(1);
  save2 = dup(2);
  if (save1 < 0 || save2 < 0) fail();

  if (close(1) < 0) fail();
  if (dup(pout[1]) != 1) fail();
  if (close(2) < 0) fail();
  if (dup(perr[1]) != 2) fail();

  if (close(pout[1]) < 0) fail();
  if (close(perr[1]) < 0) fail();

  printf("P%d%s\n", 7, "Q");
  fprintf(2, "E%d%s\n", 8, "R");

  if (close(1) < 0) fail();
  if (dup(save1) != 1) fail();
  if (close(save1) < 0) fail();
  if (close(2) < 0) fail();
  if (dup(save2) != 2) fail();
  if (close(save2) < 0) fail();

  if (read_full(pout[0], got1, 4) != 4) fail();
  if (read(pout[0], got1, 1) != 0) fail();
  if (close(pout[0]) < 0) fail();

  if (read_full(perr[0], got2, 4) != 4) fail();
  if (read(perr[0], got2, 1) != 0) fail();
  if (close(perr[0]) < 0) fail();

  if (got1[0] != 'P' || got1[1] != '7' || got1[2] != 'Q' || got1[3] != '\n')
    fail();
  if (got2[0] != 'E' || got2[1] != '8' || got2[2] != 'R' || got2[3] != '\n')
    fail();
}

void test_gets(void) {
  int p[2];
  int save0;
  char buf[12];
  char *ret;

  if (pipe(p) < 0) fail();
  if (write(p[1], "line\n", 5) != 5) fail();
  if (close(p[1]) < 0) fail();

  save0 = dup(0);
  if (save0 < 0) fail();
  if (close(0) < 0) fail();
  if (dup(p[0]) != 0) fail();
  if (close(p[0]) < 0) fail();

  ret = gets(buf, 12);

  if (close(0) < 0) fail();
  if (dup(save0) != 0) fail();
  if (close(save0) < 0) fail();

  if (ret != buf) fail();
  if (strcmp(buf, "line\n") != 0) fail();
}

void test_malloc_free(void) {
  char *a;
  char *b;

  a = malloc(32);
  b = malloc(48);
  if (a == 0 || b == 0) fail();
  if (a == b) fail();

  fill_bytes(a, 32, 3);
  fill_bytes(b, 48, 9);

  if (a[0] == 0 || a[31] == 0) fail();
  if (b[0] == 0 || b[47] == 0) fail();
  if (sum_bytes(a, 32) == sum_bytes(b, 32)) fail();

  free(a);
  free(b);
}

int main(void) {
  test_basic_proc();
  test_kill();
  test_pipe_read_write_close_dup();
  test_sbrk_sleep_uptime();
  test_fs_calls();
  test_exec();
  test_string_memory();
  test_printf_fprintf();
  test_gets();
  test_malloc_free();

  write(1, ok_msg, 16);
  exit(0);
  return 0;
}