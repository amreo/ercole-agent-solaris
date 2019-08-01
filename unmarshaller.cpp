#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <Poco/StringTokenizer.h>
#include <Poco/JSON/Array.h>
#include <Poco/Util/XMLConfiguration.h>
#include <Poco/DOM/DOMParser.h>
#include <Poco/DOM/NodeIterator.h>
#include <Poco/DOM/NodeFilter.h>
#include <Poco/DOM/Node.h>
#include <Poco/String.h>
#include "utils.h"
#include "unmarshaller.h"

using namespace std;
using namespace Poco;
using namespace Poco::XML;

Object::Ptr Unmarshaller::unmarshalHostInQJsonObject(string in)
{
    stringstream ss(in);
    std::vector<string> parts;
    string line;
    Object::Ptr hostInfo = new Object();
    while (!ss.eof()) {
        std::getline(ss, line);

        Poco::StringTokenizer parts(line, ":", StringTokenizer::TOK_TRIM);
        if (parts.count() == 2) {
            string key = parts[0];
            string value = parts[1];
            if (key == "cpucores") {
                key = "CPUCores";
            } else if (key == "cpumodel") {
                key = "CPUModel";
            } else if (key == "cputhreads") {
                key = "CPUThreads";
            } else if (key == "os") {
                key = "OS";
            } else if (key == "memorytotal") {
                key = "MemoryTotal";
            } else if (key == "swaptotal") {
                key = "SwapTotal";
            } else if (key == "oraclecluster") {
                key = "OracleCluster";
            } else if (key == "veritascluster") {
                key = "VeritasCluster";
            } else if (key == "suncluster") {
                key = "SunCluster";
            } else if (key[0]>='a' && key[0] <= 'z') {
                key[0] = key[0]+'A'-'a';
            }
            if (value == "N") {
                value = "False";
            } else if (value == "Y") {
                value = "True";
            }
            hostInfo->set(key, value);
        }

    }
    return hostInfo;
}

Poco::JSON::Array::Ptr Unmarshaller::unmarshalFilesystemInQJsonObject(string in)
{
    stringstream ss(in);
    string line;
    vector<string> parts;
    string filesystem;
    string fstype;
    string size;
    string used;
    string available;
    string usedperc;
    string mountedon;
    JSON::Array::Ptr filesystems = new JSON::Array();
    while (!ss.eof()) {
        ss >> filesystem >> fstype >> size >> used >> available >> usedperc >> mountedon;
        Object::Ptr fsInfo = new Object();
        fsInfo->set("Filesystem", filesystem);
        fsInfo->set("FsType", fstype);
        fsInfo->set("Size", size);
        fsInfo->set("Used", used);
        fsInfo->set("Available", available);
        fsInfo->set("UsedPerc", usedperc);
        fsInfo->set("MountedOn", mountedon);
        filesystems->add(fsInfo);
    }
    return filesystems;
}

Object::Ptr Unmarshaller::unmarshalDatabaseInQJsonObject(string in)
{
    Object::Ptr database = new Object();
    //TODO: Fix this algorithm!!!
    in = Poco::replace(in, "<p>", "");
    DOMParser parser;
    AutoPtr<Document> document = parser.parseString(in);
    NodeIterator trIter(document, Poco::XML::NodeFilter::SHOW_ELEMENT);
    Node* currentTrNode;
    while (currentTrNode = trIter.nextNode()) {
        if (currentTrNode->nodeName() != "tr")
            continue;

        NodeIterator tdIter(currentTrNode, Poco::XML::NodeFilter::SHOW_ELEMENT);
        Node* currentTdNode;
        int i=0;
        while (currentTdNode = tdIter.nextNode()) {
            if (currentTdNode->nodeName() != "td")
                continue;

            //assert(node.empty());
            string val = trim(currentTdNode->innerText());
            switch (i) {
                case 0:
                    database->set("Name", val);
                    break;
                case 1:
                    database->set("UniqueName", val);
                    break;
                case 2:
                    database->set("Status", val);
                    break;
                case 3:
                    database->set("Version", val);
                    break;
                case 4:
                    database->set("Platform", val);
                    break;
                case 5:
                    database->set("Archivelog", val);
                    break;
                case 6:
                    database->set("Charset", val);
                    break;
                case 7:
                    database->set("NCharset", val);
                    break;
                case 8:
                    database->set("BlockSize", val);
                    break;
                case 9:
                    database->set("CPUCount", val);
                    break;
                case 10:
                    database->set("SGATarget", val);
                    break;
                case 11:
                    database->set("PGATarget", val);
                    break;
                case 12:
                    database->set("MemoryTarget", val);
                    break;
                case 13:
                    database->set("SGAMaxSize", val);
                    break;
                case 14:
                    database->set("Used", val);
                    break;
                case 15:
                    database->set("Allocated", val);
                    break;
                case 16:
                    database->set("Elapsed", val);
                    break;
                case 17:
                    database->set("DBTime", val);
                    break;
                case 18:
                    database->set("Work", val);
                    break;
                case 19:
                    database->set("ASM", val == "Y" ? true : false);
                    break;
                case 20:
                    database->set("Dataguard", val == "Y" ? true : false);
                    break;
            }
            i++;
        }
    }
    return database;
}

