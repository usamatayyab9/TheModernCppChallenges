/**
 * @file 30_extract_url_parts.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command: g++ -std=c++17 30_extract_url_parts.cpp
 * This file is solution to "Problem 30. Extracting URL parts"
 *  mentioned in "Chapter 3: Strings and Regular Expressions" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *  
 * Problem statement:
 * Write a function that, given a string that represents a URL, parses and extracts the parts of
 * the URL (protocol, domain, port, path, query, and fragment).
 * 
 * Solution:
 * Following items are defined for implementing the solution:
 * - A struct `URLInfo` which representa all parts of a URL i.e. protocol, domain, port etc.
 *      See struct comments for details.
 * - A function Print(const URLInfo &url_info) which prints the URLInfo struct on the console.
 * - A struct `URLInfoExtractor` which provides all the necessary functionality for extracting
 *      URL parts. Regex for each part of URL is defined as private member of struct. The final
 *      regex for a complete URL is obtained by concatenating all regexes together.
 *      A function operator is provided as public member function which takes a string_view as input
 *      and returns all the URL parts found in the input string_view as URLInfo object.
 * 
 * Driver code:
 * The program first initializes a string with a dummy URL. Then using URLInfoExtractor calls it function
 * operator with the dummy URL string. And stores the URL parts into a variable. Then prints the all
 * URL parts extracted.
 * 
 * @copyright Copyright (c) 2023
 */


#include <iostream>
#include <string>
#include <string_view>
#include <regex>
#include <optional>
#include <array>
#include <fstream>
#include <iterator>
#include <charconv>

using std::cin;
using std::cmatch;
using std::cout;
using std::endl;
using std::from_chars;
using std::next;
using std::optional;
using std::ostream_iterator;
using std::regex;
using std::regex_match;
using std::string;
using std::string_view;

/**
 * @brief 
 * protocol :   Mandatory, anything before  ://. As per wikipedia
 *              A non-empty scheme component followed by a colon (:),
 *              consisting of a sequence of characters beginning with a letter and
 *              followed by any combination of letters, digits, plus (+), period (.), or hyphen (-)
 * 
 * domain   :   Mandatory, anything after :// to before : or / 
 * 
 * port     :   optional
 * 
 * path     :   Mandatory, However As per https://www.ibm.com/docs/en/cics-ts/5.2?topic=concepts-components-url,
 *              path is not optional. However it could be empty represented by only '/'.
 * 
 * query    :   optional, A ? followed by series of attributes and values consisting
 *              usually of =,& and alphanumeric characters.
 * 
 * fragemnt :   optional, followed by '#'
 */ 
struct URLInfo
{
    string           protocol   { "N/A" };
    string           domain     { "N/A" };
    optional<int>    port       {  -1   };
    string           path       { "N/A" };
    optional<string> query      { "N/A" };
    optional<string> fragment   { "N/A" };
};

void Print(const URLInfo &url_info)
{
    cout << "protocol : " << url_info.protocol                 << '\n';
    cout << "domain   : " << url_info.domain                   << '\n';
    cout << "port     : " << url_info.port.value_or(-1)        << '\n';
    cout << "path     : " << url_info.path                     << '\n';
    cout << "query    : " << url_info.query.value_or("N/A")    << '\n';
    cout << "fragment : " << url_info.fragment.value_or("N/A") << '\n';
}

struct URLInfoExtractor
{
    private:
    const string kProtocolRegex  = "([[:alpha:]]{1}[[:alnum:]+.-]*)";
    const string kSeparatorRegex = "://";
    const string kDomainRegex    = "([+-.\\w]+)";
    const string kPortRegex      = "(:[[:digit:]]+)?";
    const string kPathRegex      = "(/[_[:alnum:]-/]*)";
    const string kQueryRegex     = "(\\?[\\w=&]*)?" ;
    const string kFragmentRegex  = "(#[[:graph:]]+)?$";
    const string kURLRegex_str   = kProtocolRegex + kSeparatorRegex + kDomainRegex + kPortRegex + kPathRegex + kQueryRegex + kFragmentRegex;
    const regex kURLRegex        = regex{ kURLRegex_str.data() };
    
    public:
    URLInfo operator()(string_view str)
    {
        auto url_info              = URLInfo{};
        cout << "Regex for URL: " << kURLRegex_str << endl;
        if (auto match_info = cmatch{}; regex_match(str.data(), match_info, kURLRegex))
        {
            // cout << "Total sub-matches: " << size(match_info) << '\n';
            // std::copy(cbegin(match_info), cend(match_info), ostream_iterator<decltype(match_info)::value_type>{cout, "\n"});
            
            if (match_info[1].matched) { url_info.protocol = match_info[1]; }
            if (match_info[2].matched) { url_info.domain   = match_info[2]; }
            /*!
                Matched port string will include the colon(:) too.
                Hence port number will start from  match_info[3].first + 1.
            */
            auto port = -1;
            if (match_info[3].matched) { from_chars(next(match_info[3].first), match_info[3].second, port); }
            url_info.port = port;

            if (match_info[4].matched) { url_info.path  = match_info[4]; }
            /*! Matched query string will include '?' too. Similary matched fragment string will include hash(#) too.*/
            if (match_info[5].matched) { url_info.query    = string{ next(match_info[5].first), match_info[5].second }; }
            if (match_info[6].matched) { url_info.fragment = string{ next(match_info[6].first), match_info[6].second }; }

        }
        return url_info;
    }
};

int main()
{
    auto input_str = string{ "https://www.abcd.com:73928/path-1/path-2/path-3?user=usama&pswd=1234#1" };
    cout << "URL string: " << input_str << '\n';

    const auto kURLInfo = URLInfoExtractor{}(input_str);
    Print(kURLInfo);

    return 0;
}