#include "Unicode.h"
#include <sstream>
#include <iomanip>
#include <cctype>
using namespace std;

namespace {
    /* Reports a UTF error. */
    [[ noreturn ]] void utfError(const string& message) {
        throw UTFException(message);
    }

    /* Gets the next raw character from a stream, reporting an error() if unable to do so. */
    char get(istream& input) {
        char result;
        if (!input.get(result)) utfError("Unexpected end of stream.");

        return result;
    }
    
    /* Returns whether a given byte is a follow byte (that is, a byte starting with 10). */
    bool isFollowByte(char byte) {
        return (byte & 0b11000000) == 0b10000000;
    }
    
    /* Converts a given character to a hex value. */
    string toHex(char ch) {
        ostringstream builder;
        builder << hex << setw(2) << setfill('0') << static_cast<unsigned int>(static_cast<unsigned char>(ch));
        return builder.str();
    }
    
    /* Given a stream encoded in UTF-8, reads the bytes that compose a single Unicode
     * character.
     */
    string nextBytesFrom(istream& source) {
        char header = get(source);
        string result(1, header);
        
        /* If this character doesn't have a high bit set, that's all there is to read. */
        if ((header & 0b10000000) == 0) return result;
        
        /* Otherwise, see how many characters there are to read. */
        size_t followBytes = 0;
        if      ((header & 0b11100000) == 0b11000000) followBytes = 1;
        else if ((header & 0b11110000) == 0b11100000) followBytes = 2;
        else if ((header & 0b11111000) == 0b11110000) followBytes = 3;
        else utfError("Byte header doesn't match UTF-8 patterns.");
        
        for (size_t i = 0; i < followBytes; i++) {
            char next = get(source);
            if (!isFollowByte(next)) utfError("Expected follow byte, got " + toHex(next));
            
            result += next;
        }
        
        return result;        
    }
    
    /* Given a string representing the bytes of a UTF-8 encoded character, decodes those
     * bytes into a single character.
     */
    char32_t decode(const string& bytes) {
        if (bytes.empty()) utfError("Empty byte string?");
        
        /* If the first byte starts with a zero bit, we just return it as-is. */
        if ((bytes[0] & 0b10000000) == 0) {
            if (bytes.size() != 1) utfError("Wrong number of bytes for 7-bit code point.");
            
            return bytes[0];
        }
        
        /* If the first byte begins with 110xxxxx, then we need to decode an 11-bit number
         * of the form 110bbbbb 10bbbbbb
         */
        if ((bytes[0] & 0b11100000) == 0b11000000) {
            if (bytes.size() != 2) utfError("Wrong number of bytes for 11-bit code point.");
            if (!isFollowByte(bytes[1])) utfError("Paired byte has wrong header.");
            
            return ((bytes[0] & 0b00011111) << 6) +
                   ((bytes[1] & 0b00111111) << 0);
        }
        
        /* If the first byte begins with 1110xxxx, then we need to decode a 16-bit number
         * of the form 1110bbbb 10bbbbbb 10bbbbbb.
         */
        if ((bytes[0] & 0b11110000) == 0b11100000) {
            if (bytes.size() != 3) utfError("Wrong number of bytes for 16-bit code point.");
            if (!isFollowByte(bytes[1])) utfError("First paired byte has wrong header.");
            if (!isFollowByte(bytes[2])) utfError("Second paired byte has wrong header.");
            
            return ((bytes[0] & 0b00001111) << 12) + 
                   ((bytes[1] & 0b00111111) <<  6) +
                   ((bytes[2] & 0b00111111) <<  0);
        }
        
        /* If the first byte begins with 11110xxx, then we need to decode a 21-bit number
         * of the form 11110bbb 10bbbbbb 10bbbbbb 10bbbbbb.
         */
        if ((bytes[0] & 0b11111000) == 0b11110000) {
            if (bytes.size() != 4) utfError("Wrong number of bytes for 21-bit code point.");
            if (!isFollowByte(bytes[1])) utfError("First paired byte has wrong header.");
            if (!isFollowByte(bytes[2])) utfError("Second paired byte has wrong header.");
            if (!isFollowByte(bytes[3])) utfError("Third paired byte has wrong header.");
            
            return ((bytes[0] & 0b00000111) << 18) + 
                   ((bytes[1] & 0b00111111) << 12) +
                   ((bytes[2] & 0b00111111) <<  6) +
                   ((bytes[3] & 0b00111111) <<  0);
        }
        
        utfError("Not sure how to handle byte " + toHex(bytes[0]));
    }
    
