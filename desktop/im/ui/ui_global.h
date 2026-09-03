#ifndef UI_GLOBAL_H
#define UI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(UI_LIBRARY_EXPORT)
#  define UI_EXPORT Q_DECL_EXPORT
#else
#  define UI_EXPORT Q_DECL_IMPORT
#endif

#endif
