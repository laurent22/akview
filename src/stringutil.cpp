#include "stringutil.h"

namespace mv {
namespace stringutil {

int compareVersions(const QString& v1, const QString& v2) {
	// Equivalent to v1 - v2

	QStringList a1 = v1.split(".");
	QStringList a2 = v2.split(".");

	while (a1.size() > a2.size()) a1.removeAt(a1.size() - 1);
	while (a2.size() > a1.size()) a2.removeAt(a2.size() - 1);

	for (int i = 0; i < a1.size(); i++) {
		bool ok = true;
		int l1 = a1[i].toLong(&ok); if (!ok) break;
		int l2 = a2[i].toLong(&ok); if (!ok) break;

		if (l1 == l2) continue;
		if (l1 > l2) return +1;
		return -1;
	}

	return 0;
}
}

} // stringutil
