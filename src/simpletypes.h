#ifndef SIMPLETYPES_H
#define SIMPLETYPES_H

#include <map>
#include <vector>
#include <QKeySequence>
#include <QLabel>
#include <QWidget>

typedef std::vector<int> IntVector;
typedef std::map<QWidget*, bool> QWidgetBoolMap;
typedef std::map<QString, QMenu*> QStringQMenuMap;
typedef std::vector<float> FloatVector;
typedef std::map<QString, QLabel*> QStringQLabelMap;

#endif // SIMPLETYPES_H
