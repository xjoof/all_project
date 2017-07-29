
// all_project.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <thread>


#define START_ASCII_AGENT(agent_func)   \
    { \
      StartAsciiAgent();\
      for(;;) { \
        if(HasAsciiCmd()) {		\
          agent_func (GetAsciiSrcPtr(), GetAsciiDestPtr());   \
          FinishAsciiCmd();        \
    }}}


static bool HasAsciiAgent = false;
void StartAsciiAgent(){
  HasAsciiAgent = true;
}


static volatile bool hasAsciiCmdFlag = false;
bool HasAsciiCmd() {
  return hasAsciiCmdFlag;
}




static std::string asciiCmd;
const char *GetAsciiSrcPtr() {
  return asciiCmd.c_str();
}


static char cmdResponse[1024];
char *GetAsciiDestPtr() {
  return cmdResponse;
}

void FinishAsciiCmd() {
  hasAsciiCmdFlag = false;
}





void AsciiAgent(const char *cmd, char *res) {

  std::string cmd_str(cmd);
  std::string res_str;
  
  *res = '\0';

  for (int str_i = 0; str_i < cmd_str.size(); ++str_i) {
    res_str.push_back(cmd_str.at(str_i) + 1);
  }
  strcpy_s(res, 1024, res_str.c_str());
}




int main(int argc, char* argv[])
{
  std::thread cmd_input_threa([](){
    for (;;) {
      if (HasAsciiAgent) {
        asciiCmd.clear();
        std::getline(std::cin, asciiCmd);
        hasAsciiCmdFlag = true;

        while (hasAsciiCmdFlag){
        }
        std::cout << GetAsciiDestPtr() << std::endl;
      }
    }
  });


  START_ASCII_AGENT(AsciiAgent);
  return 0;
}

