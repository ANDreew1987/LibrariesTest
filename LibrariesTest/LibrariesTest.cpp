#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <streambuf>
#include <iomanip>

#include <nlohmann/json.hpp>

#include <pugixml.hpp>

#include <tinyxml2.h>

#include "tinyxml.h"

#include <Poco/DOM/Document.h>
#include <Poco/DOM/Element.h>
#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/DOMWriter.h>
#include <Poco/XML/XMLWriter.h>
#include <Poco/DOM/DOMParser.h>
#include <Poco/SAX/InputSource.h>
#include <Poco/XML/NamePool.h>

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

using pugi::xml_document;
using pugi::xml_node;
using pugi::xml_attribute;

using tinyxml2::XMLElement;
using tinyxml2::XMLDocument;
using tinyxml2::XMLNode;

using Poco::XML::Document;
using Poco::XML::Element;
using Poco::XML::AutoPtr;
using Poco::XML::DOMWriter;
using Poco::XML::XMLWriter;
using Poco::XML::DOMParser;
using Poco::XML::InputSource;
using Poco::XML::NamePool;

json FormatNode(Node *n)
{
    json j_atrs;
    for (auto &a : n->attributes) {
        j_atrs.emplace(a.first, a.second);
    }
    json j_childs;
    for (auto &ch : n->childNodes) {
        json j_ch = FormatNode(ch.get());
        j_childs.push_back(j_ch);
    }
    json j_result;
    if (!j_atrs.empty()){
        j_result[n->name].push_back(j_atrs);
    }
    if (!j_childs.empty()) {
        j_result[n->name].push_back(j_childs);
    }
    return j_result;
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

void FormatNode(Node *n, AutoPtr<Element> &el, AutoPtr<Document> &doc)
{
    for (auto &a : n->attributes) {
        el->setAttribute(a.first, a.second);
    }
    for (auto &ch : n->childNodes) {
        AutoPtr<Element> x_child = doc->createElement(ch->name);
        FormatNode(ch.get(), x_child, doc);
        el->appendChild(x_child);
    }
}

void Save_NlohmannJSON(Node *n)
{
    json j = FormatNode(n);
    ofstream o("gen_nlohmann.json");
    const unsigned char bom[] = { 0xEF, 0xBB, 0xBF };
    o.write(reinterpret_cast<const char*>(bom), sizeof(bom));
    //o << j;
    o << setw(4) << j;
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

/*void Save_PocoXML(Node *n)
{
    NamePool *p = new NamePool(100000); // !!!!!??????????
    AutoPtr<Document> doc = new Document(p);
    AutoPtr<Element> root = doc->createElement(n->name.c_str());
    FormatNode(n, root, doc);
    doc->appendChild(root);
    DOMWriter writer;
    writer.setNewLine("\n");
    writer.setOptions(XMLWriter::PRETTY_PRINT | XMLWriter::WRITE_XML_DECLARATION);
    ofstream o("gen_PocoXML.xml");
    writer.writeNode(o, doc);
}*/

int main()
{
    {
        cout << "Generating data ..." << endl;
        unique_ptr<Node> mainNode(GenMainNode());
        cout << "Data generated" << endl;
        Save_NlohmannJSON(mainNode.get());
        Save_PugiXML(mainNode.get());
        Save_TinyXML2(mainNode.get());
        Save_TinyXML(mainNode.get());
        //Save_PocoXML(mainNode.get());
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
    /*{
        ifstream ifs("gen_PocoXML.xml");
        InputSource src(ifs);
        DOMParser parser;
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        AutoPtr<Document> doc = parser.parse(&src);
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(t2 - t1).count();
        cout << "PocoXML parsing time = " << duration << " us" << endl;
    }*/
    /*{
        ifstream ifs("gen_PocoXML.xml");
        std::string str((std::istreambuf_iterator<char>(ifs)),
            std::istreambuf_iterator<char>());
        DOMParser parser;
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        AutoPtr<Document> doc = parser.parseString(str);
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(t2 - t1).count();
        cout << "str PocoXML parsing time = " << duration << " us" << endl;
    }*/

    cout << "Press any key to exit ..." << endl;
    cin.get();
	return 0;
}
