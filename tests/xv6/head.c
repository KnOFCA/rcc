// Author: KnOFCA

int read(int, char *, int);
int write(int, char *, int);
int open(char *, int);
int close(int);
void exit(int);
int atoi(char *);
int strcmp(char *, char *);
void fprintf(int, char *, ...);

int O_RDONLY = 0;

char usage_msg[] = "usage: head [-n lines] [file]\n";
char open_msg[] = "head: cannot open %s\n";
char read_msg[] = "head: read error\n";
char write_msg[] = "head: write error\n";

int write_all(int fd, char *buf, int n) {
  int off;
  int m;

  off = 0;
  while (off < n) {
    m = write(fd, buf + off, n - off);
    if (m <= 0) {
      return -1;
    }
    off = off + m;
  }
  return 0;
}

int run_head(int fd, int lines) {
  char buf[256];
  int n;
  int i;
  int left;

  left = lines;
  while (left > 0) {
    n = read(fd, buf, 256);
    if (n < 0) {
      fprintf(2, read_msg);
      return 1;
    }
    if (n == 0) {
      return 0;
    }

    i = 0;
    while (i < n) {
      if (write_all(1, buf + i, 1) < 0) {
        fprintf(2, write_msg);
        return 1;
      }
      if (buf[i] == '\n') {
        left = left - 1;
        if (left == 0) {
          return 0;
        }
      }
      i = i + 1;
    }
  }

  return 0;
}

int main(int argc, char **argv) {
  int fd;
  int lines;
  int argi;
  int status;

  lines = 10;
  argi = 1;

  if (argi < argc && strcmp(argv[argi], "-n") == 0) {
    if (argi + 1 >= argc) {
      fprintf(2, usage_msg);
      exit(1);
    }
    lines = atoi(argv[argi + 1]);
    if (lines < 0) {
      lines = 0;
    }
    argi = argi + 2;
  }

  if (argi == argc) {
    status = run_head(0, lines);
    exit(status);
  }

  if (argi + 1 != argc) {
    fprintf(2, usage_msg);
    exit(1);
  }

  fd = open(argv[argi], O_RDONLY);
  if (fd < 0) {
    fprintf(2, open_msg, argv[argi]);
    exit(1);
  }

  status = run_head(fd, lines);
  close(fd);
  exit(status);
  return 0;
}