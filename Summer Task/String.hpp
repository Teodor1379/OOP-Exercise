#ifndef STRING_G
#define STRING_G

#include <cstring>

class String {
public:
    String()                        { String::build(this->string, "");              }
    String(const char* string)      { String::build(this->string, string);          }
    String(const String& instance)  { String::build(this->string, instance.string); }

    ~String()                       { delete[] this->string;                        }

    String& operator=(const String& instance);

    const char* c_str() const noexcept { return this->string;   }
    const char* data()  const noexcept { return this->string;   }

    std::size_t size()      const noexcept { return strlen(this->string);    }
    std::size_t length()    const noexcept { return strlen(this->string);    }

    char& operator[](const std::size_t pos);
    const char& operator[](const std::size_t pos) const;

    void        removeSpaces();
    std::size_t countSpaces() const;
    
    bool isNumber() const;

private:
    static void build(char*& destination, const char* source);

    char*   string;
};

#endif
