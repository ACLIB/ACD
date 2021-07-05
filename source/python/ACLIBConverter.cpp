#include "Converter.hpp"

#include "python/PyModule.hpp"

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

    PyMODINIT_FUNC PyInit_aclib_converter()
    {
        static auto module = PyModule("aclib_converter");
        module.addMethod("hash", hash);
        module.addMethod("decrypt", decrypt);
        module.addMethod("unpack", unpack);
        module.init();

        PyEval_InitThreads();

        return module.getModule();
    }

}  // namespace ACLIB
