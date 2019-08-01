#ifndef UNMARSHALLER_H
#define UNMARSHALLER_H
#include <string>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Array.h>

using namespace std;
using namespace Poco::JSON;

class Unmarshaller
{
public:
    Unmarshaller();

    static Object::Ptr unmarshalHostInQJsonObject(string in);
    static Poco::JSON::Array::Ptr unmarshalFilesystemInQJsonObject(string in);
    static Object::Ptr unmarshalDatabaseInQJsonObject(string in);
    static Poco::JSON::Array::Ptr unmarshalTablespacesInQJsonObject(string in);
    static Poco::JSON::Array::Ptr unmarshalSchemasInQJsonObject(string in);
    static Poco::JSON::Array::Ptr unmarshalPatchesInQJsonObject(string in);
    static Poco::JSON::Array::Ptr unmarshalFeaturesInQJsonObject(string in);
    static Poco::JSON::Array::Ptr unmarshalLicensesInQJsonObject(string in);


};

#endif // UNMARSHALLER_H
