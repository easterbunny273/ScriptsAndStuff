#include "pythonwrapper_qtpart.h"

#include "pythonwrapper.h"

#include <QMetaMethod>

#include <set>

static std::set<PythonWrapper*> initializedWrappers;

PythonWrapper_QtPart::PythonWrapper_QtPart()
{

}

std::string PythonWrapper_QtPart::Expose(PythonWrapper * wrapper, QObject *object, std::string name)
{
    std::string returnString;

    if (initializedWrappers.find(wrapper) == initializedWrappers.end())
        returnString += ItlAddBaseCode(wrapper);

    std::string pythonCode;

    pythonCode = name + " = WrappingObject()\n";

    auto metaObject = object->metaObject();

    for (unsigned int n=metaObject->methodOffset(); n < metaObject->methodCount(); ++n)
    {
        auto metaMethod = metaObject->method(n);
        auto methodName = metaMethod.name().toStdString();

        std::string sParamNamesAsPythonList = "[";

        auto qParamNames = metaMethod.parameterNames();
        for (auto iter = qParamNames.begin(); iter != qParamNames.end(); ++iter)
        {
            auto nameAsBytes = *iter;
            if (iter != qParamNames.begin())
                sParamNamesAsPythonList.append(",");
            sParamNamesAsPythonList.append("\"");
            sParamNamesAsPythonList.append(nameAsBytes.toStdString());
            sParamNamesAsPythonList.append("\"");
        }

        sParamNamesAsPythonList.append("]");

        std::string sAddMethodAsPythonCode = std::string("add_method_to_object(") + name + ", \"" + methodName + "\", " + sParamNamesAsPythonList + ")\n";
        pythonCode.append(sAddMethodAsPythonCode);
    }

    returnString += wrapper->execute(pythonCode);

    return returnString;
}

std::string PythonWrapper_QtPart::ItlAddBaseCode(PythonWrapper *wrapper)
{
    std::string sBaseCode =
"import types\n\
\n\
__author__ = 'chmoe'\n\
\n\
class WrappingObject:\n\
    def hello(self):\n\
        print(\"hello world!\")\n\
\n\
def _add_method_to_object(object, method_name, method):\n\
    object.__dict__[method_name] = types.MethodType(method, object)\n\
\n\
def add_method_to_object(object, method_name, list_of_arg_names):\n\
    full_list_of_arg_names = [\"self\"] + list_of_arg_names\n\
    args_as_string = \", \".join(list_of_arg_names)\n\
    full_args_as_string = \", \".join(full_list_of_arg_names)\n\
    python_code = \"_add_method_to_object(object, \\\"\" + method_name+ \"\\\", (lambda \" + full_args_as_string + \": slot_wrapping_method(self, \\\"\" + method_name+ \"\\\", \" + str(list_of_arg_names) + \", [\" + args_as_string + \"])))\"\n\
    print (python_code)\n\
    exec (python_code)\n\
\n\
def slot_wrapping_method(self, slot_name, argument_names, arguments):\n\
    arguments_map = {}\n\
    for combined_entry in zip(argument_names, arguments):\n\
        key, value = combined_entry\n\
        arguments_map[key] = value\n\
\n\
    print (\"calling slot \" + slot_name + \" with arguments \" + str(arguments_map) + \" for \" + str(self))\n\
    emb.call_slot(slot_name, arguments_map)\n\
\n\
    return [42, 1]\n";

    initializedWrappers.insert(wrapper);

    return wrapper->execute(sBaseCode);
}
