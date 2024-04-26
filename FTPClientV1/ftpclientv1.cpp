#include "ftpclientv1.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <Windows.h>
#include <sstream>

FTPClient::FTPClient()
{
}

FTPClient::~FTPClient()
{
}



void FTPClient::setUsername(const string &name)
{
    this->username = name;
}

string FTPClient::getUsername()
{
    return this->username;
}
void FTPClient::setPassword(const string &pass)
{
    this->password = pass;
}

bool FTPClient::login()
{
    try
    {
        // cac buoc trong giao thuc de xac thuc tai khoan
        // add code here

        string request;
        string response;
        char respBuffer[256];

        // Send USER
        request = "USER " + username + "\r\n";
        sendStringRequest(request);

        // nhan response
        int byte_recv = recvGetLine(respBuffer, 256);
        if (byte_recv > 0)
        {
            response = respBuffer;
            // kiem tra ma phan hoi trong response
            if (response.find("331") != 0)
            {
                return false;
            }
        }
        else
        {
            return false;
        }

        // Send PASSWORD

        request = "PASS " + password + "\r\n";
        sendStringRequest(request);

        // nhan response
        byte_recv = recvGetLine(respBuffer, 256);
        if (byte_recv > 0)
        {
            response = respBuffer;
            // kiem tra ma phan hoi trong response
            if (response.find("230") != 0)
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    catch (SocketException &e)
    {

        return false;
    }
    this->working_directory = "/";
    this->checkLog = true;
    return true;
}

string FTPClient::pwd()
{
    string current_directory = "";
    try
    {
        // cac buoc trong giao thuc tao thu muc
        // add code here

        string request;
        string response;
        char respBuffer[256];

        // Send path name
        request = "PWD\r\n";
        sendStringRequest(request);

        // nhan response
        int byte_recv = recvGetLine(respBuffer, 256);

        if (byte_recv > 0)
        {
            response = respBuffer;
            // kiem tra ma phan hoi trong response
            if (response.find("257") != 0)
            {
                return "";
            }

            return response;
        }
        else
        {
            return "";
        }
    }
    catch (SocketException &e)
    {

        return "";
    }
    return "a";
}

bool FTPClient::epm()
{

    try
    {
        // cac buoc trong giao thuc tao passive mode
        // add code here

        string request;
        string response;
        char respBuffer[256];
        string s = "";
        // Send path name
        request = "PASV" + string("\r\n");
        sendStringRequest(request);

        // nhan response
        int byte_recv = recvGetLine(respBuffer, 256);
        if (byte_recv > 0)
        {
            response = respBuffer;
            // kiem tra ma phan hoi trong response
            if (response.find("227") != 0)
            {
                return false;
            }
            unsigned int ip[4];
            unsigned int p[2];
            sscanf(respBuffer, "227 Entering Passive Mode (%u,%u,%u,%u,%u,%u)", &ip[0], &ip[1], &ip[2], &ip[3], &p[0], &p[1]);
            port = p[0] * 256 + p[1];
        }
        else
        {
            return false;
        }
    }
    catch (SocketException &e)
    {

        return false;
    }

    return true;

}

bool FTPClient::openSession(const string &serverHost, unsigned short port)
{
    string response;
    bool sessionOpen;
    // Mo ket noi den Server
    bool connected = open(serverHost, port);
    if (!connected)
        return false;

    // nhan response trang thai cua Server
    char respBuffer[256];
    try
    {
        int byte_recv = recvDataBuffer(respBuffer, 256);

        if (byte_recv > 2)
        {
            response = respBuffer;
            // print(response);
            if (response.find("220") == 0)
            {
                remote_addr = serverHost;
                sessionOpen = true;
            }
            else
            {
                sessionOpen = false;
            }
        }
        else
        {
            sessionOpen = false;
        }
    }
    catch (SocketException &e)
    {

        return false;
    }

    return sessionOpen;
}

bool FTPClient::closeSession()
{
    bool sessionClose;
    string response;
    try
    {
        sendStringRequest("QUIT\r\n");
        char respBuffer[256];

        // nhan response tu Server
        int byte_recv = recvGetLine(respBuffer, 256);
        if (byte_recv > 0)
        {
            response = respBuffer;
            // kiem tra ma phan hoi trong response
            if (response.find("221") == 0)
            {
                sessionClose = true;
            }
            else
            {
                sessionClose = false;
            }
        }
        else
        {
            sessionClose = false;
        }
    }
    catch (SocketException &e)
    {

        return false;
    }
    return sessionClose;
}

bool FTPClient::cwd(const string &path_name)
{
    string new_working_directory = "";

    // cac buoc trong giao thuc tao thu muc
    // add code here
    if (path_name == ".")
    {
        if (std::count(this->working_directory.begin(), this->working_directory.end(), '/') == 1)
        {
            this->working_directory = "/";
            try
            {
                string request;
                string response;
                char respBuffer[256];

                // Send path name
                request = "CWD " + this->working_directory + "\r\n";
                sendStringRequest(request);

                // nhan response
                int byte_recv = recvGetLine(respBuffer, 256);

                if (byte_recv > 0)
                {
                    response = respBuffer;
                    // kiem tra ma phan hoi trong response
                    if (response.find("250") != 0)
                    {
                        return false;
                    }
                    // print(response);
                    return true;
                }
                else
                {
                    return false;
                }
            }
            catch (SocketException &e)
            {

                return false;
            }
        }
        else
        {
            size_t lastSlashPos = this->working_directory.find_last_of('/');
            this->working_directory = this->working_directory.substr(0, lastSlashPos);
            try
            {
                string request;
                string response;
                char respBuffer[256];

                // Send path name
                request = "CWD " + this->working_directory + "\r\n";
                sendStringRequest(request);

                // nhan response
                int byte_recv = recvGetLine(respBuffer, 256);

                if (byte_recv > 0)
                {
                    response = respBuffer;
                    // kiem tra ma phan hoi trong response
                    if (response.find("250") != 0)
                    {
                        return false;
                    }
                    // print(response);
                    return true;
                }
                else
                {
                    return false;
                }
            }
            catch (SocketException &e)
            {

                return false;
            }
        }
    }
    else
    {
        try
        {
            string request;
            string response;
            char respBuffer[256];

            // Send path name
            request = "CWD " + path_name + "\r\n";
            sendStringRequest(request);

            // nhan response
            int byte_recv = recvGetLine(respBuffer, 256);

            if (byte_recv > 0)
            {
                response = respBuffer;
                // kiem tra ma phan hoi trong response
                if (response.find("250") != 0)
                {
                    return false;
                }
                // Tach new working directory
                // print(response);
                int start_dir = response.find("/");
                int end_dir = response.find(" is") - 4 - response.find(".");
                this->working_directory = response.substr(start_dir, end_dir);
                return true;
            }
            else
            {
                return false;
            }
        }
        catch (SocketException &e)
        {

            return false;
        }
    }

    return true;
}


bool FTPClient::mkd(const string &path_name)
{
    // string new_working_directory = "";
    try
    {
        string request;
        string response;
        char respBuffer[256];

        // Send path name
        request = "MKD " + path_name + "\r\n";
        sendStringRequest(request);

        // nhan response
        int byte_recv = recvGetLine(respBuffer, 256);

        if (byte_recv > 0)
        {
            response = respBuffer;
            // kiem tra ma phan hoi trong response
            if (response.find("257") != 0)
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    catch (SocketException &e)
    {

        return false;
    }
    return true;
}

bool FTPClient::listFile()
{
    try
    {
        string request;
        string response;
        char respBuffer[256];

        // Send path name
        request = "TYPE A\r\n";
        sendStringRequest(request);

        // nhan response
        int byte_recv = recvGetLine(respBuffer, 256);

        if (byte_recv > 0)
        {
            response = respBuffer;
            // kiem tra ma phan hoi trong response
            if (response.find("200") != 0)
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    catch (SocketException &e)
    {

        return false;
    }

    if (epm())
    {
        cout << "Enable passive mode" << endl;
    }

    try
    {
        string request;
        string response;
        char respBuffer[256];

        TcpSocket data_socket;
        data_socket.connect(remote_addr, port);


        // Send path name
        request = "LIST \r\n";
        sendStringRequest(request);

        // nhan response
        int byte_recv = recvGetLine(respBuffer, 256);

        if (byte_recv > 0)
        {
            response = respBuffer;
            // kiem tra ma phan hoi trong response
            if (response.find("150") != 0)
            {
                return false;
            }

            string response1[100];
            int i = 0;
            char respBuffer1[256];
            int bytes;
            do
            {
                bytes = data_socket.recvLine(respBuffer1, sizeof(respBuffer1));
                if(bytes>0) // nhan duoc du lieu neu bytes > 0, neu khong thi error
                {
                    response1[i] = respBuffer1;
                    i++;
                }
            }
            while(bytes>0);
            cout << "List file of directory \"" << working_directory << "\":" << endl;
            for (int j=0; j<i; j++)
            {
                int pos = response1[j].find("\n");
                cout << "       " << response1[j].substr(49, pos-49) << endl;
            }
            return true;
        }
        else
        {
            return false;
        }
    }
    catch (SocketException &e)
    {

        return false;
    }
    return true;
}

bool FTPClient::uploadFile(const string &path_name)
{
    try
    {
        string request;
        string response;
        char respBuffer[256];

        // Send path name
        request = "TYPE A\r\n";
        sendStringRequest(request);

        // nhan response
        int byte_recv = recvGetLine(respBuffer, 256);

        if (byte_recv > 0)
        {
            response = respBuffer;
            // kiem tra ma phan hoi trong response
            if (response.find("200") != 0)
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    catch (SocketException &e)
    {

        return false;
    }

    if (epm())
    {
        cout << "Enable passive mode" << endl;
    }

    try
    {
        string request;
        string response;
        char respBuffer[256];

        TcpSocket data_socket;
        data_socket.connect(remote_addr, port);


        // Send path name
        request = "STOR " + path_name +"\r\n";
        sendStringRequest(request);

        // nhan response
        int byte_recv = recvGetLine(respBuffer, 256);

        if (byte_recv > 0)
        {
            response = respBuffer;
            // kiem tra ma phan hoi trong response
            if (response.find("150") != 0)
            {
                return false;
            }
            return true;
        }
        else
        {
            return false;
        }
    }
    catch (SocketException &e)
    {

        return false;
    }
    return false;
}

bool FTPClient::deleteFile(const string &path_name)
{
    try
    {
        string request;
        string response;
        char respBuffer[256];

        // Send DELE command
        request = "DELE " + path_name + "\r\n";
        sendStringRequest(request);

        // Receive response
        int byte_recv = recvGetLine(respBuffer, 256);
        if (byte_recv > 0)
        {
            response = respBuffer;
            // Check response code
            if (response.find("250") != 0)
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    catch (SocketException &e)
    {
        return false;
    }
    return true;
}
