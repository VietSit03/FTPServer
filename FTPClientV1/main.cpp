#include "ftpcliv1.h"
#include <iostream>

using namespace std;

int main()
{
    cout << "FTP Client V1 - 66mht1 group 4" << endl << endl;
    cout << "open <host> <port>" << endl;
    cout << "help" << endl;
    cout << "quit" << endl << endl;
    FTPClientCLI app;
    app.run();

    return 0;
}
