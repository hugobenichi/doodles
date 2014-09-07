#include <stdlib.h>
#include <stdio.h>


#include <Python.h>
#include <numpy/arrayobject.h>


/*
void exec_pycode(const char* code)
{
    Py_Initialize();
    PyRun_SimpleString(code);
    Py_Finalize();
}
*/


/*
int main(int argc, wchar_t** argv) {
    Py_Initialize();
    PyRun_SimpleString("print('hello word from embedded python')");
    Py_Main(argc, argv);
    Py_Finalize();
    exit(0);
}
*/


int main(int argc, wchar_t** argv) {
    int ret;
    PyObject* retval;

    Py_Initialize();
    PyRun_SimpleString("print('hello word from embedded python')");


    /* import numpy module */
    retval = PyImport_ImportModule("numpy");
    if(!retval) goto py_error;
    _import_array();

    /* setting up the main environement */
    PyObject* main_mod = PyImport_AddModule("__main__");
    PyObject* main_dic = PyModule_GetDict(main_mod);

    const char *script_name = "hello.py";
    FILE* script = fopen(script_name,"r");

    /* running the script */
    retval = PyRun_File(script, script_name, Py_file_input, main_dic, main_dic);
    if(!retval) goto py_error;

    /* function call by name */
    retval = PyRun_String("foo(10)", Py_eval_input, main_dic, main_dic);
    if(!retval) goto py_error;
    printf("%li\n", PyLong_AsLong(retval));
    
    /* function call by reference */
    PyObject* bar = PyObject_GetAttrString(main_mod, "bar");
    if(!bar) goto py_error;

    retval = PyObject_CallObject(bar, NULL);
    if(!retval) goto py_error;


    /* preparing raw memory */
    int len = 10; //00;
    double * buffer = malloc(sizeof(double) * len);
    if(!buffer) printf("malloc error\n");
    for(int i = 0; i < len; i++) buffer[i] = 1.0 * i;   


    /* making a new numpy array container*/
    int dim[1];
    dim[0] = len;
    PyObject* pyary = PyArray_FromDimsAndData(1, dim, NPY_DOUBLE, (void*) buffer);

/*
    long dim[1];
    dim[0] = len/100;
    PyObject* pyary = PyArray_SimpleNewFromData(1, dim, NPY_DOUBLE, (void*) buffer);
*/


    PyObject* print_ary = PyObject_GetAttrString(main_mod, "print_ary");
    if(!print_ary) goto py_error;

    /* function call by reference with vararg input*/
    PyObject_CallFunctionObjArgs(print_ary, pyary, NULL);

/*
    // call with constructed tuple arg
    PyObject* args = PyTuple_Pack(  6,
    PyFloat_FromDouble(x),
    PyFloat_FromDouble(xp),
    PyFloat_FromDouble(dt),
    PyFloat_FromDouble(y),
    PyFloat_FromDouble(yp),
    PyFloat_FromDouble(dz));
    retval = PyObject_CallObject(print_ary, args);
*/


    if(!retval) goto py_error;

    py_exit:
        Py_Finalize();
        exit(ret);

    py_error:
        ret = -1;
        printf("exception in script !?");
        PyErr_PrintEx(0);
        goto py_exit;

}
