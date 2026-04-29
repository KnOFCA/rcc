// Author: KnOFCA

int read(int, void *, int);
int fork(void);
int wait(int *);
int exec(const char *, char **);
void exit(int);
int fprintf(int, const char *, ...);

int run(char **argv, int argc, char *line) {
  int pid;
  int status;
  char *child_argv[32];
  int i;

  if (argc + 2 > 32) {
    fprintf(2, "xargs: too many arguments\n");
    return 1;
  }

  i = 0;
  while (i < argc) {
    child_argv[i] = argv[i];
    i = i + 1;
  }
  child_argv[argc] = line;
  child_argv[argc + 1] = 0;

  pid = fork();
  if (pid < 0) {
    fprintf(2, "xargs: fork failed\n");
    return 1;
  }
  if (pid == 0) {
    exec(argv[0], child_argv);
    fprintf(2, "xargs: exec %s failed\n", argv[0]);
    exit(1);
  }

  wait(&status);
  if (status != 0) {
    return 1;
  }
  return 0;
}

int main(int argc, char **argv) {
  char line[512];
  int m;
  int n;
  char ch;
  int status;

  if (argc < 2) {
    fprintf(2, "usage: xargs command [args ...]\n");
    exit(1);
  }

  m = 0;
  status = 0;
  while (1) {
    n = read(0, &ch, 1);
    if (n < 0) {
      fprintf(2, "xargs: read error\n");
      exit(1);
    }
    if (n == 0) {
      break;
    }
    if (ch == '\n') {
      line[m] = 0;
      if (m > 0) {
        if (run(argv + 1, argc - 1, line) != 0) {
          status = 1;
        }
      }
      m = 0;
      continue;
    }
    if (m + 1 >= 512) {
      fprintf(2, "xargs: input line too long\n");
      exit(1);
    }
    line[m] = ch;
    m = m + 1;
  }

  if (m > 0) {
    line[m] = 0;
    if (run(argv + 1, argc - 1, line) != 0) {
      status = 1;
    }
  }

  exit(status);
  return 0;
}