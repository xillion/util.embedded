#include <iostream>
#include <ncurses.h>

#ifndef UNUSED
#include <Console.h>

#include <fact/MenuFunction.h>

using namespace util;
using namespace std;

void test(IMenu::Parameters p)
{
    cout << "Hi from test!" << endl;
}

void test2(const char* input)
{
  cout << "I got: " << input << endl;
}

void anotherTest(const char* input)
{
  cout << "I really got: " << input << endl;
}

MenuGeneric menuGeneric(test);
CREATE_MENUFUNCTION(menuTest2, test2, "test entry #2");
CREATE_MENUFUNCTION(menuAnotherTest, anotherTest, "test entry #3");

class MainMenu : public Menu
{
public:
  MainMenu()
  {
    add(menuGeneric, F("test"), F("test entry"));
    add(menuTest2);
    add(menuAnotherTest);
  }
};

MainMenu mainMenu;
ConsoleMenu console(&mainMenu);

int main()
{
    cout << "Hi" << endl;

  for(;;)
  {
    /*
    if(Serial.available())
    {
      Serial << "character available: ";
      Serial << (char)Serial.read();
    }*/
    console.handler();
  }
}
#else

using namespace std;

int main()
{
    cout << "Hello world" << std::endl;
    endwin();
}
#endif
