#ifndef FPTCLIV1_H_INCLUDED
#define FPTCLIV1_H_INCLUDED
#include "cli.h"
#include "tcpsocket.h"
#include "ftpclientv1.h"

#define cmd_default "66mht1-group4>"
#define cmd_mark_colon_tilde ":~"
#define cmd_mark_greater ">"
#define cmd_mark "_"

class FTPClientCLI : public CmdLineInterface
{
private:
    FTPClient client;

public:
    string cmd_prompt_default;
    FTPClientCLI();
    void initCmd();

private:
    void doOpen(string cmd_argv[], int cmd_argc);
    void doClose(string cmd_argv[], int cmd_argc);
    void doHelp(string cmd_argv[], int cmd_argc);
    void doLogin(string cmd_argv[], int cmd_argc);

    void doList(string cmd_argv[], int cmd_argc);
    void doCWD(string cmd_argv[], int cmd_argc);
    void doMKD(string cmd_argv[], int cmd_argc);
    void doPWD(string cmd_argv[], int cmd_argc);

    void doEpm(string cmd_argv[], int cmd_argc);

    void doUf(string cmd_argv[], int cmd_argc);
    void doDf(string cmd_argv[], int cmd_argc);



};


#endif // FPTCLIV1_H_INCLUDED
