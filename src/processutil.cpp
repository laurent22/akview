#include "processutil.h"

namespace mv {
namespace processutil {

bool commandIsAvailable(const QString& name) {
	QProcess p;
	// TOOD: win version
	p.start("which", QStringList() << name);
	p.waitForFinished();
	return p.exitStatus() == QProcess::NormalExit && p.exitCode() == 0;
}

}
}