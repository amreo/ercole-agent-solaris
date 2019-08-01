#ifndef UTILS_H
#define UTILS_H
#include <functional>
#include <Poco/Dynamic/Var.h>

using namespace std;
using namespace Poco::Dynamic;

void findNodeAndUpdate(Var rootToCheck, string nodeName, function<void (Var)> updateFunction);
bool hasPrefix(string toCheck, string prefix);
string execAndGetStdout(string cmd);
string getBaseDir();
string searchBooleanAndFix(string in);
string searchFloatAndFix(string in);
string getDateAndTime();

#endif // UTILS_H
