#include <ncurses.h>
#include <stdbool.h>
#include <unistd.h>

int worker_pop_update(int worker_pop, int birth_rate);
int unemployed_pop_update(int unemployed_pop, int birth_rate);

int main() {
  // Globals
  int worker_population = 100;
  int unemployed_population = 0;
  int birth_rate = 101;
  bool is_paused = false;

  // Configs
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  timeout(0);

  // Windows
  WINDOW *population_window = newwin(5, 30, 5, 5);

  int counter = 0;

  while (1) {
    wclear(population_window);
    box(population_window, 0, 0);
    int ch = getch();
    // Update Game Tick
    if (!is_paused) {
      counter += 1;
      worker_population = worker_pop_update(worker_population, birth_rate);
    }
    printw("Char: %c Counter: %d", ch, counter);
    mvwprintw(population_window, 1, 1, "Workers: %d, Unemployed: %d",
              worker_population, unemployed_population);

    wrefresh(population_window);
    refresh();
    switch (ch) {
    case ' ':
      is_paused = !is_paused;
      break;
    }
    usleep(1000000);
  }
  endwin();
  return 0;
}

int worker_pop_update(int worker_pop, int birth_rate) {
  int result = 0;
  result = (worker_pop * birth_rate) / 100;
  return result;
}