Poco::JSON::Array::Ptr Unmarshaller::unmarshalTablespacesInQJsonObject(string in)
{
    JSON::Array::Ptr tablespaces = new JSON::Array();
    in = Poco::replace(in, "<p>", "");
    DOMParser parser;
    AutoPtr<Document> document = parser.parseString(in);
    NodeIterator trIter(document, Poco::XML::NodeFilter::SHOW_ELEMENT);
    Node* currentTrNode;
    while (currentTrNode = trIter.nextNode()) {
        if (currentTrNode->nodeName() != "tr")
            continue;

        Object::Ptr tablespace = new Object();
        NodeIterator tdIter(currentTrNode, Poco::XML::NodeFilter::SHOW_ELEMENT);
        Node* currentTdNode;
        int i=0;
        while (currentTdNode = tdIter.nextNode()) {
            if (currentTdNode->nodeName() != "td")
                continue;
            string val = trim(currentTdNode->innerText());

            switch (i) {
                case 2:
                    tablespace->set("Database", val);
                    break;
                case 3:
                    tablespace->set("Name", val);
                    break;
                case 4:
                    tablespace->set("Platform", val);
                    break;
                case 5:
                    tablespace->set("Archivelog", val);
                    break;
                case 6:
                    tablespace->set("Charset", val);
                    break;
                case 7:
                    tablespace->set("NCharset", val);
                    break;
                case 8:
                    tablespace->set("Blocksize", val);
                    break;
            }
        }

        tablespaces->add(tablespace);
    }
    return tablespaces;
}

Poco::JSON::Array::Ptr Unmarshaller::unmarshalSchemasInQJsonObject(string in)
{
    JSON::Array::Ptr schemas = new JSON::Array();
    in = Poco::replace(in, "<p>", "");
    DOMParser parser;
    AutoPtr<Document> document = parser.parseString(in);
    NodeIterator trIter(document, Poco::XML::NodeFilter::SHOW_ELEMENT);
    Node* currentTrNode;
    while (currentTrNode = trIter.nextNode()) {
        if (currentTrNode->nodeName() != "tr")
            continue;

        Object::Ptr schema = new Object();
        NodeIterator tdIter(currentTrNode, Poco::XML::NodeFilter::SHOW_ELEMENT);
        Node* currentTdNode;
        int i=0;
        while (currentTdNode = tdIter.nextNode()) {
            string val = trim(currentTdNode->innerText());
            switch (i) {
                case 2:
                    schema->set("Database", val);
                    break;
                case 3:
                    schema->set("User", val);
                    break;
                case 4:
                    schema->set("Total", val);
                    break;
                case 5:
                    schema->set("Tables", val);
                    break;
                case 6:
                    schema->set("Indexes", val);
                    break;
                case 7:
                    schema->set("LOB", val);
                    break;
            }
        }
        schemas->add(schema);
    }
    return schemas;
}

Poco::JSON::Array::Ptr Unmarshaller::unmarshalPatchesInQJsonObject(string in)
{
    JSON::Array::Ptr patches = new JSON::Array();
    in = Poco::replace(in, "<p>", "");
    in = Poco::replace(in, "&nbsp;", " ");

    DOMParser parser;
    AutoPtr<Document> document = parser.parseString(in);
    NodeIterator trIter(document, Poco::XML::NodeFilter::SHOW_ELEMENT);
    Node* currentTrNode;
    while (currentTrNode = trIter.nextNode()) {
        if (currentTrNode->nodeName() != "tr")
            continue;

        Object::Ptr patch = new Object();
        NodeIterator tdIter(currentTrNode, Poco::XML::NodeFilter::SHOW_ELEMENT);
        Node* currentTdNode;
        int i=0;
        while (currentTdNode = tdIter.nextNode()) {
            string val = trim(currentTdNode->innerText());
            switch (i) {
                case 3:
                    patch->set("Database", val);
                    break;
                case 4:
                    patch->set("Version", val);
                    break;
                case 5:
                    patch->set("PatchID", val);
                    break;
                case 6:
                    patch->set("Action", val);
                    break;
                case 7:
                    patch->set("Description", val);
                    break;
                case 8:
                    patch->set("Date", val);
                    break;
            }
        }
        patches->add(patch);
    }
    return patches;
}

Poco::JSON::Array::Ptr Unmarshaller::unmarshalFeaturesInQJsonObject(string in)
{
    stringstream ss(in);
    string line;
    vector<string> parts;
    string featureName;
    string featureStatus;
    JSON::Array::Ptr features = new JSON::Array();
    while (!ss.eof()) {
        Object::Ptr feature = new Object();
        getline(ss, line);
        StringTokenizer parts(line, ":", StringTokenizer::TOK_TRIM);
        if (parts.count() == 2) {
            featureName = parts[0];
            featureStatus = parts[1];
            feature->set("Name", featureName);
            feature->set("Status", featureStatus=="Y");
            features->add(feature);
        }
    }
    return features;
}

Poco::JSON::Array::Ptr Unmarshaller::unmarshalLicensesInQJsonObject(string in)
{
    stringstream ss(in);
    string line;
    vector<string> parts;
    string licenseName;
    string licenseCount;
    JSON::Array::Ptr licenses = new JSON::Array();
    while (!ss.eof()) {
        Object::Ptr license = new JSON::Object();
        getline(ss, line);
        StringTokenizer parts(line, ";", StringTokenizer::TOK_TRIM);
        if (parts.count() == 3) {
            licenseName = parts[0];
            licenseCount = parts[1];

            if (licenseCount.size() == 0) {
                licenseCount = "0";
            }
            license->set("Name", licenseName);
            license->set("Count", licenseCount);
            licenses->add(license);
        }
    }
    return licenses;
}
