#ifndef CSV_Included
#define CSV_Included

#include <vector>
#include <unordered_map>
#include <string>
#include <istream>
#include <stdexcept>

/* Type representing data read from a CSV file containing a header row. Access is
 * provided as csv[row][column], where column can be specified either by an integer
 * or as one of the column headers.
 */
class CSV {
public:
    /* Parsing routines. */
    static CSV parse(std::istream& source);
    static CSV parseFile(const std::string& filename);

    /* Basic accessors. */
    std::size_t numRows() const;   // Doesn't include header
    std::size_t numCols() const;
    
    /* Header information. */
    std::vector<std::string> headers() const;

    /* Accessor proxy class. */
    class RowRef {
    public:
        std::string operator[] (std::size_t col) const;
        std::string operator[] (const std::string& colHeader) const;
    
    private:
        RowRef(const CSV* parent, std::size_t row);
        const CSV* mParent;
        std::size_t mRow;

        friend class CSV;
    };
    
    RowRef operator[] (std::size_t col) const;

private:
    /* The data. It's internally represented as a 2D grid of strings, along with
     * auxiliary column header data.
     *
     * The grid itself is represented in row-major order, and does not include the
     * column headers.
     */
    std::vector<std::string> mData;
    std::size_t              mRows;

    /* Column headers are encoded as a map from headers to indices, since the
     * primary operation we'll be supporting is mapping from a name to a column.
     */
    std::unordered_map<std::string, std::size_t> mColumnHeaders;
};

/* Type representing an error caused by a CSV issue. */
class CSVException: public std::logic_error {
public:
    CSVException(const std::string& message);
};

#endif
