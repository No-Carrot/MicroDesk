#ifndef CORE_GLOBAL_H
#define CORE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CORE_LIBRARY_EXPORT)
#  define CORE_LIBRARY Q_DECL_EXPORT
#else
#  define CORE_LIBRARY Q_DECL_IMPORT
#endif

#endif // PLUGIN_GLOBAL_H
