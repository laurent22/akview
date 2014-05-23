#ifndef PLUGINEVENTS_H
#define PLUGINEVENTS_H

#include <QString>

namespace mv {

struct KeypressedEvent {
	int keyCode;
	QString text;
	int modifiers;
	bool accepted;

	KeypressedEvent() {
		accepted = false;
	}
};

}

#endif // PLUGINEVENTS_H
