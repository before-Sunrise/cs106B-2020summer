#ifndef JSON_Included
#define JSON_Included

#include <memory>
#include <istream>
#include <ostream>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <stdexcept>
#include <type_traits>

namespace minidata_json_impl {
  struct NullTag    {};
  struct BoolTag    {};
  struct IntegerTag {};
  struct DoubleTag  {};
  struct StringTag  {};
  struct ArrayTag   {};
  struct MapTag     {};

  template <typename T> struct IsJSONConvertible;
  template <typename T> struct IsString;
}

/* Type representing a value represented in JSON format. */
class JSON {
public:
    /* Wraps the specified value as a JSON object. This constructor can accept
     * as inputs any of the following types:
     *
     *   nullptr_t
     *   bool
     *   any integral type
     *   any floating-point type
     *   raw C strings
     *   std::string
     *   arrays
     *   any sequence container of items convertible to JSONs
     *   any associative container whose key type can be converted to string and whose value type can be converted to JSON
     */
    template <
      typename T,
      typename = typename std::enable_if<minidata_json_impl::IsJSONConvertible<T>::value>::type
    > JSON(const T& value);

    /* Parses a piece of text into JSON format. */
    static JSON parse(std::istream& input);
    static JSON parse(const std::string& input);

    /* Builds an array or object. */
    static JSON array (std::initializer_list<JSON> elems = {});
    template <typename... T> static JSON array(const T&... args);
    
    static JSON object(std::initializer_list<std::pair<const std::string, JSON>> elems = {});

    /* Enumeration representing what type of object this is. */
    enum class Type {
        OBJECT,
        ARRAY,
        STRING,
        NUMBER,
        BOOLEAN,
        NULLPTR_T
    };

    /* Returns the type of this object. */
    Type type() const;

    /* Accessors. All of these functions will throw a JSONException if the underlying type
     * is incorrect.
     */
    double         asDouble()  const;
    std::int64_t   asInteger() const;
    bool           asBoolean() const;
    std::nullptr_t asNull()    const;
    std::string    asString()  const;

    /* Array accessors. Again, these will throw JSONExceptions if the underlying type is
     * incorrect.
     */
    JSON operator[] (std::size_t index) const;

    /* Object accessors. As usual, these throw JSONExceptions if the underlying type is
     * incorrect.
     */
    JSON operator[] (const std::string& field) const;

    /* Something convertible to string. */
    template <
      typename String,
      typename = typename std::enable_if<minidata_json_impl::IsString<String>::value>::type
    >
    JSON operator[] (const String& field) const {
      return (*this)[std::string(field)];
    }
    bool contains(const std::string& fieldName) const;
    
    /* Generic accessor. If the wrapped JSON object is a number, looks up the element at that index,
     * assuming this is an array. If the wrapped JSON object is a string, looks up the element with
     * that key, assuming this is an object.
     *
     * Be careful calling this function with wrapped doubles; any rounding errors will cause
     * problems!
     */
    JSON operator[] (JSON key) const;

    /* Shared between arrays and objects. */
    std::size_t size() const;

    /* For arrays, iterates over the elements. For objects, iterates over the keys as JSON strings.
     * For all other types, raises an error.
     */
    class const_iterator;
    const_iterator begin()  const;
    const_iterator end()    const;
    const_iterator cbegin() const;
    const_iterator cend()   const;
    
    friend std::ostream& operator<< (std::ostream& out, JSON json);

private:
    friend class BaseJSON;
    std::shared_ptr<class BaseJSON> mImpl;

    struct ConstructorTag {};
    JSON(std::shared_ptr<class BaseJSON> impl, ConstructorTag);

    /* Construct JSON objects from objects of the appropriate types. */
    static std::shared_ptr<class BaseJSON> fromNull(std::nullptr_t value);
    static std::shared_ptr<class BaseJSON> fromDouble(double value);
    static std::shared_ptr<class BaseJSON> fromInteger(std::int64_t value);
    static std::shared_ptr<class BaseJSON> fromBoolean(bool value);
    static std::shared_ptr<class BaseJSON> fromString(const std::string& value);

    /* Constructs a JSON array from a list of JSON objects. */
    static std::shared_ptr<class BaseJSON> fromArray(const std::vector<JSON>& elems);

    /* Constructs a JSON object from a map. */
    static std::shared_ptr<class BaseJSON> fromMap(const std::unordered_map<std::string, JSON>& elems);

