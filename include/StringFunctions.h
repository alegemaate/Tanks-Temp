#ifndef STRINGFUNCTIONS_H
#define STRINGFUNCTIONS_H

#include <vector>
#include <string>

class StringFunctions {
  public:
    StringFunctions();
    virtual ~StringFunctions();

    static std::vector<std::string> explode(const std::string& s, const char& c);

    // Convert int to string
    static std::string IntToString(int value);
    static std::string DoubleToString(double value);
    static std::string BoolToString(bool value);

  protected:

  private:
};

#endif // STRINGFUNCTIONS_H
