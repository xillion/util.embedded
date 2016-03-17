#include "Console.h"
#include "fact/Menu.h"
//#include "Service.h"

using namespace FactUtilEmbedded;


#ifdef MEMORY_OPT_CODE
#define ERROR_UNRECOGNIZED_COMMAND "Unrecognized: "
#define INFO_AVAILABLE_COMMAND "Available: "
#else
#define ERROR_UNRECOGNIZED_COMMAND "Unrecognized command: "
#define INFO_AVAILABLE_COMMAND "Following commands are available: "
#endif



#if SAMD_SERIES
// samd is supposed to have a strncmp_P mapping to regular strncmp,
// but doesn't for some reason so do it here
#define strncmp_P(s1, s2, n) strncmp((s1), (s2), (n))
#endif


MenuCommand* Menu::canHandle(Parameters p)
{
#ifdef DEBUG2
  cout.println("Menu::canHandle entry");
#endif

  return MenuEnumerator::canHandle(p);
}

void Menu::handleCommand(Parameters p)
{
#ifdef DEBUG2
  cout.println("Menu::handle command");
#endif

  MenuCommand* menu = canHandle(p);

#ifdef DEBUG2
  cout.println("Menu::handle command 2");
#endif

  if(menu != NULL)
    menu->handleCommand(p.inc());
  else if(strcmp_P(*p.parameters, PSTR("help")) == 0)
  {
    showHelp(p.inc());
  }
  else
  {
    cout.println();
    cout << F(ERROR_UNRECOGNIZED_COMMAND) << *p.parameters;
    cout.println();
  }
}


void Menu::showHelp(Parameters p)
{
  if(p.count == 0)
  {
    SinglyLinkedNode* node = getHeadMenu();

    cout.println();

#ifndef MEMORY_OPT_CODE
    if(node == NULL)
    {
      cout << F("No commands registered");
      return;
    }
#endif

    cout << F(INFO_AVAILABLE_COMMAND);

    cout.println();
    cout.println();

    for(; node != NULL; node = node->getNext())
    {
      auto menu = (MenuBase*) node;

      cout << F("  ");

      const __FlashStringHelper* name = menu->getName();
      const __FlashStringHelper* description = menu->getDescription();

#ifdef DEBUG
      if(name == NULL) { cout.println(F("Name=NULL")); continue; }
      if(description == NULL) { cout.println(F("Desc=NULL")); continue; }
#endif
      showKeyValuePair(name, description, 16);
      cout.println();
    }
  }
}



#if defined(CONSOLE_FEATURE_AUTOCOMPLETE)
bool Menu::processInput(Console* console, char received)
{
  // look for tab character
  if(received == 9)
  {
    char* inputLine = console->getInputLine();
    uint8_t inputPos = console->getInputPos();

    SinglyLinkedNode* node = getHeadMenu();
    for(; node != NULL; node = node->getNext())
    {
      auto menu = (MenuBase*) node;
      auto commandName = (PGM_P) menu->getName();
      if(strncmp_P(inputLine, commandName, inputPos) == 0)
      {
        cout << (const __FlashStringHelper*)(commandName + inputPos);
        console->appendToInputLine_P(commandName + inputPos);
      }
    }
    return true;
  }
  else
    return false;
}
#endif


void MenuCommand::showPrompt()
{
#ifdef DEBUG3
  cout.println(F("Menu::showPrompt"));
#endif

  if(getName() != NULL) cout << getName();

#ifdef DEBUG3
  cout.println(F("Menu::showPrompt exit"));
#endif
}


IMenu* MenuCommand::canHandle(IMenu::Parameters p)
{
  const __FlashStringHelper* name = getName();

#ifdef DEBUG2
  cout.println(F("Menu::canHandle"));
  cout << F("evaluating ") << p.parameters[0] << F(" against name: ") << name;
  cout.println();
#endif
#if defined(DEBUG3) && defined(ESP8266)
  static int __result;
  PGM_P p2 = reinterpret_cast<PGM_P>(name);
  cout << F("ptrs. orig=") << (uint32_t) name << F(" converted: ") << (uint32_t) p2;
  cout << F(" ptrs. param=") << (uint32_t) p.parameters[0];
  cout.println();
  const __FlashStringHelper* _test1 = F("status2");
  char* _test2 = "abc";
  cout << F("phase 1:");
  auto _result = strcmp_P(_test2, (PGM_P)_test1);
  cout << _result << F(",phase 2:");
  _result = strncmp_P_dbg(_test2, (PGM_P)name, 3);
  cout << _result << F(",phase 3:");
  _result = strcmp_P_dbg(_test2, (PGM_P)name);
  cout << _result << F(",phase 4:");
  _result = strcmp_P_dbg(p.parameters[0], (PGM_P) name);
  cout << _result << F(",phase 5:");
  cout.println(F("Done testing"));
#endif


  if(strcmp_P(p.parameters[0], (PGM_P) name) == 0)
  {
#ifdef DEBUG3
    cout << F("Exit this");
#endif
    return this;
  }
  else
  {
#ifdef DEBUG3
    cout << F("Exit NULL");
#endif
    return NULL;
  }
}

void MenuGeneric::handleCommand(IMenu::Parameters p)
{
#ifdef DEBUG2
  cout.println("MenuGeneric::handleCommand");
#endif

  handler(p);
}

// TODO: Switch this to PGM_P
void IMenu::_showKeyValuePair(const __FlashStringHelper* key, uint8_t keyPadding)
{
  // FIX: some Print classes don't seem to return proper bytes-written
  size_t nameLength = strlen_P((const char*) key);
  //size_t nameLength = cout.print(menu->getName());
  cout.print(key);
  if(keyPadding > nameLength)
    keyPadding -= nameLength;

  while(keyPadding-- > 0) cout.print(' ');
}

void NestedMenuHandler::handleCommand(Parameters p)
{
  if(strcmp_P(*p.parameters, PSTR("cd")) == 0)
  {
    IMenu* handleAble = canHandle(p.inc());

    if(handleAble)
    {
      selected = handleAble;
    }
  }
}

void NestedMenuHandler::showPrompt()
{
  // TODO: We can turn getName into showLocalPrompt if we want to go even more OOP
  cout << getName();
  if(getSelected())
  {
    cout << ' ';
    getSelected()->showPrompt();
  }
  else
  {
    cout << '>';
  }
}