    /* Given a 16-bit value, writes out an escape sequence for it. */
    string oneEscapeUTF16For(uint16_t value) {
        ostringstream builder;
        builder << "\\u" << hex << setfill('0') << setw(4) << value;
        return builder.str();
    }
    
    /* Given a character that requires a surrogate pair, returns the surrogate components. */
    uint16_t highSurrogateFor(char32_t ch) {
        return ((ch - 0x10000) >> 10) + 0xD800;            // Top 10 bits
    }
    uint16_t lowSurrogateFor(char32_t ch) {
        return ((ch - 0x10000) & 0b1111111111) + 0xDC00;   // Lower 10 bits
    }
    
    /* Given a surrogate pair, assembles the pair back into a single Unicode character. */
    char32_t assembleSurrogates(uint16_t high, uint16_t low) {
        return ((high - 0xD800) << 10) + (low - 0xDC00) + 0x10000;
    }
    
    /* Given a stream, reads an escape sequence of the form \uXXXX and returns the value
     * encoded this way. It may be part of a surrogate pair, so what's returned may not
     * actually be a valid character.
     */
    char32_t readOneUTF16Escape(istream& input) {
        /* Confirm we start with \u. */
        if (get(input) != '\\') utfError("Expected \\u.");
        if (get(input) != 'u')  utfError("Expected \\u.");
        
        /* Read four bytes of hex. */
        string builder;
        for (int i = 0; i < 4; i++) {
            char next = get(input);
            if (!isxdigit(next)) utfError("Expected hexadecimal digit, got " + string(1, next));
            
            builder += next;
        }
        
        /* Convert those bytes to an integer. */
        return char32_t(stoi(builder, nullptr, 16));
    }
}

char32_t readChar(istream& source) {
    return decode(nextBytesFrom(source));
}

char32_t peekChar(istream& source) {
    /* Read the bytes and decode them. */
    auto bytes = nextBytesFrom(source);
    auto result = decode(bytes);
    
    /* Put the bytes back. */
    for (size_t i = 0; i < bytes.size(); i++) {
        source.unget();
        if (!source) utfError("Couldn't unget enough characters.");
    }
    
    return result;
}

string utf16EscapeFor(char32_t ch) {
    /* If this character is in the range where we can just directly convert it, go do so. */
    if (ch <= 0xD7FF || (ch >= 0xE000 && ch <= 0xFFFF)) {
        return oneEscapeUTF16For(ch);
    } else {
        return oneEscapeUTF16For(highSurrogateFor(ch)) + oneEscapeUTF16For(lowSurrogateFor(ch));
    }
}

char32_t readUTF16EscapedChar(istream& source) {
    char32_t result = readOneUTF16Escape(source);
    
    /* If what we read is pair of a high surrogate, read the next half and reassemble it. */
    if (result >= 0xD800 && result <= 0xDFFF) {
        if (result >= 0xDC00) utfError("Read second half of surrogate pair with no matching first half?");
        
        return assembleSurrogates(result, readOneUTF16Escape(source));
    }
    
    return result;
}


string toUTF8(char32_t charCode) {
    ostringstream result;

    /* Anything 7 bits or less just gets directly mapped to itself. */
    if (charCode < (1u << 7)) {
        result << char(charCode);
    }
    /* Anything using 11 bits or less gets broken into
     * 110xxxxx 10xxxxxx
     */
    else if (charCode < (1u << 11)) {
        uint32_t highFive = charCode >> 6;
        uint32_t lowSix   = charCode & 0b111111;

        result << char(highFive | 0b11000000) << char(lowSix | 0b10000000);
    }
    /* Anything using 16 bits or less gets broken into
     * 1110xxxx 10xxxxxx 10xxxxxx
     */
    else if (charCode < (1u << 16)) {
        uint32_t highFour = charCode >> 12;
        uint32_t midSix   = (charCode >> 6) & 0b111111;
        uint32_t lowSix   = charCode & 0b111111;

        result << char(highFour | 0b11100000)
               << char(midSix | 0b10000000)
               << char(lowSix | 0b10000000);
    }
    /* Anything using 21 bits or less gets broken into
     * 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
     */
    else if (charCode < (1u << 22)) {
        uint32_t highThree = charCode >> 18;
        uint32_t topSix    = (charCode >> 12) & 0b111111;
        uint32_t midSix    = (charCode >>  6) & 0b111111;
        uint32_t lowSix    = charCode & 0b111111;

        result << char(highThree | 0b11110000)
               << char(topSix | 0b10000000)
               << char(midSix | 0b10000000)
               << char(lowSix | 0b10000000);
    }
    else {
        utfError("Unicode value out of range: " + to_string(charCode));
    }

    return result.str();
}

UTFException::UTFException(const string& message) : logic_error(message) {

}
