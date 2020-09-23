#ifndef UTF8_Included
#define UTF8_Included

#include <istream>
#include <ostream>
#include <stdexcept>
#include <string>

/* Given a stream encoded in UTF-8, extracts one character from the stream. If the stream is
 * doesn't contain a proper encoding of a character - including if it's at EOF - this reports
 * an error by throwing a UTFException.
 */
char32_t readChar(std::istream& source);

/* Given a stream encoded in UTF-8, peeks at the next character from the stream. If the stream
 * doesn't contain a proper encoding of a character - including if it's at EOF - this reports
 * an error by throwing a UTFException.
 */
char32_t peekChar(std::istream& source);

/* Given a Unicode character in UTF-32, returns a UTF-8 representation of that character. */
std::string toUTF8(char32_t ch);

/* Given a Unicode character in UTF-32, returns a string representing a series of UTF-16 
 * escape sequences for that character. This string will either have the format
 * \uXXXX (for characters that don't need surrogates) or \uXXXX\uXXXX (for characters that
 * do.)
 */
std::string utf16EscapeFor(char32_t ch);

/* Given a string pointing at an escape sequence of the form \uHHHH (or \uHHHH\uHHHH for a
 * surrogate pair), reads the escape sequence(s) and returns the resulting character. If
 * the stream doesn't contain a sequence formatted this way - including if it's at EOF -
 * this reports an error by throwing a UTFException.
 */
char32_t readUTF16EscapedChar(std::istream& source);

/* Type representing an exception generated during UTF coding. */
class UTFException: public std::logic_error {
public:
    UTFException(const std::string& message);
};

#endif
