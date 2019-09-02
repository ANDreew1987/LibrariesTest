#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <streambuf>
#include <iomanip>

#include <nlohmann/json.hpp>

#include <rapidjson/document.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>

#include <pugixml.hpp>

#include <tinyxml2.h>

#include "tinyxml.h"

#include "Node.hpp"

using std::ofstream;
using std::ifstream;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::cout;
using std::endl;
using std::cin;
using std::istreambuf_iterator;
using std::setw;

using nlohmann::json;

using rapidjson::Document;
using rapidjson::Value;
using rapidjson::kObjectType;
using rapidjson::kArrayType;
using rapidjson::kStringType;
using rapidjson::IStreamWrapper;
using rapidjson::OStreamWrapper;
using rapidjson::Writer;

using pugi::xml_document;
using pugi::xml_node;
using pugi::xml_attribute;

using tinyxml2::XMLElement;
using tinyxml2::XMLDocument;
using tinyxml2::XMLNode;

void FormatNode(Node *n, json &j)
{
    json j_attributes;
    for (auto &a : n->attributes) {
        j_attributes.emplace(a.first, a.second);
    }
    if (!j_attributes.empty()){
        j[n->name]["attributes"] = j_attributes;
    }
    json j_childs;
    for (auto &ch : n->childNodes) {
        json j_child;
        FormatNode(ch.get(), j_child);
        j_childs.push_back(j_child);
    }
    if (!j_childs.empty()) {
        j[n->name]["nodes"] = j_childs;
    }
}

void FormatNode(Node *n, Value &v, Document::AllocatorType &allocator)
{
    v.SetString(n->name.c_str(), allocator);
    Value j_attrs(kObjectType);
    for (auto &a : n->attributes) {
        j_attrs.AddMember(
            Value().SetString(a.first.c_str(), allocator),
            Value().SetString(a.second.c_str(), allocator),
            allocator);
    }
    if (!n->attributes.empty()) {
        v.SetObject().AddMember("attributes", j_attrs, allocator);
    }
    Value j_nodes(kArrayType);
    for (auto &ch : n->childNodes) {
        Value j_node(kObjectType);
        FormatNode(ch.get(), j_node, allocator);
        j_nodes.PushBack(j_node, allocator);
    }
    if (!n->childNodes.empty()) {
        v.SetObject().AddMember("nodes", j_nodes, allocator);
    }
}

void FormatNode(Node *n, xml_node &x_node)
{
    for (auto &a : n->attributes) {
        xml_attribute atr = x_node.append_attribute(a.first.c_str());
        atr.set_value(a.second.c_str());
    }
    for (auto &ch : n->childNodes) {
        xml_node x_child = x_node.append_child(ch->name.c_str());
        FormatNode(ch.get(), x_child);
    }
}

void FormatNode(Node *n, XMLElement *el, XMLDocument &doc)
{
    for (auto &a : n->attributes) {
        el->SetAttribute(a.first.c_str(), a.second.c_str());
    }
    for (auto &ch : n->childNodes) {
        XMLElement *x_child = doc.NewElement(ch->name.c_str());
        FormatNode(ch.get(), x_child, doc);
        el->InsertEndChild(x_child);
    }
}

void FormatNode(Node *n, TiXmlElement *el)
{
    for (auto &a : n->attributes) {
        el->SetAttribute(a.first.c_str(), a.second.c_str());
    }
    for (auto &ch : n->childNodes) {
        TiXmlElement *x_child = new TiXmlElement(ch->name.c_str());
        FormatNode(ch.get(), x_child);
        el->LinkEndChild(x_child);
    }
}

void Save_NlohmannJSON(Node *n)
{
    json j;
    FormatNode(n, j);
    ofstream o("gen_nlohmann.json");
    const unsigned char bom[] = { 0xEF, 0xBB, 0xBF };
    o.write(reinterpret_cast<const char*>(bom), sizeof(bom));
    //o << j;
    o << setw(4) << j;
}

void Save_RapidJSON(Node *n)
{
    Document doc;
    Document::AllocatorType &allocator = doc.GetAllocator();
    Value root(kObjectType);
    FormatNode(n, root, allocator);
    ofstream o("gen_rapid.json");
    OStreamWrapper osw(o);
    Writer<OStreamWrapper> writer(osw);
    root.Accept(writer);
}

