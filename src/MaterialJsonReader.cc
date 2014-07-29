#include "MaterialJsonReader.hh"

#include <fstream>
#include <stdexcept>
#include <cmath>
#include <globals.hh>

#include "util/json.hh"

#include "MaterialTemplate.hh"

using namespace g4dicom;
using namespace std;
using namespace g4::util;

template<typename T> T parseAndCheckValue(const Json::Value& value, const std::string& name)
{
    if (!value.isMember(name)) 
    {
        throw std::runtime_error(std::string("JSON does not contain value \"") + name + "\"");
    }
    return readJsonValue<T>(value[name]);
}

void readBaseMaterial(const Json::Value& materialJson, MaterialTemplate* tmpl)
{
    if (materialJson.isMember("g4material"))
    {
        tmpl->g4material = materialJson["g4material"].asString();
    }
    else if (materialJson.isMember("elements") && materialJson["elements"].size())
    {
        Json::Value elementsJson = materialJson["elements"];
        for (int elementIndex = 0; elementIndex < elementsJson.size(); elementIndex++)
        {
            Json::Value elementJson = elementsJson[elementIndex];
            if (elementJson.size() != 2)
            {
                throw runtime_error("Element JSON element must be an array with two values.");
            }
            int z = elementJson[0].asInt();

            double amount = elementJson[1].asDouble();
            if (floor(amount) == amount)
            {
                tmpl->compoundElements[z] = (int)amount;
            }
            else
            {;
                tmpl->mixtureElements[z] = amount;
            }
        }
    }
    else
    {
        throw runtime_error("No material specification in JSON (g4material or non-empty elements required.");
    }
}

void readDensities(const Json::Value& materialJson, MaterialTemplate* tmpl)
{
    if (materialJson.isMember("densities"))
    {
        Json::Value densitiesJson = materialJson["densities"];
        if (!densitiesJson.isArray())
        {
            throw runtime_error("Densities JSON element is not an array.");
        }
        for (int densityIndex = 0; densityIndex < densitiesJson.size(); densityIndex++) 
        {
            Json::Value densityJson = densitiesJson[densityIndex];
            if (!densityJson.isArray() || densityJson.size() != 2)
            {
                throw runtime_error("Density JSON element must be an array with two values.");
            }
            int hu = densityJson[0].asInt();
            double density = densityJson[1].asDouble();
            tmpl->densities[hu] = density;
        }
    }
    else
    {
        double density = parseAndCheckValue<double>(materialJson, "density");
        tmpl->SetUniformDensity(density);
    }    
}

vector<MaterialTemplate*> MaterialJsonReader::LoadTemplates(const std::string& path)
{
    G4cout << "Loading materials from " << path << "..." << G4endl;

    shared_ptr<Json::Value> pRoot = parseJsonFile(path);
    if (!pRoot->isArray())
    {
        throw runtime_error("Root JSON element is not an array.");
    }
    if (!pRoot->size())
    {
        throw runtime_error("Root JSON element is empty.");
    }

    vector<MaterialTemplate*> templates;
    Json::Value& root = *pRoot;
    for (int index = 0; index < root.size(); ++index)
    {
        Json::Value materialJson = root[index];
        MaterialTemplate* tmpl = new MaterialTemplate();

        tmpl->name = parseAndCheckValue<string>(materialJson, "name");
        tmpl->minHU = parseAndCheckValue<int>(materialJson, "minHU");
        tmpl->maxHU = parseAndCheckValue<int>(materialJson, "maxHU");
        
        readBaseMaterial(materialJson, tmpl);
        readDensities(materialJson, tmpl);
        
        // G4cout << "Found material template " << tmpl->name << G4endl;
        templates.push_back(tmpl);
    }
    
    return templates;
}