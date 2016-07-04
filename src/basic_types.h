#ifndef BASIC_TYPES_H
#define BASIC_TYPES_H

class QString;

// constexpr string
class str_const
{
public:
    template<std::size_t N>
    constexpr str_const(const char(&a)[N]) : _str(a), _size(N - 1)
    {}
    constexpr char operator[](std::size_t n) const
    {
        return (n < _size) ? _str[n] :
            throw std::out_of_range("");
    }
    constexpr std::size_t size() const { return _size; }
    const char *c_str() const { return _str; }

private:
    const char* const _str;
    const std::size_t _size;

};

inline QString str_const_toString(const str_const &str) {
    return QString::fromUtf8(str.c_str(), str.size());
}

#endif // BASIC_TYPES_H
