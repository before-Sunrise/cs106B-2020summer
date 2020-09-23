// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include "testing/SimpleTest.h"
#include "map.h"
#include "set.h"
#include <string>
#include <iostream>
#include "filelib.h"
#include <fstream>
#include "simpio.h"
#include "strlib.h"
#include "search.h"
using namespace std;

/* helper function to Trim away leading and trailing punctuation
 * non-word token return empty string
 * All tokens should be converted to lowercase
 */
string cleanToken(string token) {
    int i =0;
    while(ispunct(token[i])){ //trim leadding punctuation
        i++;
    }
    // now token[i] is not punctuation
    token = token.substr(i);
    i = token.length()-1;
    while(ispunct(token[i])){ //trim trailing punctuation
        i--;
    }
    token = token.substr(0,i+1);
    int flag = 0;
    for(char &ch:token){
        if(isalpha(ch)){
            flag++;
        }
        ch = toLowerCase(ch);
    }
    if(!flag){// non-word token( token does not contain at least one letter a-z,A-Z)
        token = {};
    }
    return token;
}

/* given a dbfile, return a Map of
 * <url, Set of corresponding contents' unique token(clean by cleanToken)>
 */
Map<string, Set<string>> readDocs(string dbfile) {
    Map<string, Set<string>> docs;
    ifstream in;

    if (!openFile(in, dbfile))
        error("Cannot open file named " + dbfile);

    Vector<string> lines;
    readEntireFile(in, lines);
    int size = lines.size();
    for(int i = 0; i < size; i += 2){
        string url = lines[i];
        Vector<string> contents = stringSplit(lines[i+1]," "); // stringSplit returns vector whose some elements are ""
        Set<string> contentsInSet;
        for(string &str:contents){
            str = cleanToken(str);
            if(!contentsInSet.contains(str) && !str.empty()){ // avoid to add the empty string
                contentsInSet.add(str);
            }
        }
        docs.add(url,  contentsInSet);
    }

    return docs;
}

/* given docs which is built by readDocs, return a Map
 * from word to a set of URLs where that word can be found
 * this function will invert the map created by readDocs
 */
Map<string, Set<string>> buildIndex(Map<string, Set<string>>& docs) {
    Map<string, Set<string>> index;
    for(string url:docs){
        for(string token:docs[url]){
            //遍历每个url对应的set的每一个token
            index[token].add(url); //use auto-insert
        }
    }
    return index;
}

// help function of findQueryMatches,given a query, return vector of clean token and +/-
Vector<string> processQuery(string query){
    Vector<string> tempedTokens = stringSplit(query, " ");
    Vector<string> token;
    for(string tempedToken:tempedTokens){
        if(tempedToken[0] == '+' || tempedToken[0] == '-'){
            token.add(tempedToken.substr(0,1));
            token.add(cleanToken(tempedToken.substr(1)));
        }else if(!tempedToken.empty()){
            token.add(cleanToken(tempedToken));
        }
    }
    return token;
}

/* given query and index, return the set of webpage(url) that contains  the query
 *
 */
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query) {
    Set<string> result;
    Vector<string> processedQuery = processQuery(query);
    int size = processedQuery.size();
    for(int i = 0; i < size; i++){
        if(processedQuery[i] == "+"){
            Set<string> tokenSearchResult = index[processedQuery[i+1]];
            result *= tokenSearchResult;
            i++; // this branch deal with i and i+1, so next iteratin should be i+2
        }else if(processedQuery[i] == "-"){
            Set<string> tokenSearchResult = index[processedQuery[i+1]];
            result -= tokenSearchResult;
            i++;
        }else{
            Set<string> tokenSearchResult = index[processedQuery[i]];
            result += tokenSearchResult;
        }
    }
    return result;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void searchEngine(string dbfile) {
    Map<string, Set<string>> Docs = readDocs(dbfile);
    Map<string, Set<string>> index = buildIndex(Docs);
    cout << "Stand by while building index.." << endl
         << "indexed " << Docs.keys().size() << " pages containing " << index.keys().size() << "unique terms"
         << endl << endl << endl;
    while(true){
        string querySentence = getLine("Enter query sentence (RETURN/ENTER to quit): ");
        if(querySentence.empty()) break;
        Set<string> result = findQueryMatches(index, querySentence);
        cout << "Found " << result.size() << " matching pages" << endl;
        cout << result << endl << endl << endl;
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on tokens with no punctuation") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
}

PROVIDED_TEST("cleanToken on tokens with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on non-word tokens"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

PROVIDED_TEST("readDocs from tiny.txt, contains 4 documents") {
    Map<string, Set<string>> docs = readDocs("res/tiny.txt");
    EXPECT_EQUAL(docs.size(), 4);
}

PROVIDED_TEST("readDocs from tiny.txt, suess has 5 unique words and includes lowercase fish") {
    Map<string, Set<string>> docs = readDocs("res/tiny.txt");
    Set<string> seuss = docs["www.dr.seuss.net"];
    EXPECT_EQUAL(seuss.size(), 5);
    EXPECT(seuss.contains("fish"));
    EXPECT(!seuss.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 20 unique tokens overall") {
    Map<string, Set<string>> docs = readDocs("res/tiny.txt");
    Map<string, Set<string>> index = buildIndex(docs);
    EXPECT_EQUAL(index.size(), 20);
    EXPECT(index.containsKey("fish"));
    EXPECT(!index.containsKey("@"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> docs = readDocs("res/tiny.txt");
    Map<string, Set<string>> index = buildIndex(docs);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> docs = readDocs("res/tiny.txt");
    Map<string, Set<string>> index = buildIndex(docs);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 3);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}


// TODO: add your test cases here