    /* Dispatching constructors. */
    template <typename T> JSON(minidata_json_impl::NullTag,    const T& value) : mImpl(fromNull(value)) {}
    template <typename T> JSON(minidata_json_impl::BoolTag,    const T& value) : mImpl(fromBoolean(value)) {}
    template <typename T> JSON(minidata_json_impl::IntegerTag, const T& value) : mImpl(fromInteger(value)) {}
    template <typename T> JSON(minidata_json_impl::DoubleTag,  const T& value) : mImpl(fromDouble(value)) {}
    template <typename T> JSON(minidata_json_impl::StringTag,  const T& value) : mImpl(fromString(value)) {}
    template <typename T> JSON(minidata_json_impl::ArrayTag,   const T& value) : 
      mImpl(fromArray(std::vector<JSON>(std::begin(value), std::end(value)))) {}
    template <typename T> JSON(minidata_json_impl::MapTag,     const T& value) : 
      mImpl(fromMap(std::unordered_map<std::string, JSON>(std::begin(value), std::end(value)))) {}
};

class JSON::const_iterator {
public:
    /* Iterator typedefs. */
    typedef std::ptrdiff_t          difference_type;
    typedef const JSON              value_type;
    typedef const JSON*             pointer;
    typedef const JSON&             reference;
    typedef std::input_iterator_tag iterator_category;

    const_iterator();
    
    bool operator== (const_iterator rhs) const;
    bool operator!= (const_iterator rhs) const;
    
    const_iterator& operator++ ();
    const const_iterator operator++ (int);
    
    const JSON& operator* () const;
    const JSON* operator->() const;

private:
    friend class JSONSource;
    std::shared_ptr<class JSONSource> mImpl;
    
    const_iterator(std::shared_ptr<class JSONSource>);
};

/* Exception type thrown when an error occurs. */
class JSONException: public std::logic_error {
public:
    JSONException(const std::string& reason);
};

/***** Private Implementation Details *****/
namespace minidata_json_impl {
  /* Traits types to determine the best type to use for something. */

  /* Is this a null pointer? */
  template <typename T> struct IsNull {
    static const bool value = std::is_same<typename std::remove_cv<T>::type, std::nullptr_t>::value;
  };

  /* Is this a boolean value? */
  template <typename T> struct IsBool {
    static const bool value = std::is_same<typename std::remove_cv<T>::type, bool>::value;
  };

  /* Is this an integer? */
  template <typename T> struct IsInteger {
    /* Need to explicitly define bool away. */
    static const bool value = !IsBool<T>::value && std::is_integral<T>::value;
  };

  /* Is this a double? */
  template <typename T> struct IsDouble {
    static const bool value = std::is_floating_point<T>::value;
  };

  /* Is this a C-style string? */
  template <typename T> struct IsString {
    static const bool value = std::is_convertible<T, std::string>::value;
  };
  /* Is this an array of JSONs? */
  template <typename T> struct IsArray {
    /* Does std::begin give us something we can make a JSON from? */
    template <typename U> static std::true_type  evaluate(int, typename std::enable_if<std::is_convertible<typename U::value_type, JSON>::value>::type* = nullptr);
    template <typename U> static std::false_type evaluate(...);

    static const bool value = std::is_same<decltype(evaluate<T>(0)), std::true_type>::value;
  }; 
  template <size_t N> struct IsArray<JSON [N]> {
    static const bool value = true;
  };
  template <size_t N> struct IsArray<const JSON [N]> {
    static const bool value = true;
  };

  /* Is this a map of JSONs? */
  template <typename T> struct IsMap {
    /* Does std::begin give us something where .first can be made a string and
     * .second can be made a JSON?
     */
    template <typename U> using value_type = typename U::value_type;
    template <typename U> using k_type   = decltype(std::declval<value_type<U>>().first);
    template <typename U> using v_type = decltype(std::declval<value_type<U>>().second);
    template <typename U> static std::true_type  evaluate(int,
                                                          typename std::enable_if<IsString<k_type<U>>::value>* = 0,
                                                          typename std::enable_if<std::is_convertible<v_type<U>, JSON>::value>::type* = 0);
    template <typename U> static std::false_type evaluate(...);

    static const bool value = std::is_same<decltype(evaluate<T>(0)), std::true_type>::value;
  };

  template <typename T> struct TagFor {
    using type = typename std::conditional<IsNull<T>::value, NullTag,
                  typename std::conditional<IsBool<T>::value, BoolTag,
                    typename std::conditional<IsInteger<T>::value, IntegerTag,
                      typename std::conditional<IsDouble<T>::value, DoubleTag,
                        typename std::conditional<IsString<T>::value, StringTag,
                          typename std::conditional<IsArray<T>::value, ArrayTag,
                            typename std::conditional<IsMap<T>::value, MapTag, void>::type
                          >::type
                        >::type
                      >::type
                    >::type
                  >::type
                >::type;
  };

  template <typename T> struct IsJSONConvertible {
    static const bool value = !std::is_same<typename TagFor<T>::type, void>::value;
  };
}

/* Forward things to the right constructor. */
template <typename T, typename> JSON::JSON(const T& value) : JSON(typename minidata_json_impl::TagFor<T>::type(), value) {

}

/* Variadic functions convert to initializer lists. */
template <typename... T> JSON JSON::array(const T&... args) {
  return array({ args... });
}

#endif
