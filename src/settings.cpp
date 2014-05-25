#include "settings.h"
#include "version.h"

namespace mv {

Settings::Settings() : QSettings(VER_COMPANYNAME_STR, VER_PRODUCTNAME_STR) {

}

}
