#include <unistd.h>
#include <iostream>
#include <fstream>
#include "configuration.h"
#include "utils.h"
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/Object.h>
using namespace std;
using namespace Poco::JSON;

Configuration::Configuration()
{

}

string Configuration::getHostname() const
{
    return hostname;
}

string Configuration::getEnvtype() const
{
    return envtype;
}

string Configuration::getLocation() const
{
    return location;
}

string Configuration::getServerUrl() const
{
    return serverUrl;
}

string Configuration::getServerUsr() const
{
    return serverUsr;
}

string Configuration::getServerPsw() const
{
    return serverPsw;
}

int Configuration::getPeriod() const
{
    return period;
}

bool Configuration::getForcestats() const
{
    return forcestats;
}

Configuration Configuration::loadFromJson(string filename)
{
    ifstream input;
    input.open(filename);
    Parser parser;
    Poco::Dynamic::Var parsedJson = parser.parse(input);
    Object::Ptr root = parsedJson.extract<Object::Ptr>();
    Configuration newconf;
    newconf.envtype = root->get("envtype").convert<std::string>();
    newconf.hostname = root->get("hostname").convert<std::string>();
    newconf.location = root->get("location").convert<std::string>();
    newconf.serverUrl = root->get("serverurl").convert<std::string>();
    newconf.serverUsr = root->get("serverusr").convert<std::string>();
    newconf.serverPsw = root->get("serverpsw").convert<std::string>();
    newconf.period = root->get("frequency").convert<int>();
    newconf.forcestats = root->get("forcestats").convert<bool>();
    newconf.enableServerValidation = root->get("EnableServerValidation").isBoolean()?root->get("EnableServerValidation").convert<bool>() : false;

    if (newconf.hostname == "default") {
        char hostname[64];
        gethostname(hostname, 64);
        newconf.hostname = string(hostname);
    }

    newconf.basedir = getBaseDir();
    return newconf;
}

void Configuration::printConfig() const
{
    cout << getDateAndTime() << " CONFIGURATION: " << endl
         << "   envtype: " << envtype << endl
         << "   hostname: " << hostname << endl
         << "   location: " << location << endl
         << "   serverurl: " << serverUrl << endl
         << "   serverusr: " << serverUsr << endl
         << "   serverpsw: " << serverPsw << endl
         << "   period(==frequency): " << period << endl
         << "   forcestats: " << forcestats << endl
         << "   basedir: " << basedir << endl
         << "   enableServerValidation: " << enableServerValidation << endl;
}

string Configuration::getBasedir() const
{
    return basedir;
}

bool Configuration::getEnableServerValidation() const
{
    return enableServerValidation;
}
