# define PY_SSIZE_T_CLEAN
# include <Python.h>
# include <stdlib.h>
# include <stdio.h>
# include "symnmf.h"

int n = 0;
int d = 0;

/* This function frees memory of a two-dimensional array */
void free_matrix_module(double **matrix, int rows){
    int i = 0;
    for(i=0; i< rows; i++){
        free(matrix[i]);
    }
    free(matrix);
}

/* This function converts PyObject to C object for the data points */
double **convertPythonToC_X(PyObject *self, PyObject *args){
    PyObject *X = NULL;
    PyObject *curr_cordinate_item = NULL;
    PyObject *curr_item_list = NULL;
    PyObject* innerList = NULL;
    int checklen = 0;
    int checkleninner = 0;
    int j = 0;
    int ind = 0;
    double curr_cordinate_index = 0.0;
    double *nextVectorToRead = NULL;
    double **X_c = NULL;

    if (!PyArg_ParseTuple(args, "O", &X)) {
        PyErr_SetString(PyExc_Exception, "An Error Has Occurred\n");
        return NULL;
    }
    if (!PyList_Check(X)) {
        PyErr_SetString(PyExc_Exception, "An Error Has Occurred\n");
        return NULL;
    }

    checklen = PyObject_Length(X);
    if (checklen < 0) {
        PyErr_SetString(PyExc_Exception, "An Error Has Occurred\n");
        return NULL;
    }

    innerList = PyList_GetItem(X, 0);
    checkleninner = PyObject_Length(innerList);
    if (checkleninner < 0) {
        PyErr_SetString(PyExc_Exception, "An Error Has Occurred\n");
        return NULL;
    }

    n = (int)checklen;
    d = (int)checkleninner;

    X_c =(double **) malloc(n * sizeof(double *));
    if (X_c == NULL) {
        PyErr_SetString(PyExc_Exception, "An Error Has Occurred\n");
        return NULL;
    }

    /* Creating a matrix of datapoints */
    for (j = 0; j < n; j++) {
        nextVectorToRead =(double *) malloc(d * sizeof(double));
        /* check if allocation did not succeed */
        if (nextVectorToRead == NULL) {
            free_matrix_module(X_c, j - 1);
            PyErr_SetString(PyExc_Exception, "An Error Has Occurred\n");
            return NULL;
        }

        curr_item_list = PyList_GetItem(X, j);
        for (ind = 0; ind < d; ind++) {
            curr_cordinate_item = PyList_GetItem(curr_item_list, ind);
            curr_cordinate_index = (double) PyFloat_AsDouble(curr_cordinate_item);
            nextVectorToRead[ind] = curr_cordinate_index;
        }
        X_c[j] = nextVectorToRead;

    }
    return X_c;
}

/* This function converts C object to PyObject of a two-dimensional array */
PyObject *convertCToPython(double **convertobj, int n, int innerlen){
    PyObject *resultP = NULL;
    PyObject *row_i_to_py = NULL;
    PyObject *row_i_result = NULL;
    PyObject *row_cord = NULL;
    int j = 0;
    int i = 0;

    resultP = PyList_New(n);
    if (resultP == NULL){
        PyErr_SetString(PyExc_Exception, "An Error Has Occurred\n");
        return NULL;
    }

    for ( i = 0 ; i < n; i++) {
        row_i_to_py = PyList_New(innerlen);
        if (row_i_to_py == NULL) {
            PyErr_SetString(PyExc_Exception, "An Error Has Occurred\n");
            return NULL;
        }

        for (j = 0; j < innerlen; j++) {
            row_cord = Py_BuildValue("d", convertobj[i][j]);
            PyList_SetItem(row_i_to_py, j, row_cord);
        }
        row_i_result = Py_BuildValue("O", row_i_to_py);
        PyList_SetItem(resultP, i, row_i_result);
    }
    return resultP;
}

/* This function converts PyObject to C object for SymNMF */
double **convertPythonToCSymnmf(PyObject *matrix, int len, int innerlen) {
    PyObject *curr_cordinate_item = NULL;
    PyObject *curr_item_list = NULL;
    int j = 0;
    int ind = 0;
    double curr_cordinate_index = 0.0;
    double *nextVectorToRead = NULL;
    double **res = NULL;

    res =  (double **)malloc(len * sizeof(double *));
    if (res == NULL) {
        PyErr_SetString(PyExc_Exception, "An Error Has Occurred\n");
        return NULL;
    }

    for (j = 0; j < len; j++) {
        nextVectorToRead =(double *) malloc(innerlen * sizeof(double));
        /* check if allocation did not succeed */
        if (nextVectorToRead == NULL) {
            free_matrix_module(res, j - 1);
            PyErr_SetString(PyExc_Exception, "An Error Has Occurred\n");
            return NULL;
        }

        curr_item_list = PyList_GetItem(matrix, j);
        for (ind = 0; ind < innerlen; ind++) {
            curr_cordinate_item = PyList_GetItem(curr_item_list, ind);
            curr_cordinate_index = (double) PyFloat_AsDouble(curr_cordinate_item);
            nextVectorToRead[ind] = curr_cordinate_index;
        }
        res[j] = nextVectorToRead;

    }
    return res;
}

