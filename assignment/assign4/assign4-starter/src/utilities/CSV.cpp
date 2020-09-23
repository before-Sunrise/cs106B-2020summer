#include "CSV.h"
#include <sstream>
#include <fstream>
#include <tuple>
using namespace std;

namespace {
    /* Reports an error. */
    [[ noreturn ]] void csvError(const string& message) {
        throw CSVException(message);
    }

    /* Reads a single CSV token from a source. Each token either
     *
     *  1. does not start with a quote, in which case we read up until the first comma, or
     *  2. starts with a quote, in which case we read to the upcoming close quote, watching for
     *     escaped quotes along the way.
     *
     * Empty entries are acceptable.
     */
    string readOneTokenFrom(istream& input) {
        /* Edge case: empty entries are fine. */
        if (input.peek() == ',') return "";
        
        /* If we don't start with a quote, read up until we do. */
        if (input.peek() != '"') {
            string result;           
            while (true) {
                int ch = input.peek();
                if (ch == EOF) return result;
                if (ch == ',') return result;
                result += char(input.get());
            }
        }
        
        /* We are looking a quoted string. Keep reading characters, keeping in mind that a close
         * quote might not actually be the end-of-string marker.
         */
        input.get(); // Skip quotation mark
        
        string result;
        while (true) {
            int ch = input.get();
            
            if (ch == EOF) csvError("Unterminated string literal.");
            else if (ch != '"') result += char(ch);
            else {
                int next = input.peek();
                if (next == EOF || next == ',') return result; // End of token
                else if (next == '"') {
                    /* Consume this character so we don't process it twice. */
                    input.get();
                    result += '"';
                } else csvError("Unexpected character found after quote.");
            }
        }
    }

    /* Tokenizes a line from a CSV file, returning a list of tokens within that line. */
    vector<string> tokenize(const string& line) {
        /* Edge case: we assume there are no empty lines even though in principle we could
         * envision a 0 x n data array. That likely just means something went wrong.
         */
        if (line.empty()) csvError("Empty line in CSV data.");
    
        /* Convert to a stream to make it easier to treat the characters as though they're a stream. */
        istringstream input(line);
        
        vector<string> result;
        while (true) {
            result.push_back(readOneTokenFrom(input));
            
            /* We should either see a comma or an EOF at this point. */
            if (input.peek() == EOF) return result;
            if (input.get()  != ',') csvError("Entries in CSV file aren't comma-separated?");
        }
    }

    /* Reads the first line of a CSV file, breaking it apart into headers. */
    unordered_map<string, size_t> readHeaders(istream& input) {
        string line;
        if (!getline(input, line)) csvError("Could not read header row from CSV source.");
        
        unordered_map<string, size_t> result;
        for (auto token: tokenize(line)) {
            if (result.count(token)) csvError("Duplicate column header: " + token);
            
            size_t index = result.size();
            result[token] = index;
        }
        
        return result;
    }
    
    /* Reads the body of a CSV file under the assumption that it has a certain number of
     * columns.
     *
     * The result is a pairing of the row-major-ordering of the data, along with the
     * number of rows in the data.
     */
    tuple<vector<string>, size_t> readBody(istream& input, size_t numCols) {
        /* We'll build the grid as a vector<vector<string>> and collapse it at the end. */
        vector<vector<string>> lines;
        for (string line; getline(input, line); ) {
            auto tokens = tokenize(line);
            if (tokens.size() != numCols) csvError("Lines have varying number of entries.");
            
            lines.push_back(tokens);
        }
        
        /* Flatten the list. */
        vector<string> result;
        for (size_t row = 0; row < lines.size(); row++) {
            for (size_t col = 0; col < numCols; col++) {
                result.push_back(lines[row][col]);
            }
        }
        return make_tuple(result, lines.size());
    }
}

CSV CSV::parse(istream& input) {
    CSV result;

    result.mColumnHeaders = readHeaders(input);
    tie(result.mData, result.mRows) = readBody(input, result.mColumnHeaders.size());

    return result;
}

CSV CSV::parseFile(const string& filename) {
    ifstream input(filename);
    if (!input) csvError("Cannot open file " + filename);

    return CSV::parse(input);
}

size_t CSV::numRows() const {
    return mRows;
}

size_t CSV::numCols() const {
    return mColumnHeaders.size();
}

vector<string> CSV::headers() const {
    vector<string> result(mColumnHeaders.size());
    for (const auto& entry: mColumnHeaders) {
        result[entry.second] = entry.first;
    }
    return result;
}

CSV::RowRef CSV::operator[] (size_t row) const {
    if (row >= numRows()) csvError("Row out of range.");
    
    return RowRef(this, row);
}

CSV::RowRef::RowRef(const CSV* parent, size_t row) : mParent(parent), mRow(row) {

}

string CSV::RowRef::operator[] (size_t col) const {
    if (col >= mParent->numCols()) csvError("Column out of range.");
    
    return mParent->mData[mParent->numCols() * mRow + col];
}
string CSV::RowRef::operator[] (const string& colHeader) const {
    auto itr = mParent->mColumnHeaders.find(colHeader);
    if (itr == mParent->mColumnHeaders.end()) csvError("Column not found: " + colHeader);

    return (*this)[itr->second];
}

CSVException::CSVException(const string& message) : logic_error(message) {
    // Handled in initialization list
}
