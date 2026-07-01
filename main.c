#include <ncurses.h>
#include <stdbool.h>
#include <unistd.h>

typedef struct {
  int amount_of_o2;
  float o2_need;
  int amount_of_food;
  float food_need;
  int worker_population;
  int unemployed_population;
  int birth_rate;
  int empty_building_slots;

} Game;

void worker_pop_update(Game *game);
void unemployed_pop_update(Game *game);
void amount_of_o2_update(Game *game);
void amount_of_food_update(Game *game);
void apply_death(Game *game);

int main() {
  // Globals
  bool is_paused = false;
  bool is_quit = false;
  Game game;
  game.amount_of_food = 500;
  game.amount_of_o2 = 100;
  game.birth_rate = 101;
  game.empty_building_slots = 100;
  game.unemployed_population = 50;
  game.worker_population = 50;

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
      worker_pop_update(&game);

      unemployed_pop_update(&game);

      amount_of_o2_update(&game);

      amount_of_food_update(&game);
      apply_death(&game);
    }

    wprintw(population_window, "Population");
    mvwprintw(population_window, 1, 1, "Workers: %d, Unemployed: %d",
              game.worker_population, game.unemployed_population);
    wprintw(survival_window, "Survival");
    mvwprintw(survival_window, 1, 1, "Amount of O2: %d", game.amount_of_o2);
    mvwprintw(survival_window, 2, 1, "Amount of Food: %d", game.amount_of_food);

    wrefresh(survival_window);
    wrefresh(population_window);
    refresh();
    usleep(1000000);
  }
  endwin();
  return 0;
}

void worker_pop_update(Game *game) {
  game->worker_population = (game->worker_population * game->birth_rate) / 100;
}
void unemployed_pop_update(Game *game) {
  game->unemployed_population =
      (game->unemployed_population * (game->birth_rate + 5)) / 100;
}
void amount_of_o2_update(Game *game) {
  game->amount_of_o2 += game->empty_building_slots;
  game->amount_of_o2 -= game->unemployed_population + game->worker_population;
}
void amount_of_food_update(Game *game) {
  game->amount_of_food +=
      (game->worker_population / 10 + game->worker_population / 10) / 2;
  game->amount_of_food -= game->unemployed_population + game->worker_population;
}
void apply_death(Game *game) {
  if (game->unemployed_population >= 0) {
    if (game->amount_of_food < 0) {
      game->unemployed_population += game->amount_of_food;
    }
    if (game->amount_of_o2 < 0) {
      game->unemployed_population -= game->amount_of_o2;
    }
  } else {
    if (game->amount_of_food < 0) {
      game->worker_population += game->amount_of_food;
    }
    if (game->amount_of_o2 < 0) {
      game->worker_population -= game->amount_of_o2;
    }
  }
}
