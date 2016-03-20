#ifndef PYTHONWRAPPER_QTPART_H
#define PYTHONWRAPPER_QTPART_H

#include <QObject>
#include <string>

class PythonWrapper;

class PythonWrapper_QtPart
{
public:
    PythonWrapper_QtPart();

    static std::string Expose(PythonWrapper * wrapper, QObject * object, std::string name);

private:
    static std::string ItlAddBaseCode(PythonWrapper * wrapper);
};

#endif // PYTHONWRAPPER_QTPART_H
