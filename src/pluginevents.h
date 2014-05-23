#ifndef PLUGINEVENTS_H
#define PLUGINEVENTS_H

#include <QString>

struct KeypressedEvent {
	int keyCode;
	QString text;
	int modifiers;
	bool accepted;

	KeypressedEvent() {
		accepted = false;
	}
};

#endif // PLUGINEVENTS_H
