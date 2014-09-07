#include <stdio.h>
#include <stdlib.h>
#include <Python.h>


//extern PyObject * PyExc_SystemError_copy; // = PyExc_SystemError;


int run_code(int val){
//int main(void){
    int ret = 0;
    PyObject* retval;

    Py_Initialize();
    PyRun_SimpleString("print('hello word from embedded python')");

    PyObject* main_mod = PyImport_AddModule("__main__");
    PyObject* main_dic = PyModule_GetDict(main_mod);

    const char *script_name = "hello.py";
    FILE* script = fopen(script_name,"r");

    retval = PyImport_ImportModule("numpy");
//    retval = PyImport_ImportModuleEx("numpy", main_dic, main_dic, NULL);
    if(!retval) goto py_error;

    retval = PyRun_File(script, script_name, Py_file_input, main_dic, main_dic);
    if(!retval) goto py_error;

/*
    PyObject* retobj = PyRun_String("foo(11)", Py_eval_input, main_dic, main_dic);
    long retval = PyLong_AsLong(retobj);
*/

    py_exit:
        Py_Finalize();
        return ret;

    py_error:
        ret = -1;
        printf("exception in script !?");
        PyErr_PrintEx(0);
        goto py_exit;

}
