#include <LocationFinder.h>
#include <string.h>
#include <Python.h>

static PyObject *PyCFunc_Finder(PyObject *self, PyObject *args)
{
    const char *findType = NULL;

    if (!PyArg_ParseTuple(args, "s", &findType))
    {
        PyErr_SetString(PyExc_ValueError, "Wrong argument, expect a string");
        return NULL;
    }
    InstallPath installPath;

    if (!strcmp(findType, "visual_studio"))
    {
        installPath.type = VISUAL_STUDIO;
    }
    else if (!strcmp(findType, "python"))
    {
        installPath.type = PYTHON;
    }
    else if (!strcmp(findType, "node_js"))
    {
        installPath.type = NODE_JS;
    }
    else
    {
        return PyList_New(0);
    }

    int isInstallPathFind = GetInstallPath(&installPath);

    if (!isInstallPathFind)
    {
        return PyList_New(0);
    }

    PyObject *pyList_installPathResult = PyList_New(installPath.versions);

    for (int i = 0; i < installPath.versions; i++)
    {
        PyList_SetItem(pyList_installPathResult, i, PyUnicode_FromString(installPath.path[i]));
    }

    ReleaseInstallPath(&installPath);

    return pyList_installPathResult;
};

static PyMethodDef methods[] = {
    {"Finder", PyCFunc_Finder, METH_VARARGS, NULL},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef module = {
    PyModuleDef_HEAD_INIT,
    "PyLocationFinder",
    NULL,
    -1,
    methods};

PyMODINIT_FUNC
PyInit_PyLocationFinder(void)
{
    return PyModule_Create(&module);
}
