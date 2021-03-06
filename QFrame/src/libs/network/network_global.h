#ifndef NETWORK_GLOBAL_H
#define NETWORK_GLOBAL_H

#include <QtCore/qglobal.h>

#include <QtCore/qglobal.h>

#if !defined(NETWORK_LIBRARY_DISABLE)
#if defined(NETWORK_LIBRARY)
#  define NETWORK_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define NETWORK_SHARED_EXPORT Q_DECL_IMPORT
#endif
#else
#  define NETWORK_SHARED_EXPORT
#endif

#if !defined(NETWORK_NAMESPACE_DISABLE)
#define NETWORK_NAMESPACE NetWork
#endif

#if !defined(NETWORK_NAMESPACE)
#define NETWORK_BEGIN_NAMESPACE
#define NETWORK_END_NAMESPACE
#define NETWORK_USE_NAMESPACE
#define NETWORK_PREPEND_NAMESPACE(name) name
#else
#define NETWORK_BEGIN_NAMESPACE namespace NETWORK_NAMESPACE {
#define NETWORK_END_NAMESPACE }
#define NETWORK_USE_NAMESPACE using namespace NETWORK_NAMESPACE;
#define NETWORK_PREPEND_NAMESPACE(name) NETWORK_NAMESPACE::name
#endif // NETWORK_NAMESPACE

NETWORK_BEGIN_NAMESPACE
// nothing
NETWORK_END_NAMESPACE

#endif // NETWORK_GLOBAL_H
