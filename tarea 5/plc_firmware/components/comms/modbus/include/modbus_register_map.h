#ifndef MODBUS_REGISTER_MAP_H
#define MODBUS_REGISTER_MAP_H

#include <stdint.h>

#define MB_COIL_DO_BASE               0
#define MB_COIL_RUN_CMD               16
#define MB_COIL_STOP_CMD              17

#define MB_DISCRETE_DI_BASE           0
#define MB_DISCRETE_ALARM_ACTIVE      16

#define MB_HREG_AO0_SETPOINT          0
#define MB_HREG_AO1_SETPOINT          1
#define MB_HREG_SYSTEM_MODE           10
#define MB_HREG_IP_OCTET_1            20
#define MB_HREG_IP_OCTET_2            21
#define MB_HREG_IP_OCTET_3            22
#define MB_HREG_IP_OCTET_4            23
#define MB_HREG_MQTT_PORT             30

#define MB_IREG_AI0_VALUE             0
#define MB_IREG_AI1_VALUE             1
#define MB_IREG_VOLTAGE               10
#define MB_IREG_CURRENT               11
#define MB_IREG_POWER                 12
#define MB_IREG_FREQUENCY             13

#endif
