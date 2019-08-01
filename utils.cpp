#include <unistd.h>
#include <string>
#include <sstream>
#include <iostream>
#include <sys/utsname.h>
#include <fstream>
#include <Poco/RegularExpression.h>
#include "whereami++.hpp"
#include "utils.h"
#include <ctime>
#include <memory>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Object.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/String.h>

using namespace Poco::JSON;
using namespace Poco::Dynamic;

//Find a node with a certain name in a ptree, and exec the update function
void findNodeAndUpdate(Var rootToCheck, string nodeName, function<void (Var)> updateFunction) {
     //Check if a node is terminal.
     if (!rootToCheck.isStruct() && !rootToCheck.isArray() || (rootToCheck.isStruct() && rootToCheck.extract<Object::Ptr>()->size() == 0)
             || (rootToCheck.isArray() && rootToCheck.extract<Poco::JSON::Array::Ptr>()->size() == 0)) {
        return;
     }

     //If object
     if (rootToCheck.isStruct()) {
         //Check all childs of the (current) root
         Object::Ptr rootAsObject = rootToCheck.extract<Object::Ptr>();
         vector<string> keys;
         rootAsObject->getNames(keys);
         for (const string& node : keys) {
             Var val = rootAsObject->get(node);
             if (node == nodeName) {
                 updateFunction(val);
             }
             findNodeAndUpdate(val, nodeName, updateFunction);
         }
     } else if (rootToCheck.isArray()) {
         //Check all childs of the (current) root
         Poco::JSON::Array::Ptr rootAsArray = rootToCheck.extract<Poco::JSON::Array::Ptr>();

         for (const Var& node : *rootAsArray) {
             findNodeAndUpdate(node, nodeName, updateFunction);
         }
     }
}

//Return true if the string toCheck has the prefix prefix
bool hasPrefix(string toCheck, string prefix) {
    auto res = std::mismatch(prefix.begin(), prefix.end(), toCheck.begin());
    return res.first == prefix.end();
}

string execAndGetStdout(string cmd) {
    //Thank https://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c-using-posix    std::array<char, 128> buffer;
    //I don't like this function. I think should be rewritten using boost.process
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
    std::array<char, 128> buffer;
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data();
    }
    return result;
}

string getBaseDir()
{
    return whereami::executable_dir();
}

//Stupid fix! replace certain string to their corresponding boolean value
string searchBooleanAndFix(string in)
{
    in = Poco::replace(in, "\"False\"", "false");
    in = Poco::replace(in,"\"True\"", "true");
    in = Poco::replace(in,"\"false\"", "false");
    in = Poco::replace(in,"\"true\"", "true");
    return in;
}

//Stupid fix! replace certain string to their corresponding float value
string searchFloatAndFix(string in)
{
    Poco::RegularExpression patternFloat("\"([+-]?)([0-9]+)([.,]([0-9]+))\"");
    patternFloat.subst(in, "$1$2.$4");
    Poco::RegularExpression patternInt("\"([+-]?)([0-9]+)\"");
    patternInt.subst(in, "$1$2");

    return in;
}

string getDateAndTime()
{
    time_t now = time(0);
    tm* nowTm = localtime(&now);
    stringstream ss("");
    ss << 1900+nowTm->tm_year << "/"
       << (nowTm->tm_mon<9 ? "0" : "") << 1+nowTm->tm_mon << "/"
       << (nowTm->tm_mday < 10 ? "0" : "") << nowTm->tm_mday << " "
       << (nowTm->tm_hour < 10 ? "0" : "") << nowTm->tm_hour << ":"
       << (nowTm->tm_min < 10 ? "0" : "") << nowTm->tm_min << ":"
       << (nowTm->tm_sec < 10 ? "0" : "") << nowTm->tm_sec;
    return ss.str();
}
