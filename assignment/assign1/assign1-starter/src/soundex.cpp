/*
This exercise is all about C++ string processing,
 with a little bit of file reading and use of the Vector data structure.
*/
#include "testing/SimpleTest.h"
#include "strlib.h"
#include "filelib.h"
#include <fstream>
#include <cctype>
#include <string>
#include "simpio.h"
using namespace std;

/* This function takes a string s and returns a version of
 * the string that has all non-letter characters removed
 * from it.
 *
 * WARNING: The provided code is buggy!
 *
 * if string s hava more than one nonletter, then(s = s.substr...)this line of code
 * will be wrong after removing the first nonletter
 */
/*string removeNonLetters(string s) {
    for (int i = 0; i < s.length(); i++) {
        if (!isalpha(s[i])) {
            // remove the character at index i
            s = s.substr(0,i) + s.substr(i+1);
        }
    }
    return s;
}*/

string removeNonLetters(string s){
    int i = 0;
    while(true){
        if(!isalpha(s[i])){
            s = s.substr(0,i) + s.substr(i+1);
        }else{
            i++;
        }
        if(i == s.size()) break;
    }
    return s;
}

char saveFirstLetter(string surname){
    return toUpperCase(surname[0]);
}

// transfer string surname to corresponding integer(still string form)
string stringToDigit(string surname){
    const Vector<string> digits{"AEIOUHWY","BFPV","CGJKQSXZ","DT","L","MN","R"};
    string ans{""};
    for(char ch:surname){
        ch = toUpperCase(ch);
        for(int i = 0; i < digits.size(); i++){
            if(digits[i].find(ch) != string::npos){
                ans += integerToString(i);//have to transfer int to string/char
                break;
            }
        }
    }
    return ans;
}

string coalesceDuplicate(string surname){
    if(surname.size()<2) return surname;//避免string访问越界，出现undifined behavior
    int flag =0;
    while(true){
        // delete the same letter
        if(surname[flag+1] == surname[flag]){
            surname = surname.substr(0,flag+1) + surname.substr(flag+2);
        }else{
            flag++;
        }
        //当flag指向string的最后一个元素时，整个string就没有重复的letter
        if(flag == surname.size()-1){
            break;
        }

    }
    return surname;
}

string replaceFisrtNumber(string number, char first){
    if(number.empty()) return {""};
    number[0] = first;
    return number;
}

string removeAllZeros(string numbers){
    if(numbers.empty()) return {""};
    int i = 0;
    while(true){
        if(numbers[i]  == '0'){
            numbers = numbers.substr(0,i) + numbers.substr(i+1);
        }else{
            i++;
        }
        if(i == numbers.size()) break;
    }
    return numbers;
}

string formattingToLength4(string surname){
    int length = surname.size();
    if(length == 4) return surname;
    else if(length < 4){
        int time = 4 - length;
        for(int i = 0; i < time; i++){
            surname += '0';
        }
    }else{
        surname = surname.substr(0,4);
    }
    return surname;
}
/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
string soundex(string s) {
    s = removeNonLetters(s);
    char first = saveFirstLetter(s);
    s = stringToDigit(s);
    s = coalesceDuplicate(s);
    s = replaceFisrtNumber(s,first);
    s = removeAllZeros(s);
    s = formattingToLength4(s);
    return s;

}


/* if want to run this code, u have to swtich back from running in testing mode to running in main mode
 *  change the SELECTED_TESTS argument back to NO_TESTS in main.cpp and uncomment the call to soundexSearch
 */
void soundexSearch(string filepath) {
    // The proivded code opens the file with the given name 
    // and then reads the lines of that file into a vector.
    ifstream in;
    Vector<string> databaseNames;
    Vector<string> matchingSurname;

    if (openFile(in, filepath)) {
        readEntireFile(in, databaseNames);
    }
    cout << "Read file " << filepath << ", "
         << databaseNames.size() << " names found." << endl;

    while(true){
        string surname = getLine("please enter a surname,enter if want to quit");
        if(surname == "") break;
        string soundexCode = soundex(surname);
        cout << "soundex Code is " << soundexCode << endl;
        for(string name:databaseNames){
            if(soundex(name) == soundexCode){
                matchingSurname.add(name);
            }
        }
        matchingSurname.sort(); //sort in place
        cout << "matching from database :" << matchingSurname << endl;
        matchingSurname.clear();
    }


}


/* * * * * * Test Cases * * * * * */


PROVIDED_TEST("Test removing puntuation, digits, and spaces") {
    string s = "O'Hara";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "tldr");
}


PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Master"), "M236");
    EXPECT_EQUAL(soundex("Jue"), "J000");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

// TODO: add your test cases here
STUDENT_TEST("Test removeNonletters on one nonletter"){
    EXPECT_EQUAL(removeNonLetters("9"),"");
}

STUDENT_TEST("Test removeNonletters on more than one nonletter"){
    EXPECT_EQUAL(removeNonLetters("987 '"),"");
    // the given removeNonLetters return "8 "
}

STUDENT_TEST("Test stringToDigit"){
    EXPECT_EQUAL(stringToDigit("Zelenski"),"20405220");
}

STUDENT_TEST("Test coalesceDuplicate"){
    EXPECT_EQUAL(coalesceDuplicate("222025"),"2025");
    EXPECT_EQUAL(coalesceDuplicate("222222"),"2");
}

STUDENT_TEST("Test  removeAllZeros"){
    EXPECT_EQUAL(removeAllZeros("00000000"),"");
    EXPECT_EQUAL(removeAllZeros("1200300"),"123");
    EXPECT_EQUAL(removeAllZeros("0001200300"),"123");
}

STUDENT_TEST("Test formattingToLength4"){
    EXPECT_EQUAL(formattingToLength4("12345"),"1234");
    EXPECT_EQUAL(formattingToLength4("12"),"1200");
}
