#include <MAUtil/String.h>

namespace Cloudbase {

/**
 * This class is used internally to execute calls to the cloudbase.io APIs. Each request is self-contained within a
 * CBHttpConnection object. This way the CBHelper object can execute multiple requests at the same time.
 */
class CBHttpConnectionParameter {
public:
	CBHttpConnectionParameter();

	MAUtil::String name;
	MAUtil::String value;
	MAHandle fileData;
	bool isFile;
};
}
