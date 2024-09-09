#ifndef PYCALLER_H
#define PYCALLER_H
#pragma push_macro("slots")
#undef slots
#include <Python.h>
#pragma pop_macro("slots")
#include <QObject>
#include <QMap>
#include "util.h"


class PyCaller: public QObject
{
    Q_OBJECT

public:
    PyCaller();
    ~PyCaller();
    void init_python();
    void try_on(QString pimg_fn, QMap<QString, QString>& topBottomFiles, bool tuck);

private:
    PyObject* m_pInstance;
    bool m_isSuccess;
    JsonReader m_jsonConfig;
};

#endif // PYCALLER_H