void Save_PugiXML(Node *n)
{
    xml_document doc;
    xml_node x_node = doc.append_child(n->name.c_str());
    FormatNode(n, x_node);
    ofstream o("gen_pugi.xml");
    doc.save(o, "\t", pugi::format_indent | pugi::format_write_bom);
}

void Save_TinyXML2(Node *n)
{
    XMLDocument d;
    XMLElement *el = d.NewElement(n->name.c_str());
    el->SetName(n->name.c_str());
    FormatNode(n, el, d);
    // TODO: Add xml declaration
    d.LinkEndChild(el);
    d.SetBOM(true);
    d.SaveFile("gen_tiny2.xml");
}

void Save_TinyXML(Node *n)
{
    TiXmlElement *el = new TiXmlElement(n->name.c_str());
    FormatNode(n, el);
    TiXmlDocument d;
    d.LinkEndChild(el);
    d.SaveFile("gen_tiny.xml");
}

int main()
{
    {
        cout << "Generating data ..." << endl;
        unique_ptr<Node> mainNode(GenMainNode());
        cout << "Data generated" << endl;
        Save_NlohmannJSON(mainNode.get());
        Save_RapidJSON(mainNode.get());
        Save_PugiXML(mainNode.get());
        Save_TinyXML2(mainNode.get());
        Save_TinyXML(mainNode.get());
    }

    {
        ifstream ifs("gen_nlohmann.json");
        json j;
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        ifs >> j;
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(t2 - t1).count();
        cout << "[FILE] nlohmann parsing time = " << duration << " us" << endl;
    }
    {
        ifstream ifs("gen_nlohmann.json");
        std::string str((std::istreambuf_iterator<char>(ifs)),
            std::istreambuf_iterator<char>());
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        json j = str;
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(t2 - t1).count();
        cout << "[MEMORY] nlohmann parsing time = " << duration << " us" << endl;
    }
    {
        ifstream ifs("gen_rapid.json");
        IStreamWrapper isw(ifs);
        Document doc;
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        doc.ParseStream(isw);
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(t2 - t1).count();
        cout << "[FILE] RapidJSON parsing time = " << duration << " us" << endl;
    }
    {
        ifstream ifs("gen_rapid.json");
        std::string str((std::istreambuf_iterator<char>(ifs)),
            std::istreambuf_iterator<char>());
        Document doc;
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        doc.Parse(str.c_str());
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(t2 - t1).count();
        cout << "[MEMORY] RapidJSON parsing time = " << duration << " us" << endl;
    }
    {
        xml_document doc;
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        doc.load_file("gen_pugi.xml");
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(t2 - t1).count();
        cout << "[FILE] pugixml parsing time = " << duration << " us" << endl;
    }
    {
        ifstream ifs("gen_pugi.xml");
        std::string str((std::istreambuf_iterator<char>(ifs)),
            std::istreambuf_iterator<char>());
        xml_document doc;
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        doc.load_string(str.c_str());
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(t2 - t1).count();
        cout << "[MEMORY] pugixml parsing time = " << duration << " us" << endl;
    }
    {
        XMLDocument doc;
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        doc.LoadFile("gen_tiny2.xml");
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(t2 - t1).count();
        cout << "[FILE] tinyxml2 parsing time = " << duration << " us" << endl;
    }
    {
        ifstream ifs("gen_tiny2.xml");
        std::string str((std::istreambuf_iterator<char>(ifs)),
            std::istreambuf_iterator<char>());
        XMLDocument doc;
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        doc.Parse(str.c_str());
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(t2 - t1).count();
        cout << "[MEMORY] tinyxml2 parsing time = " << duration << " us" << endl;
    }
    {
        TiXmlDocument doc;
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        doc.LoadFile("gen_tiny.xml", TIXML_DEFAULT_ENCODING);
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(t2 - t1).count();
        cout << "[FILE] tinyxml parsing time = " << duration << " us" << endl;
    }
    {
        ifstream ifs("gen_tiny.xml");
        std::string str((std::istreambuf_iterator<char>(ifs)),
            std::istreambuf_iterator<char>());
        TiXmlDocument doc;
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        doc.Parse(str.c_str());
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(t2 - t1).count();
        cout << "[MEMORY] tinyxml parsing time = " << duration << " us" << endl;
    }

    cout << "Press any key to exit ..." << endl;
    cin.get();
	return 0;
}
