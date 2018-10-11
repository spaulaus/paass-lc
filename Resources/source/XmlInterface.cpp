///@file XmlInterface.cpp
///@brief Singleton class that handles opening and closing XML files using pugixml.
///@author S. V. Paulauskas
///@date February 09, 2017
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <XmlInterface.hpp>


#include "XmlInterface.hpp"

XmlInterface *XmlInterface::instance_ = nullptr;

XmlInterface::XmlInterface(const std::string &file) {
    LoadDocument(file);
}

XmlInterface *XmlInterface::get() {
    if (!instance_)
        instance_ = new XmlInterface("Config.xml");
    return instance_;
}

XmlInterface *XmlInterface::get(const std::string &file) {
    if (!instance_)
        instance_ = new XmlInterface(file);
    else
        instance_->LoadDocument(file);
    return instance_;
}

XmlInterface::~XmlInterface() {
    delete instance_;
    instance_ = nullptr;
}

const pugi::xml_document *XmlInterface::GetDocument() const { return &xmlDocument_; }

void XmlInterface::LoadDocument(const std::string &file) {
    pugi::xml_parse_result result = xmlDocument_.load_file(file.c_str());

    if (!result)
        throw std::invalid_argument("XmlInterface::XmlInterface : We were unable to open a configuration file named \""
            + file + " Received the following from pugixml : " + std::string(result.description()));

    std::cout << "XmlInterface - Successfully loaded \"" << file << "\" into memory." << std::endl;
}
