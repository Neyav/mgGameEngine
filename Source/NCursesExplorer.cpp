#include <ncurses.h>

int main(void)
{

	initscr();
	noecho();
	curs_set(false);

	endwin();
}
