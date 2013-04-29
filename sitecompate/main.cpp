/* 
 * File:   main.cpp
 * Author: wayming
 *
 * Created on 13 April 2013, 4:06 PM
 */

#include <cstdlib>
#include <locale>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/searching/boyer_moore.hpp>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/stream.hpp>

using namespace std;
using namespace boost;
using namespace boost::iostreams;
using namespace algorithm;


int edit_distance_helper(string::const_iterator it1, string::const_iterator it2)
{
    static int count = 1;
    cout << count++ << endl;
    if (*it1 == '\0') {
        int ret = 0;
        while(*it2++ != '\0')
            ret++;
        return ret;
    }
    if (*it2 == '\0') {
        int ret = 0;
        while(*it1++ != '\0')
            ret++;
        return ret;
    }
    int dis0 = (*it1 == *it2) ? 0 : 1;
    int dis1 = edit_distance_helper(it1, it2+1) + 1;
    int dis2 = edit_distance_helper(it1+1, it2) + 1;
    int dis3 = edit_distance_helper(it1+1, it2+1) + dis0;
    
    return min(min(dis1, dis2), dis3);
}

int edit_distance(const string& str1, const string& str2)
{
    return edit_distance_helper(str1.begin(), str2.begin());
}

void pairs(set<string>& v, string s) 
{
    for(string::const_iterator it = s.begin();  it != s.end() && it+1 != s.end(); it++) {
        v.insert(string(it, it+2));
    }
    
}

int similarity(const string& str1, const string& str2)
{
    vector<string> word1V;
    split(word1V, str1, is_space());
    vector<string> word2V;
    split(word2V, str2, is_space());
    
    set<string> paris1S;
    BOOST_FOREACH(string word, word1V) {
        pairs(paris1S, word);
    }
    set<string> paris2S;
    BOOST_FOREACH(string word, word2V) {
        pairs(paris2S, word);
    }

//    BOOST_FOREACH(string p, paris1S) {
//        cout << p << endl;
//    }
//    BOOST_FOREACH(string p, paris2S) {
//        cout << p << endl;
//    }

    set<string> interS;
    set_intersection(
            paris1S.begin(), paris1S.end(), paris2S.begin(), paris2S.end(),
            insert_iterator< set<string> > (interS, interS.begin()));
//    BOOST_FOREACH(string d, interS) {
//        cout << d << endl;
//    }
    return interS.size();

}

string match(const string& s, vector< string >& v)
{
    int most = 0;
    string mstr;
    BOOST_FOREACH( string s0, v) {
        int nsimilar = similarity(s, s0);
        if(nsimilar > most) {
            most = nsimilar;
            mstr = s0;
        }
    }
    cout << s << endl;
    cout << "match" << endl;
    cout << mstr << endl;
    cout << "by " << most << endl;
    return mstr;
}

void load(const string fname, vector< string >& v) {
    ifstream ifs;
    ifs.open(fname.c_str(), ifstream::in);
    while(ifs.good()) {
        string s;
        getline(ifs, s);
        v.push_back(s);
    }
    ifs.close();
}
/*
 * 
 */
int main(int argc, char** argv) {
    vector< string > v1;
    vector< string > v2;
    load(string("amazon"), v1);
    load(string("ebay"), v2);
    BOOST_FOREACH(string s, v1) {
        match(s, v2);
    }
    return 0;
}

