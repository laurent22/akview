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

bool NaturalSortCompare::operator() (const QString& s1, const QString& s2) {
	if (s1 == "" || s2 == "") return s1 < s2;

	// Move to either the first difference between the strings
	// or to the first digit.
	int startIndex = -1;
	int length = s1.length() > s2.length() ? s2.length() : s1.length();
	for (int i = 0; i < length; i++) {
		QChar c1 = s1[i];
		QChar c2 = s2[i];
		if (c1 != c2 || (isNumber(c1) && isNumber(c2))) {
			startIndex = i;
			break;
		}
	}

	// If the strings are the same, exit now.
	if (startIndex < 0) return s1 < s2;

	// Now extract the numbers, if any, from the two strings.
	QString sn1;
	QString sn2;
	bool done1 = false;
	bool done2 = false;
	length = s1.length() < s2.length() ? s2.length() : s1.length();
	for (int i = startIndex; i < length; i++) {
		if (!done1 && i < s1.length()) {
			if (isNumber(s1[i])) {
				sn1 += QString(s1[i]);
			} else {
				done1 = true;
			}
		}

		if (!done2 && i < s2.length()) {
			if (isNumber(s2[i])) {
				sn2 += QString(s2[i]);
			} else {
				done2 = true;
			}
		}

		if (done1 && done2) break;
	}

	// If one of the string doesn't contain a number, use a regular comparison.
	if (sn1 == "" || sn2 == "") return s1 < s2;

	return sn1.toInt() < sn2.toInt();
}


}
} // stringutil
