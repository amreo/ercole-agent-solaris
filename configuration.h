#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include <string>

using namespace std;

class Configuration
{
public:
    Configuration();

    string getHostname() const;
    string getEnvtype() const;
    string getLocation() const;
    string getServerUrl() const;
    string getServerUsr() const;
    string getServerPsw() const;
    int getPeriod() const;
    bool getForcestats() const;
    string getBasedir() const;
    bool getEnableServerValidation() const;

    static Configuration loadFromJson(string filename);

    void printConfig() const;

private:
    string hostname;
    string envtype;
    string location;
    string serverUrl;
    string serverUsr;
    string serverPsw;
    int period;
    bool forcestats;
    string basedir;
    bool enableServerValidation;
};

#endif // CONFIGURATION_H
