#ifndef STRINGUTIL_H
#define STRINGUTIL_H

namespace mv {

namespace stringutil {

	int compareVersions(const QString& v1, const QString& v2);

	struct NaturalSortCompare {
		inline bool isNumber(QChar c) { return c >= '0' && c <= '9'; }
		bool operator() (const QString& s1, const QString& s2);
	};

} // stringutil

}

#endif // STRINGUTIL_H
