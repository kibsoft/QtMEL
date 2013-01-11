#ifndef QVACL_GLOBAL_H
#define QVACL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QVACL_LIBRARY)
#  define QVACLSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QVACLSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QVACL_GLOBAL_H
