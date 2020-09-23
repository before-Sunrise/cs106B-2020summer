#include "datapoint.h"
#include "strlib.h"
#include <sstream>
#include <iomanip>
using namespace std;

/* Utilities to read and write quoted strings.
 *
 * TODO: This should be replaced with the use of std::quoted as soon as
 * C++14 support is available on Windows.
 */
namespace {
    string quotedVersionOf(const string& source) {
        ostringstream result;
        result << '"';

        for (char ch: source) {
            /* Escape close quotes. */
            if (ch == '"') result << "\\\"";

            /* Escape slashes. */
            else if (ch == '\\') result << "\\\\";

            /* Print out any other printable characters. */
            else if (isgraph(ch)) result << ch;

            /* Otherwise, escape it. */
            else {
                result << "\\x" << hex << setfill('0') << setw(2) << +static_cast<unsigned char>(ch);
            }
        }

        result << '"';
        return result.str();
    }

    /* Reads a quoted version of a string. */
    bool readQuoted(istream& in, string& out) {
        /* Read a character; it must be a quote. */
        char read;
        in >> read;

        if (!in || read != '"') return false;

        /* Keep reading until we get a close quote. */
        string result;
        while (true) {
            /* Can't read? That's a problem! */
            if (!in.get(read)) return false;

            /* If this is a quote, we're done. */
            else if (read == '"') break;

            /* Otherwise, if it's a slash, treat it as an escape. */
            else if (read == '\\') {
                /* Get the next character to see what we're supposed to do. */
                if (!in.get(read)) return false;

                /* Output slashes and quotes. */
                else if (read == '\\' || read == '"') result += read;

                /* Hex? Read two characters and decode them. */
                else if (read == 'x') {
                    string hexCode;
                    in >> setw(2) >> hexCode;
                    if (!in) return false;

                    /* Convert this to a number. */
                    try {
                        result += static_cast<char>(stringToInteger(hexCode, 16));
                    } catch (const ErrorException& e) {
                        return false;
                    }
                }
                /* Otherwise, we have no idea what this is. */
                else return false;
            }

            /* Otherwise, just append it. */
            else result += read;
        }

        out = result;
        return true;
    }

    /* Constants used to track whether we did vector or scalar initialization. */
    const int kIsVector    = 0xA110C2;
    const int kIsScalar    = 0xA110C1;
    const int kDeallocated = 0xA110C0;

    /* Allocates a block of memory that contains one more DataPoint than requested.
     * That extra DataPoint is a sentinel that tells us whether we used the scalar
     * or vector alloc/dealloc function.
     */
    void* allocate(size_t space, bool isVector) {
        /* Get more space than we need. */
        void* result = operator new(sizeof(DataPoint) + space);

        /* Construct a DataPoint in that space. */
        ::new (result) DataPoint{ "", isVector? kIsVector : kIsScalar };

        /* Return a pointer one past where we started. */
        return ((DataPoint *) result) + 1;
    }

    /* Deallocates a block of memory, checking to make sure the type of deallocation
     * performed was the right one.
     */
    void deallocate(void* memory, bool isVector) {
        DataPoint* predecessor = ((DataPoint*) memory) - 1;

        if (predecessor->priority == kIsVector) {
            if (!isVector) {
                cerr << "You are attempting to deallocate a block of memory that you allocated with"
                        "new[] using the delete operator. This will cause memory errors. Instead,"
                        "use the delete[] operator (with square brackets)." << endl;
                abort();
            }
        } else if (predecessor->priority == kIsScalar) {
            if (isVector) {
                cerr << "You are attempting to deallocate a block of memory that you allocated with"
                       "new using the delete[] operator. This will cause memory errors. Instead,"
                       "use the delete operator (without square brackets)." << endl;
                abort();
            }
        } else if (predecessor->priority == kDeallocated) {
            cerr << "You are attempting to delete memory that you have already deleted. Review your"
                    "code to see where this error is occurring and see if you can fix the bug." << endl;
            abort();
        } else {
            cerr << "Something went wrong when you tried to deallocate memory. This could mean that you "
                    "deallocated memory you didn't allocate, or that you used the wrong deallocation "
                    "operator (for example, mixing up delete and delete[]). Review your code to see "
                    "where this error is occurring and see if you can fix the bug." << endl;
            abort();
        }

        /* Replace the marker with something indicating we deallocated this space. */
        predecessor->priority = kDeallocated;

        /* Destroy the DataPoint we created. */
        predecessor->~DataPoint();

        /* Free the memory. */
        operator delete(predecessor);
    }
}

/* Equality comparison. */
bool operator== (const DataPoint& lhs, const DataPoint& rhs) {
    return lhs.name == rhs.name && lhs.priority == rhs.priority;
}

/* Inequality holds when the two points aren't equal. */
bool operator!= (const DataPoint& lhs, const DataPoint& rhs) {
    return !(lhs == rhs);
}

/* Prints a DataPoint to a stream. The output format is
 *
 *   { "name of the data point, \"properly escaped\"": pt.priority }
 */
ostream& operator<< (ostream& out, const DataPoint& pt) {
    ostringstream builder;
    builder << "{ " << quotedVersionOf(pt.name) << ", " << pt.priority << " }";
    return out << builder.str();
}

/* Reads a DataPoint from a stream. */
istream& operator>> (istream& in, DataPoint& result) {
    istream::sentry sentry(in);
    if (sentry) {
        /* Grab the next character; it should be an open brace. */
        char expected;
        in >> ws >> expected;
        if (!in || expected != '{') {
            in.setstate(ios::failbit);
            return in;
        }

        /* Extract the string. */
        DataPoint read;
        in >> ws;
        if (!readQuoted(in, read.name) || !in) {
            in.setstate(ios::failbit);
            return in;
        }

        /* Confirm there's a comma here. */
        in >> ws >> expected;
        if (!in || expected != ',') {
            in.setstate(ios::failbit);
            return in;
        }

        /* Read the priority. */
        in >> ws >> read.priority;
        if (!in) {
            in.setstate(ios::failbit);
            return in;
        }

        /* Read the close brace. */
        in >> ws >> expected;
        if (!in || expected != '}') {
            in.setstate(ios::failbit);
            return in;
        }

        result = read;
    }
    return in;
}
