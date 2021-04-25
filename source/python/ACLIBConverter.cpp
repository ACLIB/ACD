#include "Converter.hpp"

#include <Python.h>
#include <algorithm>
#include <iomanip>
#include <iostream>

namespace ACLIB
{
    static PyObject* hash(PyObject* self, PyObject* args)
    {
        const char* name;

        if(!PyArg_ParseTuple(args, "s", &name))
        {
            if(!PyUnicode_Check(name))
            {
                PyErr_SetString(PyExc_TypeError, "parameter must be a string");
                return nullptr;
            }
            PyErr_SetString(PyExc_TypeError, "parameter missing");
            return nullptr;
        }

        return PyUnicode_FromString(Converter::getKeyFromFolderName(name).c_str());
    }

    static PyObject* decrypt(PyObject* self, PyObject* args)
    {
        const char* file;

        if(!PyArg_ParseTuple(args, "s", &file))
        {
            if(!PyUnicode_Check(file))
            {
                PyErr_SetString(PyExc_TypeError, "parameter must be a string");
                return nullptr;
            }
            PyErr_SetString(PyExc_TypeError, "parameter missing");
            return nullptr;
        }

        auto decrypted_files = Converter::decryptACD(file);

        PyObject* files = PyDict_New();
        for(auto it = decrypted_files.begin(); it != decrypted_files.end(); it++)
        {
            PyDict_SetItem(
                files,
                PyUnicode_FromString(it->first.c_str()),
                PyUnicode_FromString(it->second.data()));
        }

        return files;
    }

    static PyObject* unpack(PyObject* self, PyObject* args)
    {
        const char* file;
        const char * destination;

        if(!PyArg_ParseTuple(args, "ss", &file, &destination))
        {
            if(!PyUnicode_Check(file) || !PyUnicode_Check(destination))
            {
                PyErr_SetString(PyExc_TypeError, "parameters must be a string");
                return nullptr;
            }
            PyErr_SetString(PyExc_TypeError, "parameters missing");
            return nullptr;
        }
        return PyBool_FromLong(Converter::unpackACD(file, destination));
    }

    static PyMethodDef module_methods[] = {
        {"hash", hash, METH_VARARGS, "doc"},
        {"decrypt", decrypt, METH_VARARGS, "doc"},
        {"unpack", unpack, METH_VARARGS, "doc"},
        {nullptr, nullptr, 0, nullptr}};

    struct PyModuleDef aclib_module = {
        PyModuleDef_HEAD_INIT,
        "aclib_converter",
        "Assetto Corsa 'all in one' python module that provides an interface for shared memory "
        "pages.",
        -1,
        module_methods};

    PyMODINIT_FUNC PyInit_aclib_converter()
    {
        PyObject* module = PyModule_Create(&aclib_module);

        PyEval_InitThreads();

        //        if(PyType_Ready(&EventLoopType) < 0)
        //        {
        //            // err
        //            printf("Could not init EventLoop type");
        //        }

        if(!module)
        {
            // err
            printf("Could not init aclib_converter module.");
        }

        // PyModule_AddObject(module, "EventLoop", reinterpret_cast<PyObject*>(&EventLoopType));

        return module;
    }

}  // namespace ACLIB