/* Wrapper function for symnmf */
static PyObject *symnmf(PyObject *self, PyObject *args) {
    PyObject *H = NULL;
    PyObject *W = NULL;
    PyObject *resultP = NULL;
    int checkH = 0;
    int checkW = 0;
    int k = 0;
    double **RES_H = NULL;
    double **W_c = NULL;
    double **H_c = NULL;
    int n_curr = 0;

    if (!PyArg_ParseTuple(args, "OOii", &H, &W, &n_curr, &k)) {
        PyErr_SetString(PyExc_Exception, "An Error Has Occurred\n");
        return NULL;
    }
    if (!PyList_Check(H)) {
        PyErr_SetString(PyExc_Exception, "An Error Has Occurred\n");
        return NULL;
    }
    if (!PyList_Check(W)) {
        PyErr_SetString(PyExc_Exception, "An Error Has Occurred\n");
        return NULL;
    }

    checkH = PyList_Size(H);
    if (checkH < 0) {
        PyErr_SetString(PyExc_Exception, "An Error Has Occurred\n");
        return NULL;
    }
    checkW = PyList_Size(W);
    if (checkW < 0) {
        PyErr_SetString(PyExc_Exception, "An Error Has Occurred\n");
        return NULL;
    }

    H_c = convertPythonToCSymnmf(H, n_curr,k);
    if (H_c == NULL){
        PyErr_SetString(PyExc_Exception, "An Error Has Occurred\n");
        return NULL;
    }

    W_c = convertPythonToCSymnmf(W, n_curr,n_curr);
    if (W_c == NULL){
        free_matrix_module(H_c,n_curr);
        PyErr_SetString(PyExc_Exception, "An Error Has Occurred\n");
        return NULL;
    }

    RES_H = symnmf_c(H_c,W_c,n_curr,k);

    free_matrix_module(H_c,n_curr);
    free_matrix_module(W_c,n_curr);

    if (RES_H == NULL){
        PyErr_SetString(PyExc_Exception, "An Error Has Occurred\n");
        return NULL;
    }

    resultP = convertCToPython(RES_H, n_curr,k);
    if (resultP == NULL){
        free_matrix_module(RES_H,n);
        return NULL;
    }

    free_matrix_module(RES_H, n_curr);
    return resultP;

}

/* Wrapper function for sym */
static PyObject *sym(PyObject *self, PyObject *args) {
    PyObject *resultP = NULL;
    double **RES_A = NULL;
    double **X_c = NULL;

    X_c = convertPythonToC_X(self,args);
    if (X_c == NULL){
        return NULL;
    }
    RES_A = sym_c(X_c,n,d);
    free_matrix_module(X_c,n);

    if (RES_A == NULL){
        PyErr_SetString(PyExc_Exception, "An Error Has Occurred\n");
        return NULL;
    }
    resultP = convertCToPython(RES_A,n,n);
    if (resultP == NULL){
        free_matrix_module(RES_A,n);
        return NULL;
    }

    free_matrix_module(RES_A, n);
    return resultP;

}

/* Wrapper function for ddg */
static PyObject *ddg(PyObject *self, PyObject *args) {
    PyObject *resultP = NULL;
    double **RES_D = NULL;
    double **X_c = NULL;

    X_c = convertPythonToC_X(self,args);
    if (X_c == NULL){
        return NULL;
    }

    RES_D = ddg_c(X_c,n,d);
    free_matrix_module(X_c,n);

    if (RES_D == NULL){
        PyErr_SetString(PyExc_Exception, "An Error Has Occurred\n");
        return NULL;
    }

    resultP = convertCToPython(RES_D,n,n);
    if (resultP == NULL){
        free_matrix_module(RES_D,n);
        return NULL;
    }
    free_matrix_module(RES_D, n);
    return resultP;

}

/* Wrapper function for norm */
static PyObject *norm(PyObject *self, PyObject *args) {
    PyObject *resultP = NULL;
    double **RES_W = NULL;
    double **X_c = NULL;

    X_c = convertPythonToC_X(self,args);
    if (X_c == NULL){
        return NULL;
    }

    RES_W = norm_c(X_c,n,d);
    free_matrix_module(X_c,n);

    if (RES_W == NULL){
        PyErr_SetString(PyExc_Exception, "An Error Has Occurred\n");
        return NULL;
    }

    resultP = convertCToPython(RES_W,n,n);
    if (resultP == NULL){
        free_matrix_module(RES_W,n);
        return NULL;
    }

    free_matrix_module(RES_W, n);
    return resultP;

}

/* Module functions */
static PyMethodDef symnmf_FunctionsTable[] = {
        {
                "symnmf", // name exposed to Python
                symnmf, // C wrapper function
                METH_VARARGS, // received variable args
                "Perform full symNMF" // documentation
        }, {
                "sym", // name exposed to Python
                sym, // C wrapper function
                METH_VARARGS, // received variable args (but really just 1)
                "Calculate and output the similarity matrix" // documentation
        }, {
                "ddg", // name exposed to Python
                ddg, // C wrapper function
                METH_VARARGS, // received variable args (but really just 1)
                "Calculate and output the Diagonal Degree Matrix" // documentation
        }, {
                "norm", // name exposed to Python
                norm, // C wrapper function
                METH_VARARGS, // received variable args (but really just 1)
                "Calculate and output the normalized similarity Matrix" // documentation
        },
        {
                NULL, NULL, 0, NULL
        }
};

/* Module definition function */
static struct PyModuleDef modulesymnmfdef = {
        PyModuleDef_HEAD_INIT,
        "mysymnmf",
        NULL,
        -1,
        symnmf_FunctionsTable
};

/* Module initialization function */
PyMODINIT_FUNC PyInit_mysymnmf(void){
    PyObject *m;
    m = PyModule_Create(&modulesymnmfdef);
    if (!m) {
        return NULL;
    }
    return m;
}