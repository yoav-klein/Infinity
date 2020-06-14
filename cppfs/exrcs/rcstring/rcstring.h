
#ifndef __RC_STRING_H__
#define __RC_STRING_H__

namespace ilrd
{

class RCString
{
public:
    explicit RCString(const char *str = "");
    RCString(const RCString& other);
    ~RCString();
    char& operator[](std::size_t index);
    const char& operator[](std::size_t index) const;
    RCString& operator=(const RCString& other);
    size_t Length() const;
    const char* ToCStr() const;
    bool IsShared() const;

private:
    void DeleteIfShould();
    char* InitBuffer(const char *str);
    char* m_buffer;
    friend std::istream& operator>>(std::istream& i_stream, RCString& str);
};
std::ostream& operator<<(std::ostream& os, const RCString& str);
std::istream& operator>>(std::istream& i_stream, RCString& str);

bool operator==(const RCString& lhs, const RCString& rhs);
bool operator!=(const RCString& lhs, const RCString& rhs);
bool operator>(const RCString& lhs, const RCString& rhs);
bool operator<(const RCString& lhs, const RCString& rhs);


} // namespace ilrd
#endif //__RC_STRING_H__ 
