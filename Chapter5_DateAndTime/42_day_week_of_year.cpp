/**
 * @file 42_day_week_of_year.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 42_day_week_of_year.cpp
 * This file is solution to "Problem 42. Day and week of the year"
 *  mentioned in "Chapter 5: Date and Time" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *  
 * Problem statement:
 * Write a function that, given a date, returns the day of the year
 * (from 1 to 365 or 366 for leap years) and another function that,
 * for the same input, returns the calendar week of the year.
 * 
 * Solution :
 * The actual solution is provided by the functions:
 *  DaysSinceStartOfYear() which calculates the day of the year
 *  for a given date.
 *  WeekNum() which calculates week of the year for a given date.
 * 
 * See function comments for more details.
 * 
 * The other functions in this file WeekCountTestFunction() and
 * WeekNumberTestFunction() provided for testing purposes.
 * 
 * Driver code:
 * The program start by calling above test functions.
 * The initalizes my birthdate and calculates the
 * day of the year and week of the year.
 * 
 * @copyright Copyright (c) 2023
 */
#include <iostream>
#include <array>
#include <utility>
#include <algorithm>
#include <string>
#include <cassert>

#include "date_utility_functions.h"

using std::array;
using std::cout;
using std::endl;
using std::pair;
using std::string;

void WeekCountTestFunction()
{
    /**
     * Test cases built from data available at https://www.epochconverter.com/years 
     * 
    */
    assert(WeekCount(1800) ==	52); assert(WeekCount(1801) ==	53); assert(WeekCount(1802) ==	52); assert(WeekCount(1803) ==	52); assert(WeekCount(1804) ==	52); assert(WeekCount(1805) ==	52); assert(WeekCount(1806) ==	52); assert(WeekCount(1807) ==	53); assert(WeekCount(1808) ==	52); assert(WeekCount(1809) ==	52);
    assert(WeekCount(1810) ==	52); assert(WeekCount(1811) ==	52); assert(WeekCount(1812) ==	53); assert(WeekCount(1813) ==	52); assert(WeekCount(1814) ==	52); assert(WeekCount(1815) ==	52); assert(WeekCount(1816) ==	52); assert(WeekCount(1817) ==	52); assert(WeekCount(1818) ==	53); assert(WeekCount(1819) ==	52);
    assert(WeekCount(1820) ==	52); assert(WeekCount(1821) ==	52); assert(WeekCount(1822) ==	52); assert(WeekCount(1823) ==	52); assert(WeekCount(1824) ==	53); assert(WeekCount(1825) ==	52); assert(WeekCount(1826) ==	52); assert(WeekCount(1827) ==	52); assert(WeekCount(1828) ==	52); assert(WeekCount(1829) ==	53);
    assert(WeekCount(1830) ==	52); assert(WeekCount(1831) ==	52); assert(WeekCount(1832) ==	52); assert(WeekCount(1833) ==	52); assert(WeekCount(1834) ==	52); assert(WeekCount(1835) ==	53); assert(WeekCount(1836) ==	52); assert(WeekCount(1837) ==	52); assert(WeekCount(1838) ==	52); assert(WeekCount(1839) ==	52);
    assert(WeekCount(1840) ==	53); assert(WeekCount(1841) ==	52); assert(WeekCount(1842) ==	52); assert(WeekCount(1843) ==	52); assert(WeekCount(1844) ==	52); assert(WeekCount(1845) ==	52); assert(WeekCount(1846) ==	53); assert(WeekCount(1847) ==	52); assert(WeekCount(1848) ==	52); assert(WeekCount(1849) ==	52);
    assert(WeekCount(1850) ==	52); assert(WeekCount(1851) ==	52); assert(WeekCount(1852) ==	53); assert(WeekCount(1853) ==	52); assert(WeekCount(1854) ==	52); assert(WeekCount(1855) ==	52); assert(WeekCount(1856) ==	52); assert(WeekCount(1857) ==	53); assert(WeekCount(1858) ==	52); assert(WeekCount(1859) ==	52);
    assert(WeekCount(1860) ==	52); assert(WeekCount(1861) ==	52); assert(WeekCount(1862) ==	52); assert(WeekCount(1863) ==	53); assert(WeekCount(1864) ==	52); assert(WeekCount(1865) ==	52); assert(WeekCount(1866) ==	52); assert(WeekCount(1867) ==	52); assert(WeekCount(1868) ==	53); assert(WeekCount(1869) ==	52);
    assert(WeekCount(1870) ==	52); assert(WeekCount(1871) ==	52); assert(WeekCount(1872) ==	52); assert(WeekCount(1873) ==	52); assert(WeekCount(1874) ==	53); assert(WeekCount(1875) ==	52); assert(WeekCount(1876) ==	52); assert(WeekCount(1877) ==	52); assert(WeekCount(1878) ==	52); assert(WeekCount(1879) ==	52);
    assert(WeekCount(1880) ==	53); assert(WeekCount(1881) ==	52); assert(WeekCount(1882) ==	52); assert(WeekCount(1883) ==	52); assert(WeekCount(1884) ==	52); assert(WeekCount(1885) ==	53); assert(WeekCount(1886) ==	52); assert(WeekCount(1887) ==	52); assert(WeekCount(1888) ==	52); assert(WeekCount(1889) ==	52);
    assert(WeekCount(1890) ==	52); assert(WeekCount(1891) ==	53); assert(WeekCount(1892) ==	52); assert(WeekCount(1893) ==	52); assert(WeekCount(1894) ==	52); assert(WeekCount(1895) ==	52); assert(WeekCount(1896) ==	53); assert(WeekCount(1897) ==	52); assert(WeekCount(1898) ==	52); assert(WeekCount(1899) ==	52);
    assert(WeekCount(1900) ==	52); assert(WeekCount(1901) ==	52); assert(WeekCount(1902) ==	52); assert(WeekCount(1903) ==	53); assert(WeekCount(1904) ==	52); assert(WeekCount(1905) ==	52); assert(WeekCount(1906) ==	52); assert(WeekCount(1907) ==	52); assert(WeekCount(1908) ==	53); assert(WeekCount(1909) ==	52);
    assert(WeekCount(1910) ==	52); assert(WeekCount(1911) ==	52); assert(WeekCount(1912) ==	52); assert(WeekCount(1913) ==	52); assert(WeekCount(1914) ==	53); assert(WeekCount(1915) ==	52); assert(WeekCount(1916) ==	52); assert(WeekCount(1917) ==	52); assert(WeekCount(1918) ==	52); assert(WeekCount(1919) ==	52);
    assert(WeekCount(1920) ==	53); assert(WeekCount(1921) ==	52); assert(WeekCount(1922) ==	52); assert(WeekCount(1923) ==	52); assert(WeekCount(1924) ==	52); assert(WeekCount(1925) ==	53); assert(WeekCount(1926) ==	52); assert(WeekCount(1927) ==	52); assert(WeekCount(1928) ==	52); assert(WeekCount(1929) ==	52);
    assert(WeekCount(1930) ==	52); assert(WeekCount(1931) ==	53); assert(WeekCount(1932) ==	52); assert(WeekCount(1933) ==	52); assert(WeekCount(1934) ==	52); assert(WeekCount(1935) ==	52); assert(WeekCount(1936) ==	53); assert(WeekCount(1937) ==	52); assert(WeekCount(1938) ==	52); assert(WeekCount(1939) ==	52);
    assert(WeekCount(1940) ==	52); assert(WeekCount(1941) ==	52); assert(WeekCount(1942) ==	53); assert(WeekCount(1943) ==	52); assert(WeekCount(1944) ==	52); assert(WeekCount(1945) ==	52); assert(WeekCount(1946) ==	52); assert(WeekCount(1947) ==	52); assert(WeekCount(1948) ==	53); assert(WeekCount(1949) ==	52);
    assert(WeekCount(1950) ==	52); assert(WeekCount(1951) ==	52); assert(WeekCount(1952) ==	52); assert(WeekCount(1953) ==	53); assert(WeekCount(1954) ==	52); assert(WeekCount(1955) ==	52); assert(WeekCount(1956) ==	52); assert(WeekCount(1957) ==	52); assert(WeekCount(1958) ==	52); assert(WeekCount(1959) ==	53);
    assert(WeekCount(1960) ==	52); assert(WeekCount(1961) ==	52); assert(WeekCount(1962) ==	52); assert(WeekCount(1963) ==	52); assert(WeekCount(1964) ==	53); assert(WeekCount(1965) ==	52); assert(WeekCount(1966) ==	52); assert(WeekCount(1967) ==	52); assert(WeekCount(1968) ==	52); assert(WeekCount(1969) ==	52);
    assert(WeekCount(1970) ==	53); assert(WeekCount(1971) ==	52); assert(WeekCount(1972) ==	52); assert(WeekCount(1973) ==	52); assert(WeekCount(1974) ==	52); assert(WeekCount(1975) ==	52); assert(WeekCount(1976) ==	53); assert(WeekCount(1977) ==	52); assert(WeekCount(1978) ==	52); assert(WeekCount(1979) ==	52);
    assert(WeekCount(1980) ==	52); assert(WeekCount(1981) ==	53); assert(WeekCount(1982) ==	52); assert(WeekCount(1983) ==	52); assert(WeekCount(1984) ==	52); assert(WeekCount(1985) ==	52); assert(WeekCount(1986) ==	52); assert(WeekCount(1987) ==	53); assert(WeekCount(1988) ==	52); assert(WeekCount(1989) ==	52);
    assert(WeekCount(1990) ==	52); assert(WeekCount(1991) ==	52); assert(WeekCount(1992) ==	53); assert(WeekCount(1993) ==	52); assert(WeekCount(1994) ==	52); assert(WeekCount(1995) ==	52); assert(WeekCount(1996) ==	52); assert(WeekCount(1997) ==	52); assert(WeekCount(1998) ==	53); assert(WeekCount(1999) ==	52);
    assert(WeekCount(2000) ==	52); assert(WeekCount(2001) ==	52); assert(WeekCount(2002) ==	52); assert(WeekCount(2003) ==	52); assert(WeekCount(2004) ==	53); assert(WeekCount(2005) ==	52); assert(WeekCount(2006) ==	52); assert(WeekCount(2007) ==	52); assert(WeekCount(2008) ==	52); assert(WeekCount(2009) ==	53);
    assert(WeekCount(2010) ==	52); assert(WeekCount(2011) ==	52); assert(WeekCount(2012) ==	52); assert(WeekCount(2013) ==	52); assert(WeekCount(2014) ==	52); assert(WeekCount(2015) ==	53); assert(WeekCount(2016) ==	52); assert(WeekCount(2017) ==	52); assert(WeekCount(2018) ==	52); assert(WeekCount(2019) ==	52);
    assert(WeekCount(2020) ==	53); assert(WeekCount(2021) ==	52); assert(WeekCount(2022) ==	52); assert(WeekCount(2023) ==	52); assert(WeekCount(2024) ==	52); assert(WeekCount(2025) ==	52); assert(WeekCount(2026) ==	53); assert(WeekCount(2027) ==	52); assert(WeekCount(2028) ==	52); assert(WeekCount(2029) ==	52);
    assert(WeekCount(2030) ==	52); assert(WeekCount(2031) ==	52); assert(WeekCount(2032) ==	53); assert(WeekCount(2033) ==	52); assert(WeekCount(2034) ==	52); assert(WeekCount(2035) ==	52); assert(WeekCount(2036) ==	52); assert(WeekCount(2037) ==	53); assert(WeekCount(2038) ==	52); assert(WeekCount(2039) ==	52);
    assert(WeekCount(2040) ==	52); assert(WeekCount(2041) ==	52); assert(WeekCount(2042) ==	52); assert(WeekCount(2043) ==	53); assert(WeekCount(2044) ==	52); assert(WeekCount(2045) ==	52); assert(WeekCount(2046) ==	52); assert(WeekCount(2047) ==	52); assert(WeekCount(2048) ==	53); assert(WeekCount(2049) ==	52);
    assert(WeekCount(2050) ==	52); assert(WeekCount(2051) ==	52); assert(WeekCount(2052) ==	52); assert(WeekCount(2053) ==	52); assert(WeekCount(2054) ==	53); assert(WeekCount(2055) ==	52); assert(WeekCount(2056) ==	52); assert(WeekCount(2057) ==	52); assert(WeekCount(2058) ==	52); assert(WeekCount(2059) ==	52);
    assert(WeekCount(2060) ==	53); assert(WeekCount(2061) ==	52); assert(WeekCount(2062) ==	52); assert(WeekCount(2063) ==	52); assert(WeekCount(2064) ==	52); assert(WeekCount(2065) ==	53); assert(WeekCount(2066) ==	52); assert(WeekCount(2067) ==	52); assert(WeekCount(2068) ==	52); assert(WeekCount(2069) ==	52);
    assert(WeekCount(2070) ==	52); assert(WeekCount(2071) ==	53); assert(WeekCount(2072) ==	52); assert(WeekCount(2073) ==	52); assert(WeekCount(2074) ==	52); assert(WeekCount(2075) ==	52); assert(WeekCount(2076) ==	53); assert(WeekCount(2077) ==	52); assert(WeekCount(2078) ==	52); assert(WeekCount(2079) ==	52);
    assert(WeekCount(2080) ==	52); assert(WeekCount(2081) ==	52); assert(WeekCount(2082) ==	53); assert(WeekCount(2083) ==	52); assert(WeekCount(2084) ==	52); assert(WeekCount(2085) ==	52); assert(WeekCount(2086) ==	52); assert(WeekCount(2087) ==	52); assert(WeekCount(2088) ==	53); assert(WeekCount(2089) ==	52);
    assert(WeekCount(2090) ==	52); assert(WeekCount(2091) ==	52); assert(WeekCount(2092) ==	52); assert(WeekCount(2093) ==	53); assert(WeekCount(2094) ==	52); assert(WeekCount(2095) ==	52); assert(WeekCount(2096) ==	52); assert(WeekCount(2097) ==	52); assert(WeekCount(2098) ==	52); assert(WeekCount(2099) ==	53);
    assert(WeekCount(2100) ==	52);
}

void WeekNumberTestFunction()
{
    assert(WeekNum(Date{ 23, Month::kMarch, 2023 }) == 12);
    assert(WeekNum(Date{ 17, Month::kJune, 1980 }) == 25);
    assert(WeekNum(Date{ 25, Month::kSept, 1800 }) == 39);
    assert(WeekNum(Date{ 25, Month::kSept, 1607 }) == 38); // Test case fails result should be 38
}

int main()
{
    WeekCountTestFunction();
    cout << "All test cases passed for calculating number of weeks in a year" << endl;

    WeekNumberTestFunction();
    cout << "All test cases passed for calculating week index of a date" << endl;

    constexpr auto kMyBirthDate = Date{ 18, Month::kMarch, 1997 };
    cout << "I was born on " << DaysSinceStartOfYear(kMyBirthDate) << "th day of the year" << endl;
    cout << "I was born in " << WeekNum(kMyBirthDate) << "th week of the year" << endl;
    
    return 0;
}