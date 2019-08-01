#if defined(sun) || defined(__sun)
    #if defined(__SVR4) || defined(__svr4__)
        //I'don't know why, but if I not put this line below, make fail on solaris
        #include <net/if.h>
    #endif
#endif
#include <iostream>
#include <unistd.h>
#include <memory>
#include <vector>
#include <istream>
#include <string>
#include <sstream>
#include <exception>
#include "configuration.h"
#include "utils.h"
#include "unmarshaller.h"
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/AtomicCounter.h>
#include <Poco/URI.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPBasicCredentials.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Array.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/Stringifier.h>

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace Poco::JSON;
using namespace Poco::Dynamic;

void sendData(Configuration conf, Object::Ptr data) {
    cout << getDateAndTime() << " Sending data..." << endl;
    stringstream encodedData;
    Stringifier::stringify(data, encodedData);
    string s = encodedData.str();
    //Change the type of the values
    s = searchBooleanAndFix(s);
    s = searchFloatAndFix(s);
    cout << getDateAndTime() << " Data: " << s << endl;
    //Build the http request
    URI reqUrl(conf.getServerUrl());
    HTTPClientSession client(reqUrl.getHost(), reqUrl.getPort());
    HTTPRequest req;
    HTTPBasicCredentials credentials(conf.getServerUsr(), conf.getServerPsw());
    req.setURI(reqUrl.toString());
    req.setMethod("POST");
    req.setContentType("application/json");
    credentials.authenticate(req);
    req.setContentLength(s.length());

    //Eventually disable certificate validation
    if (conf.getEnableServerValidation() == false) {
//        QSslConfiguration sslConf = QSslConfiguration::defaultConfiguration();
//        sslConf.setPeerVerifyMode(QSslSocket::VerifyNone);
//        QSslConfiguration::setDefaultConfiguration(sslConf);
    }

    //Send the HTTP request
    std::ostream& stream = client.sendRequest(req);
    stream << s;

    HTTPResponse response;

    if (response.getStatus() == HTTPResponse::HTTPStatus::HTTP_OK) {
        cout << getDateAndTime() << " Sending Result: SUCCESS" << endl;
    } else {
        cout << getDateAndTime() << " Sending Result: FAILED" << endl;
        cout << getDateAndTime() << " StatusCode=" << response.getStatus()
             << " StatusMessage=" << response.getReason() << endl;
    }
}

string fetchDBStatus(Configuration conf, string sid, string dbVersion) {
    cout << getDateAndTime() << " Fetching db status: " << sid << endl;
    stringstream cmd;
    cmd << conf.getBasedir() << "/fetch/dbstatus " << sid << " " << dbVersion;
    string out = execAndGetStdout(cmd.str());
    return out;
}

string fetchLicenses(Configuration conf, string sid, string hostType, string dbVersion) {
    cout << getDateAndTime() << " Fetching license: " << sid << endl;
    stringstream cmd;
    cmd << conf.getBasedir() << "/fetch/license " << sid << " " << hostType << " " << dbVersion;
    string out = execAndGetStdout(cmd.str());

    return out;
}

vector<string> loadOratab(Configuration conf) {
    string out = execAndGetStdout(conf.getBasedir().append("/fetch/oratab"));
    vector<string> dbs;

    stringstream bufio(out);
    string line;
    while (!bufio.eof()) {
        getline(bufio, line);
        string trimmedLine = trim(line);
        if (line.size() != 0) {
            dbs.push_back(trimmedLine);
        }
    }

    return dbs;
}

string fetcher(Configuration conf, string fetcherName, string sid, string dbVersion) {
    try {
        cout << getDateAndTime() << " Fetching " <<fetcherName << ": " << sid << endl;

        stringstream cmd;
        cmd << conf.getBasedir() << "/fetch/" << fetcherName << " " << sid << " " << dbVersion;
        string out = execAndGetStdout(cmd.str());
        return out;
    } catch (exception& e) {
        cerr << getDateAndTime() << "ERROR! " << e.what() << endl;
        return "";
    }
}

