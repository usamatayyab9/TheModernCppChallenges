/**
 * @file 74_selecting_data_xml_xpath.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command: g++ -std=c++17 ./74_selecting_data_xml_xpath.cpp ./pugixml/src/pugixml.cpp
 * 
 *  This file is solution to "Problem 74. Selecting data from XML using XPath"
 *  mentioned in "Chapter 9 : Data Serialization" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 * 
 * Solution :
 * 
 * To solve this problem I also utilized the pugi xml library. Because Qt's support for XPath
 * queries is very confusing hence therfore not used for this solution.
 * 
 * - PrintMovieTitlesAfter Function:
 *      Takes an XML document (pugi::xml_document) and a year as input. Constructs an XPath query
 *      to select movie titles where the release year is greater than the specified year. Executes
 *      the XPath query on the XML document. Prints the titles of the selected movies.
 * - PrintLastActor Function:
 *      Takes an XML document (pugi::xml_document) as input. Constructs an XPath query to select
 *      the last actor and role in the cast of any movie. Executes the XPath query on the XML
 *      document. Prints the information of the last actor and role in the movie cast.
 * 
 * Driver code:
 * - Reads the movies.xml into a xml_document.
 * - Calls PrintMovieTitlesAfter() with generated xml document to print all the movies after 2007.
 * - Calls PrintLastActor() with the generated document to print the names of last actors in all movies.
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include <string_view>
#include <string>
#include <sstream>

#include "./pugixml/src/pugixml.hpp"

using std::cout;
using std::endl;
using std::string;
using std::string_view;

inline constexpr auto kMoviesFileXmlFile = string_view{ "./movies.xml" };

/**
 * @brief Prints the titles of movies released after a specified year.
 * 
 * @param doc The XML document containing movie information.
 * @param year The reference year to filter movies.
 */
void PrintMovieTitlesAfter(const pugi::xml_document &doc, const int &year)
{
    auto sstream = std::stringstream{};
    sstream << "/movies/movie[@year > " <<  year << "]/@title";
    const auto kXPathYearTitleQuery = sstream.str();
    const auto kTitles = doc.select_nodes(kXPathYearTitleQuery.data());
    for (const auto &kTitle : kTitles)
    {
        cout << kTitle.attribute().value() << endl;
    }
}

/**
 * @brief Prints the information of the last actor and role in the movie cast.
 * 
 * @param doc The XML document containing movie information.
 */
void PrintLastActor(const pugi::xml_document &doc)
{
    const auto kXPathYearTitleQuery = std::string_view{ "/movies/movie/cast/role[last()]" };
    const auto kTitles = doc.select_nodes(kXPathYearTitleQuery.data());
    for (const auto &kTitle : kTitles)
    {
        cout << kTitle.node().attribute("actor").value() << " as " << kTitle.node().attribute("role").value() << endl;
    }
}

int main()
{
    auto doc = pugi::xml_document{};
    if (auto result = doc.load_file(kMoviesFileXmlFile.data()); result)
    {
        cout << "XML file loaded successfully" << endl;
        PrintMovieTitlesAfter(doc, 2007); //Print title of all the movies with release year > 2007
        cout << "\n******************************************\n";
        PrintLastActor(doc);
    }
    return 0;
}