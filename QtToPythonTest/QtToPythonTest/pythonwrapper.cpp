#include "pythonwrapper.h"

#include <Python.h>

#include <iostream>

PyObject * m_pyMainModule;

static int numargs=0;

/* Return the number of arguments of the application command line */
static PyObject*
emb_numargs(PyObject *self, PyObject *args)
{
    if(!PyArg_ParseTuple(args, ":numargs"))
        return NULL;
    return PyLong_FromLong(numargs);
}


std::string HelperGetStringFromPyObject(PyObject *object)
{
    std::cerr << "extracting string" << std::endl;

    return PyBytes_AsString(object);
    /*
    bool isUnicode = PyUnicode_Check(object);

    if (isUnicode)
    {
        PyObject * temp_bytes = PyUnicode_AsEncodedString(object, "ASCII", "strict"); // Owned reference
        if (temp_bytes != NULL)
        {
            auto my_result = PyBytes_AS_STRING(temp_bytes); // Borrowed pointer
            my_result = strdup(my_result);
            Py_DECREF(temp_bytes);

            return my_result;
        }
        else
        {
            // TODO: Handle encoding error.
            return "";
        }
    }
    else if (PyBytes_Check(object)) {
        auto my_result = PyBytes_AS_STRING(object); // Borrowed pointer
        my_result = strdup(my_result);
        return my_result;
    }

    return "";*/
}


/* Return the number of arguments of the application command line */
static PyObject*
call_slot(PyObject *self, PyObject *args)
{
    std::cout << "call_slot enter" << std::endl;

    char * szMethodName;
    PyObject * parametersMap;

    if (!PyArg_ParseTuple(args, "sO", &szMethodName, parametersMap))
        return NULL;

    //std::string methodName = HelperGetStringFromPyObject(methodNameO);

    std::cout << "now calling: " << szMethodName << std::endl;


    return Py_None;
}

static PyMethodDef EmbMethods[] = {
    {
        "call_slot", call_slot, METH_VARARGS, "Return the number of arguments received by the process."},
        //"numargs", emb_numargs, METH_VARARGS, "Return the number of arguments received by the process."},
    {NULL, NULL, 0, NULL}
};

static PyModuleDef EmbModule = {
    PyModuleDef_HEAD_INIT, "emb", NULL, -1, EmbMethods,
    NULL, NULL, NULL, NULL
};

static PyObject*
PyInit_emb(void)
{
    return PyModule_Create(&EmbModule);
}

PythonWrapper::PythonWrapper()
{
    numargs = 0;
    PyImport_AppendInittab("emb", &PyInit_emb);

    // start up python
    Py_Initialize();

    m_pyMainModule = PyImport_ImportModule("__main__"); //create main module

    execute("import emb");
}

std::string PythonWrapper::execute(const std::string &str)
{
    ItlResetOutputCatcher();

    PyRun_SimpleString(str.c_str());

    PyObject *catcher = PyObject_GetAttrString(m_pyMainModule,"catchOutErr"); //get our catchOutErr created above
    PyErr_Print(); //make python print any errors

    PyObject *output = PyObject_GetAttrString(catcher,"value"); //get the stdout and stderr from our catchOutErr object

    bool isSafe = (catcher!=0) && (output != 0);

    if (!isSafe)
        return "<error>";

    bool isUnicode = PyUnicode_Check(output);

    if (isUnicode)
    {
        PyObject * temp_bytes = PyUnicode_AsEncodedString(output, "ASCII", "strict"); // Owned reference
        if (temp_bytes != NULL)
        {
            auto my_result = PyBytes_AS_STRING(temp_bytes); // Borrowed pointer
            my_result = strdup(my_result);
            Py_DECREF(temp_bytes);

            return my_result;
        }
        else
        {
            // TODO: Handle encoding error.
        }
    }
    else if (PyBytes_Check(output)) {
        auto my_result = PyBytes_AS_STRING(output); // Borrowed pointer
        my_result = strdup(my_result);
        return my_result;
    }

    return "";
}

PythonWrapper::~PythonWrapper()
{
    Py_Finalize();
}

void PythonWrapper::ItlResetOutputCatcher()
{
    std::string stdOutErr =
"import sys\n\
class CatchOutErr:\n\
   def __init__(self):\n\
       self.value = ''\n\
   def write(self, txt):\n\
       self.value += txt\n\
catchOutErr = CatchOutErr()\n\
sys.stdout = catchOutErr\n\
sys.stderr = catchOutErr\n\
"; //this is python code to redirect stdouts/stderr
    //PyRun_SimpleString("catchOutErr.reset()");
            PyRun_SimpleString(stdOutErr.c_str()); //invoke code to redirect
}
