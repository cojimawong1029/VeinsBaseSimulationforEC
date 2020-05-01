cplusplus {{
#include "veins/modules/messages/BaseFrame1609_4_m.h"
}}

namespace veins;

class BaseFrame1609_4;

packet CarControl extends BaseFrame1609_4 {
//	double TargetSpeed;
	int VehicleId[8];
	double speed[8];
}