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

int O_RDONLY = 0;
int O_RDWR = 2;
int O_CREATE = 512;
int O_TRUNC = 1024;

char ok_msg[] = "RCC_SYSCALL_ALL OK\n";
char fail_msg[] = "RCC_SYSCALL_ALL FAIL\n";

char dir_name[] = "rcc_sys_dir";
char file_name[] = "base.txt";
char link_name[] = "hard.txt";
char node_name[] = "rcc_node";
char exec_word[] = "EXEC";
char echo_prog[] = "echo";

void fail(void) {
  write(1, fail_msg, 21);
  exit(1);
}

int same_bytes(char *a, char *b, int n) {
  int i = 0;
  while (i < n) {
    if (a[i] != b[i]) return 0;
    i = i + 1;
  }
  return 1;
}

int read_full(int fd, char *buf, int want) {
  int got = 0;
  int n;
  while (got < want) {
    n = read(fd, buf + got, want - got);
    if (n <= 0) return -1;
    got = got + n;
  }
  return got;
}

void fill_bytes(char *buf, int n, int seed) {
  int i = 0;
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

void test_pipe_dup_rw(void) {
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

int main() {
  test_basic_proc();
  test_kill();
  test_pipe_dup_rw();
  test_sbrk_sleep_uptime();
  test_fs_calls();
  test_exec();

  write(1, ok_msg, 19);
  exit(0);
  return 0;
}