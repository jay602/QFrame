#ifndef PUREMVC_GLOBAL_H
#define PUREMVC_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PUREMVC_LIBRARY)
#  define PUREMVC_EXPORT Q_DECL_EXPORT
#else
#  define PUREMVC_EXPORT Q_DECL_IMPORT
#endif

#endif // PUREMVC_GLOBAL_H
