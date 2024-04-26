#include "ftpcliv1.h"
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

FTPClientCLI::FTPClientCLI() : CmdLineInterface(cmd_default)
{
    initCmd();
}

void FTPClientCLI::initCmd()
{
    // add code here
    addCmd("open", CLI_CAST(&FTPClientCLI::doOpen));
    addCmd("close", CLI_CAST(&FTPClientCLI::doClose));
    addCmd("help", CLI_CAST(&FTPClientCLI::doHelp));
    addCmd("login", CLI_CAST(&FTPClientCLI::doLogin));
    addCmd("list", CLI_CAST(&FTPClientCLI::doList));
    addCmd("pwd", CLI_CAST(&FTPClientCLI::doPWD));
    addCmd("cwd", CLI_CAST(&FTPClientCLI::doCWD));
    addCmd("mkd", CLI_CAST(&FTPClientCLI::doMKD));
    addCmd("uf", CLI_CAST(&FTPClientCLI::doUf));
    addCmd("df", CLI_CAST(&FTPClientCLI::doDf));

}

void FTPClientCLI::doOpen(string cmd_argv[], int cmd_argc)
{
    if (cmd_argc == 3)
    {
        // add code here
        // cmd_argv[1] : host   cmd_argv[2] : port
        int port = stoi(cmd_argv[2]);
        bool ok = client.openSession(cmd_argv[1], port);

        if (!ok)
        {
            cout << "NOTICE: Connect to server failed." << endl
                 << endl;
            return;
        }
    }
    else if (cmd_argc == 2)
    {
        bool ok = client.openSession(cmd_argv[1], 21);

        if (!ok)
        {
            cout << "NOTICE: Connect to server failed." << endl
                 << endl;
            return;
        }
    }
    else
    {
        cout << "NOTICE: Command without parameters" << endl
             << endl;
        return;
    }
    this->cmd_prompt_default = cmd_argv[1];
    this->setCmdPrompt(this->cmd_prompt_default + cmd_mark_greater);
    cout << "NOTICE: Connect to server succeeed." << endl
         << endl;

}

void FTPClientCLI::doClose(string cmd_argv[], int cmd_argc)
{
    // add code here
    // dong phien
    bool ok = client.closeSession();
    if (ok)
    {
        cout << "NOTICE: Ket thuc phien SMTP thanh cong" << endl;
    }
    else
    {
        cout << "NOTICE: Ket thuc phien SMTP, Server thong bao co loi xay ra" << endl;
    }
    client.close(); // dong localsocket.
    this->setCmdPrompt(cmd_default);
    cout << endl;
}

void FTPClientCLI::doHelp(string cmd_argv[], int cmd_argc)
{
    cout << "Commands may be abbreviated. Commands are:" << endl
         << endl;
    cout << std::setw(30) << std::left << "login";
    cout << std::setw(20) << std::left << "login to ftp server" << std::endl;
    cout << std::setw(30) << std::left << "list";
    cout << std::setw(20) << std::left << "Get list file at current directory" << std::endl;
    cout << std::setw(30) << std::left << "cwd <dir>";
    cout << std::setw(20) << std::left << "Change working directory" << std::endl;
    cout << std::setw(30) << std::left << "cwd .";
    cout << std::setw(20) << std::left << "Return to the previous directory" << std::endl;
    cout << std::setw(30) << std::left << "mkd <dir>";
    cout << std::setw(20) << std::left << "Make directory" << std::endl;
    cout << std::setw(30) << std::left << "uf <file>";
    cout << std::setw(20) << std::left << "Upload file" << std::endl;
    cout << std::setw(30) << std::left << "df <file>";
    cout << std::setw(20) << std::left << "Delete file" << std::endl << endl << endl;

}