void buildData(Configuration conf) {
    Object::Ptr data = new Object();
    JSON::Array::Ptr dbsInfo = new JSON::Array();
    Object::Ptr extraInfo = new Object();
    //Get the host data
    Object::Ptr hostInfo = Unmarshaller::unmarshalHostInQJsonObject(fetcher(conf, "host", "", "X"));
    data->set("Hostname", hostInfo->get("Hostname").convert<std::string>());

    //Set the location and envtype values
    data->set("Environment", conf.getEnvtype());
    hostInfo->set("Environment", conf.getEnvtype());
    data->set("Location", conf.getLocation());
    data->set("Info", hostInfo);
    //Fetch the data of the filesystem
    extraInfo->set("Filesystems", Unmarshaller::unmarshalFilesystemInQJsonObject(fetcher(conf, "filesystem", "", "X")));

    //Load the list of dbs
    vector<string> dbs = loadOratab(conf);

    //Set the Databases property
    data->set("Databases", cat(string(" "), dbs.begin(), dbs.end()));

    //Foreach a database build a dbInfo tree and add to dbsInfo
    for(const string& db : dbs) {
        Object::Ptr dbInfo = new Object();

        //Fetch the status of the db
        string dbStatus = fetchDBStatus(conf, db, "X");

        //If it's open, get all properties.
        if (dbStatus == "OPEN") {
            string outVersion = fetcher(conf, "dbversion", db, "X");
            string dbVersion = "12";
            if (hasPrefix(outVersion, "12")) {
                dbVersion = "11";
            } else if (hasPrefix(outVersion, "11")) {
                dbVersion = "10";
            } else if (hasPrefix(outVersion, "9")) {
                dbVersion = "9";
            }

            if (conf.getForcestats()) {
                fetcher(conf, "stats", db, dbVersion);
            }

            dbInfo = Unmarshaller::unmarshalDatabaseInQJsonObject(fetcher(conf, "db", db, dbVersion));
            dbInfo->set("Tablespaces", Unmarshaller::unmarshalTablespacesInQJsonObject(fetcher(conf, "tablespace", db, dbVersion)));
            dbInfo->set("Schemas", Unmarshaller::unmarshalSchemasInQJsonObject(fetcher(conf, "schema", db, dbVersion)));
            dbInfo->set("Patches", Unmarshaller::unmarshalPatchesInQJsonObject(fetcher(conf, "patch", db, dbVersion)));
            dbInfo->set("Features", Unmarshaller::unmarshalFeaturesInQJsonObject(fetcher(conf, "feature", db, dbVersion)));
            dbInfo->set("Licenses", Unmarshaller::unmarshalLicensesInQJsonObject(fetchLicenses(conf, db, hostInfo->get("Type").convert<string>(), dbVersion)));
        } else if (dbStatus == "MOUNTED") {
             dbInfo = Unmarshaller::unmarshalDatabaseInQJsonObject(fetcher(conf, "dbmounted", db, "X"));
        }
        dbsInfo->add(dbInfo);
    }

    vector<string> schemas;
    //Build the schemas list
    findNodeAndUpdate(dbsInfo, "Schemas", [&schemas](Var node){
        for (const Var& item : *node.extract<JSON::Array::Ptr>() ) {
            schemas.push_back(item.extract<Object::Ptr>()->get("User").convert<std::string>());
        }
    });
    data->set("Schemas", cat(string(" "), schemas.begin(), schemas.end()));

    //Add all databases info
    extraInfo->set("Databases", dbsInfo);
    data->set("Extra", extraInfo);

    //Send the data to the server
    sendData(conf, data);
}

Configuration loadConf() {
    string basedir = getBaseDir();
    if (access(string(basedir).append("/config.json").c_str(), F_OK) != -1) {
        return Configuration::loadFromJson(basedir.append("/config.json"));
    } else {
        return Configuration::loadFromJson("/opt/ercole-agent/config.json");
    }
}

int main(int argc, char* argv[])
{
    Configuration conf = loadConf();
    conf.printConfig();

    if (argc == 1) {
        //A too ugly solution
        while (true) {
            buildData(conf);
            //cout << getDateAndTime() << " Waiting " << conf.getPeriod() << " Hours" << endl;
            sleep(conf.getPeriod() * 3600); //frequency == period!!!
        }
    } else {
        buildData(conf);
    }


    return 0;
}
