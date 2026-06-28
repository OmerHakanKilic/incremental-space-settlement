#include <ncurses.h>
#include <unistd.h>

int main() {
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  timeout(0);

  int ch;
  int counter = 0;
  while (1) {
    clear();
    ch = getch();
    counter += 1;
    printw("Char: %c Counter: %d", ch, counter);
    refresh();
    usleep(1000000);
  }
  endwin();
  return 0;
}
