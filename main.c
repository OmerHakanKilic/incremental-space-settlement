#include <ncurses.h>
#include <stdbool.h>
#include <unistd.h>

int worker_pop_update(int worker_pop, int birth_rate);
int unemployed_pop_update(int unemployed_pop, int birth_rate);
int amount_of_o2_update(int amount_of_o2, int population,
                        int empty_building_slots);
int amount_of_food_update(int population, int food_production,
                          int amount_of_food);

int main() {
  // Globals
  int worker_population = 100;
  int unemployed_population = 50;
  int birth_rate = 101;
  int empty_building_slots = 100;
  int amount_of_o2 = 500;
  int amount_of_food = 500;
  bool is_paused = false;
  bool is_quit = false;

  // Configs
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  timeout(0);

  // Windows
  WINDOW *population_window = newwin(5, 30, 5, 5);
  WINDOW *survival_window = newwin(5, 30, 10, 5);

  while (!is_quit) {

    // Input Handling
    int ch = getch();
    switch (ch) {
    case ' ':
      is_paused = !is_paused;
      break;
    case 'q':
      is_quit = !is_quit;
      break;
    }

    wclear(population_window);
    wclear(survival_window);
    box(survival_window, 0, 0);
    box(population_window, 0, 0);

    // Update Game Tick
    if (!is_paused) {
      worker_population = worker_pop_update(worker_population, birth_rate);
      unemployed_population =
          unemployed_pop_update(unemployed_population, birth_rate);
      amount_of_o2 = amount_of_o2_update(
          amount_of_o2, (worker_population + unemployed_population),
          empty_building_slots);
      amount_of_food = amount_of_food_update(
          worker_population + unemployed_population,
          worker_population * (empty_building_slots / 10), amount_of_food);
    }

    mvwprintw(population_window, 1, 1, "Workers: %d, Unemployed: %d",
              worker_population, unemployed_population);
    mvwprintw(survival_window, 1, 1, "Amount of O2: %d", amount_of_o2);
    mvwprintw(survival_window, 2, 1, "Amount of Food: %d", amount_of_food);

    wrefresh(survival_window);
    wrefresh(population_window);
    refresh();
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
int unemployed_pop_update(int unemployed_pop, int birth_rate) {
  int result = 0;
  result = (unemployed_pop * (birth_rate + 5)) / 100;
  return result;
}
int amount_of_o2_update(int amount_of_o2, int population,
                        int empty_building_slots) {
  int result = amount_of_o2;
  result += empty_building_slots;
  result -= population;
  return result;
}
int amount_of_food_update(int population, int food_production,
                          int amount_of_food) {
  int result = amount_of_food;
  result += food_production;
  result -= population;
  return result;
}
