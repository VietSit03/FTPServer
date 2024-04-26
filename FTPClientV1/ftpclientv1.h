#ifndef FPTCLIENTV1_H_INCLUDED
#define FPTCLIENTV1_H_INCLUDED
#include "tcpclient.h"
#include <vector>

class FTPClient : public TcpClient
{
private:
    string username;
    string password;

public:
    string working_directory;
    string remote_addr;
    int port;
    bool checkLog = false;
public:
    FTPClient();
    ~FTPClient();

    bool openSession(const string &serverHost, unsigned short port = 21);
    void setUsername(const string &user);
    string getUsername();
    void setPassword(const string &pass);
    bool closeSession(); // ket thuc phien va dong ket noi
    bool login();

    bool cwd(const string &path_name);
    bool mkd(const string &path_name);
    string pwd();
    bool epm();

    vector<string> readData(const string &filename);
    bool writeData(const string &filename);
    bool uploadSingeFileToFtpServer(string path_name);
    vector<string> get_all_files_names_within_folder(const string &folder, const string &file_extension);
    vector<string> split(string str, char divider);
    bool listFile();

    bool uploadFile(const string &path_name);
    bool deleteFile(const string &path_name);


};


#endif // FPTCLIENTV1_H_INCLUDED
