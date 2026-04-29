// Author: KnOFCA

int read(int, void *, int);
int write(int, const void *, int);
int open(const char *, int);
int close(int);
void exit(int);
int printf(const char *, ...);
int fprintf(int, const char *, ...);
char *strchr(const char *, char);
void *memmove(void *, const void *, int);

char buf[1024];

int match(char *re, char *text);
int matchhere(char *re, char *text);
int matchstar(int c, char *re, char *text);

void grep(char *pattern, int fd, char *name, int show_name) {
  int n;
  int m;
  char *p;
  char *q;

  m = 0;
  while (1) {
    n = read(fd, buf + m, 1024 - m - 1);
    if (n < 0) {
      fprintf(2, "grep: read error\n");
      return;
    }
    if (n == 0) {
      break;
    }
    m = m + n;
    buf[m] = 0;
    p = buf;
    while (1) {
      q = strchr(p, '\n');
      if (q == 0) {
        break;
      }
      *q = 0;
      if (match(pattern, p)) {
        if (show_name) {
          printf("%s:", name);
        }
        *q = '\n';
        write(1, p, q + 1 - p);
      }
      p = q + 1;
    }
    if (p != buf) {
      m = m - (p - buf);
      memmove(buf, p, m);
    }
  }

  if (m > 0) {
    buf[m] = 0;
    if (match(pattern, buf)) {
      if (show_name) {
        printf("%s:", name);
      }
      printf("%s\n", buf);
    }
  }
}

int main(int argc, char **argv) {
  int fd;
  int i;
  int status;
  int show_name;

  if (argc < 2) {
    fprintf(2, "usage: grep pattern [file ...]\n");
    exit(1);
  }

  if (argc < 3) {
    grep(argv[1], 0, "", 0);
    exit(0);
  }

  status = 0;
  show_name = 0;
  if (argc > 3) {
    show_name = 1;
  }

  i = 2;
  while (i < argc) {
    fd = open(argv[i], 0);
    if (fd < 0) {
      fprintf(2, "grep: cannot open %s\n", argv[i]);
      status = 1;
      i = i + 1;
      continue;
    }
    grep(argv[1], fd, argv[i], show_name);
    close(fd);
    i = i + 1;
  }

  exit(status);
  return 0;
}

int match(char *re, char *text) {
  if (re[0] == '^') {
    return matchhere(re + 1, text);
  }
  do {
    if (matchhere(re, text)) {
      return 1;
    }
    if (*text == 0) {
      break;
    }
    text = text + 1;
  } while (1);
  return 0;
}

int matchhere(char *re, char *text) {
  if (re[0] == 0) {
    return 1;
  }
  if (re[1] == '*') {
    return matchstar(re[0], re + 2, text);
  }
  if (re[0] == '$' && re[1] == 0) {
    return *text == 0;
  }
  if (*text != 0 && (re[0] == '.' || re[0] == *text)) {
    return matchhere(re + 1, text + 1);
  }
  return 0;
}

int matchstar(int c, char *re, char *text) {
  do {
    if (matchhere(re, text)) {
      return 1;
    }
    if (*text == 0) {
      break;
    }
    if (*text != c && c != '.') {
      break;
    }
    text = text + 1;
  } while (1);
  return 0;
}