void FTPClientCLI::doLogin(string cmd_argv[], int cmd_argc)
{
    string resp;
    if (!client.isConnected()) {
        cout << "NOTICE: not connected!" << endl << endl;
        return;
    }
    else if (cmd_argc == 1)
    {
        // add code here SMTP Protocol
        string username, password;

        cout << "Username:";
        getline(cin, username);
        client.setUsername(username);

        cout << "Password:";
        getline(cin, password);
        client.setPassword(password);

        bool ok = client.login();
        if (!ok)
        {
            cout << "NOTICE: login failed." << endl << endl;
        }
        else
        {
            this->cmd_prompt_default = username + cmd_mark + this->cmd_prompt_default;
            string cmd_prompt = this->cmd_prompt_default + client.working_directory + cmd_mark_greater;
            this->setCmdPrompt(cmd_prompt);
            cout << "NOTICE: login successful!" << endl << endl;
        }
    }
    else
    {
        cout << "NOTICE: Lenh khong dung tham so" << endl
             << endl;
    }
}

void FTPClientCLI::doPWD(string cmd_argv[], int cmd_argc)
{
    if (cmd_argc == 1)
    {
        cout << client.working_directory << endl;
        cout << client.pwd() << endl;
    }
    else
    {
        cout << "NOTICE: Lenh khong dung tham so" << endl;
    }
    cout << endl;
}

void FTPClientCLI::doEpm(string cmd_argv[], int cmd_argc)
{
    if (cmd_argc == 1)
    {
        if (!client.epm())
        {
            cout << "NOTICE: failed passive mode." << endl;
        }
    }
    else
    {
        cout << "NOTICE: Lenh khong dung tham so" << endl;
    }
    cout << endl;
}

void FTPClientCLI::doMKD(string cmd_argv[], int cmd_argc)
{
    if (!client.isConnected() || !client.checkLog)
    {
        cout << "NOTICE: You are not logged in" << endl << endl;
    }
    else
    {
        bool ok = client.mkd(cmd_argv[1]);
        if (!ok)
        {
            cout << "NOTICE: Failed to create new directory" << endl << endl;
        }
        else
        {
            cout << "/" + cmd_argv[1] + " was created successfully!" << endl << endl;
        }
    }
}

void FTPClientCLI::doCWD(string cmd_argv[], int cmd_argc)
{
    if (!client.isConnected() || !client.checkLog)
    {
        cout << "NOTICE: You are not logged in" << endl << endl;
    }
    else
    {
        bool ok = client.cwd(cmd_argv[1]);
        if (!ok)
        {
            cout << "NOTICE: Failed to change working directory" << endl << endl;
        }
        else
        {
            string cmd_prompt = this->cmd_prompt_default + client.working_directory + cmd_mark_greater;
            this->setCmdPrompt(cmd_prompt);
            cout << client.working_directory + " is current directory!" << endl << endl;
        }
    }
}

void FTPClientCLI::doList(string cmd_argv[], int cmd_argc)
{
    bool ok = client.listFile();
    if (!ok)
    {
        cout << "NOTICE: Fail to get list file" << endl;
    }
    else
    {
        //cout << "Success" << endl;
    }
}

void FTPClientCLI::doUf(string cmd_argv[], int cmd_argc)
{
    if (!client.isConnected() || !client.checkLog)          //Chưa mở kết nối || Chưa login
    {
        cout << "NOTICE: You are not logged in" << endl << endl;
    }
    else
    {
        bool ok = client.uploadFile(cmd_argv[1]);
        if (!ok)
        {
            cout << "NOTICE: Failed to upload file" << endl << endl;
        }
        else
        {
            cout << "Uploaded file!" << endl << endl;
        }
    }
}

void FTPClientCLI::doDf(string cmd_argv[], int cmd_argc)
{
    if (!client.isConnected() || !client.checkLog)          //Chưa mở kết nối || Chưa login
    {
        cout << "NOTICE: You are not logged in" << endl << endl;
    }
    else
    {
        bool ok = client.deleteFile(cmd_argv[1]);
        if (!ok)
        {
            cout << "NOTICE: Failed to delete file" << endl << endl;
        }
        else
        {
            cout << "Deleted file!" << endl << endl;
        }
    }
}
