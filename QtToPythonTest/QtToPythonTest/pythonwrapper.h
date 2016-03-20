#ifndef PYTHONWRAPPER_H
#define PYTHONWRAPPER_H

#include <string>

class PythonWrapper
{
public:
    PythonWrapper();
    std::string execute(const std::string &str);
    ~PythonWrapper();

private:
    void ItlResetOutputCatcher();
};

#endif // PYTHONWRAPPER_H
