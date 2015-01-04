#include <ncurses.h>
#include <cmath>
using std::abs;

class Window
{
  short left_rpm;
  short right_rpm;
public:
  Window()
  {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, true);
    wtimeout(stdscr, 1);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    clear();
  }
  ~Window()
  {
    endwin();
  }
  void showRPM(int line, char ch, short rpm)
  {
      mvprintw(line, 0, "  RPM:");
      mvaddch(line, 0, ch);
      if(rpm > 0) attrset(COLOR_PAIR(2));
      else attrset(COLOR_PAIR(1));
      for(int c = 0; c < abs(rpm) / 400; ++c)
      mvaddch(line, 6 + c, '*');
      attrset(0);
  }
  void setRPM(const short& left, const short& right)
  {
    this->left_rpm = left;
    this->right_rpm = right;
    return;
  }
  void mainLoop()
  {
    char input;
    while(input != 'q')
    {
      input = getch();
      showRPM(10, 'L', this->left_rpm);
      showRPM(11, 'R', this->right_rpm);
      refresh();
    }
    return;
  }
};

int main()
{
  Window win;

  int cnt = 0;
  win.mainLoop();


  return 0;
}
