cplusplus {{
#include "veins/modules/EdgeComputing/Information/CarInfo.h"
#include "veins/modules/messages/BaseFrame1609_4_m.h"
}}

namespace veins;

class BaseFrame1609_4;
class noncobject CarInfo;

packet CarInfoWSM extends BaseFrame1609_4 {
	CarInfo carInfo;
}