#include "pycaller.h"
#include <QCoreApplication>
#include <QDebug>

PyCaller::PyCaller(): m_pInstance(nullptr), m_isSuccess(true)
{
    m_jsonConfig = JsonReader();
    init_python();
}

void PyCaller::init_python()
{
    Py_Initialize();
    if(!Py_IsInitialized())
    {
        m_isSuccess = false;
        qDebug() << "failed to init python" << Qt::endl;
    }
    else
    {
        PyObject* pModule = nullptr;
        PyObject* pDict = nullptr;
        PyObject* pClassCalc = nullptr;
        PyObject* pConstruct = nullptr;

        //add path to python files
        PyRun_SimpleString("import sys");
        QString pythonDir = m_jsonConfig.getValue("python_script_path");
        QString cmdStr = "sys.path.append('" + pythonDir + "')";
        PyRun_SimpleString(cmdStr.toStdString().data());

        pModule = PyImport_ImportModule("tryon");
        if (!pModule)
        {
            m_isSuccess = false;
            qDebug() << "failed to import python module (tryon.py)!" << Qt::endl;
        }

        pDict = PyModule_GetDict(pModule);
        if (!pDict)
        {
            m_isSuccess = false;
            qDebug() << "can't find dictionary!" <<Qt::endl;
        }

        pClassCalc = PyDict_GetItemString(pDict, "TryOn");
        if (!pClassCalc)
        {
            m_isSuccess = false;
            qDebug() << "Can't find try_on class!" << Qt::endl;
        }

        pConstruct = PyInstanceMethod_New(pClassCalc);
        if (!pConstruct)
        {
            m_isSuccess = false;
            qDebug() << "Can't find try_on class constructor!" << Qt::endl;
        }

        QString configFilePath = pythonDir + "/"+ "configForPython.json";

        PyObject* arg1 = PyUnicode_FromString(configFilePath.toStdString().data());

        PyObject* args = PyTuple_New(1);
        PyTuple_SetItem(args, 0, arg1);
        if (PyErr_Occurred())
        {
            m_isSuccess = false;
            PyErr_Print();
        }

        m_pInstance = PyObject_CallObject(pConstruct, args);
        if (PyErr_Occurred())
        {
            m_isSuccess = false;
            PyErr_Print();
        }

        // Py_XDECREF(pModule);
        // Py_XDECREF(pDict);
        // Py_XDECREF(pClassCalc);
        // Py_XDECREF(pConstruct);
        // Py_XDECREF(args);
        // Py_XDECREF(arg1);
    }
}

void PyCaller::try_on(QString pimg_fn, QMap<QString, QString>& topBottomFiles, bool tuck)
{
    if (!m_isSuccess)
        return;

    PyObject* pid_arg = PyTuple_New(3);
    int idx;
    idx = pimg_fn.lastIndexOf("/");
    pimg_fn = pimg_fn.right(pimg_fn.length()-(idx+1));
    PyTuple_SetItem(pid_arg, 0, Py_BuildValue("s", pimg_fn.toStdString().data()));
    PyTuple_SetItem(pid_arg, 1, Py_BuildValue("z", NULL));
    PyTuple_SetItem(pid_arg, 2, Py_BuildValue("z", NULL));

    PyObject* gid1 = PyTuple_New(3);
    QString topImgPath = topBottomFiles.value("top");
    idx = topImgPath.lastIndexOf("/");
    QString fnTop = topImgPath.right(topImgPath.length()-(idx+1));

    PyTuple_SetItem(gid1, 0, Py_BuildValue("s", fnTop.toStdString().data()));
    PyTuple_SetItem(gid1, 1, Py_BuildValue("z", NULL));
    PyTuple_SetItem(gid1, 2, Py_BuildValue("i", 5));

    PyObject* gid2 = PyTuple_New(3);
    QString bottomImgPath = topBottomFiles.value("bottom");
    idx = bottomImgPath.lastIndexOf("/");
    QString fnBottom = bottomImgPath.right(bottomImgPath.length()-(idx+1));
    PyTuple_SetItem(gid2, 0, Py_BuildValue("s", fnBottom.toStdString().data()));
    if (PyErr_Occurred())
        PyErr_Print();

    PyTuple_SetItem(gid2, 1, Py_BuildValue("z", NULL));
    PyTuple_SetItem(gid2, 2, Py_BuildValue("i", 1));

    PyObject* gids_arg = PyList_New(2);
    PyList_SetItem(gids_arg, 0, gid1);
    PyList_SetItem(gids_arg, 1, gid2);

    PyObject_CallMethod(m_pInstance, "generator", "(OOO)", pid_arg, gids_arg, tuck?Py_True:Py_False);
    if (PyErr_Occurred())
        PyErr_Print();
}

PyCaller::~PyCaller()
{
    Py_XDECREF(m_pInstance);
    Py_Finalize();
}
