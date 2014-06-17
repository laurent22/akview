#ifndef MV_PROCESSUTIL_H
#define MV_PROCESSUTIL_H

#include <QProcess>
#include <QString>

namespace mv {

namespace processutil {

	bool commandIsAvailable(const QString& name);

}

}

#endif
