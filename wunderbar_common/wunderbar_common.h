
#ifndef WUNDERBAR_COMMON_H_
#define WUNDERBAR_COMMON_H_

#include "types.h"
#include "ble_types.h"
#include "device_manager_cnfg.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAX_CLIENTS                      DEVICE_MANAGER_MAX_CONNECTIONS  /**< Max number of clients. */
#define CONNECTION_INTERVAL_MS           220
#define SCAN_INTERVAL_MS                 CONNECTION_INTERVAL_MS
#define SCAN_WINDOW_MS                   SCAN_INTERVAL_MS - 20
#define ADV_INTERVAL_MS                  SCAN_WINDOW_MS


#define SUPERVISION_TIMEOUT_MS           3000
#define BATTERY_LEVEL_MEAS_INTERVAL_MS   30000

#define SLAVE_LATENCY                    5                                                     /**< Determines slave latency in counts of connection events. */

#define SCAN_INTERVAL                    MSEC_TO_UNITS(SCAN_INTERVAL_MS, UNIT_0_625_MS)        /**< Determines scan interval in units of 0.625 millisecond. */
#define SCAN_WINDOW                      MSEC_TO_UNITS(SCAN_WINDOW_MS, UNIT_0_625_MS)          /**< Determines scan window in units of 0.625 millisecond. */

#define MIN_CONNECTION_INTERVAL          MSEC_TO_UNITS(CONNECTION_INTERVAL_MS, UNIT_1_25_MS)   /**< Determines maximum connection interval in millisecond. */
#define MAX_CONNECTION_INTERVAL          MSEC_TO_UNITS(CONNECTION_INTERVAL_MS, UNIT_1_25_MS)   /**< Determines maximum connection interval in millisecond. */
#define SUPERVISION_TIMEOUT              MSEC_TO_UNITS(SUPERVISION_TIMEOUT_MS, UNIT_10_MS)     /**< Determines supervision time-out in units of 10 millisecond. */
#define BATTERY_LEVEL_MEAS_INTERVAL      APP_TIMER_TICKS(BATTERY_LEVEL_MEAS_INTERVAL_MS, APP_TIMER_PRESCALER)  /**< Battery level measurement interval (ticks). */

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MAX_DISCOVERY_SERVICES                            3
/**@brief Service Relayr UUID. */
#define SHORT_SERVICE_RELAYR_UUID                         0x2000

/**@brief Service Config UUID. */
#define SHORT_SERVICE_CONFIG_UUID                         0x2001

/**@brief Service Relayr with no MITM protection UUID. */
#define SHORT_SERVICE_RELAYR_OPEN_COMM_UUID               0x2002

/**@brief Characteristic UUIDs of Relayr Service. */
#define CHARACTERISTIC_SENSOR_ID_UUID                     0x2010
#define CHARACTERISTIC_SENSOR_BEACON_FREQUENCY_UUID       0x2011
#define CHARACTERISTIC_SENSOR_FREQUENCY_UUID              0x2012
#define CHARACTERISTIC_SENSOR_LED_STATE_UUID              0x2013
#define CHARACTERISTIC_SENSOR_THRESHOLD_UUID              0x2014
#define CHARACTERISTIC_SENSOR_CONFIG_UUID                 0x2015
#define CHARACTERISTIC_SENSOR_DATA_R_UUID                 0x2016
#define CHARACTERISTIC_SENSOR_DATA_W_UUID                 0x2017

/**< Passkey change is not available from main bluetooth module. */
#define CHARACTERISTIC_SENSOR_PASSKEY_UUID                0x2018
#define CHARACTERISTIC_SENSOR_MITM_REQ_FLAG_UUID          0x2019

/**< Number of characteristics in Relayr Service. */
#define NUMBER_OF_RELAYR_CHARACTERISTICS                  8
#define MAX_NUMBER_OF_CHARACTERISTICS                     NUMBER_OF_RELAYR_CHARACTERISTICS + 4

/**< Number of sensors. */
#define NUMBER_OF_SENSORS                                 6

/**@brief Characteristic UUIDs of BLE SIG Service. */
#define CHARACTERISTIC_BATTERY_LEVEL_UUID                 BLE_UUID_BATTERY_LEVEL_CHAR
#define CHARACTERISTIC_MANUFACTURER_NAME_UUID             BLE_UUID_MANUFACTURER_NAME_STRING_CHAR
#define CHARACTERISTIC_HARDWARE_REVISION_UUID             BLE_UUID_HARDWARE_REVISION_STRING_CHAR
#define CHARACTERISTIC_FIRMWARE_REVISION_UUID             BLE_UUID_FIRMWARE_REVISION_STRING_CHAR

/**< List of used characteristics UUID. */
#define LIST_OF_SENSOR_CHARS  {                                               \
                                CHARACTERISTIC_SENSOR_ID_UUID,                \
                                CHARACTERISTIC_SENSOR_BEACON_FREQUENCY_UUID,  \
                                CHARACTERISTIC_SENSOR_FREQUENCY_UUID,         \
                                CHARACTERISTIC_SENSOR_LED_STATE_UUID,         \
                                CHARACTERISTIC_SENSOR_THRESHOLD_UUID,         \
                                CHARACTERISTIC_SENSOR_CONFIG_UUID,            \
                                CHARACTERISTIC_SENSOR_DATA_R_UUID,            \
                                CHARACTERISTIC_SENSOR_DATA_W_UUID,            \
                                CHARACTERISTIC_BATTERY_LEVEL_UUID,            \
                                CHARACTERISTIC_MANUFACTURER_NAME_UUID,        \
                                CHARACTERISTIC_HARDWARE_REVISION_UUID,        \
                                CHARACTERISTIC_FIRMWARE_REVISION_UUID         \
                              }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**@brief Sensors Device Names. */
#define DEVICE_NAME_HTU             "WunderbarHTU"
#define DEVICE_NAME_GYRO            "WunderbarGYRO"
#define DEVICE_NAME_LIGHT           "WunderbarLIGHT"
#define DEVICE_NAME_MIC             "WunderbarMIC"
#define DEVICE_NAME_BRIDGE          "WunderbarRATH"
#define DEVICE_NAME_IR              "WunderbarIR"
#define DEVICE_NAME_CFG_APP         "WunderbarApp"

/**@brief Master Bluetooth Device Name. */
#define DEVICE_NAME_MASTER_BLE      "WunderbarBar"

/**@brief Max lenght of Device Name. */
#define BLE_DEVNAME_MAX_LEN         14

/**@brief List of Sensors Device Names. */
#define LIST_OF_SENSOR_NAMES  {                      \
                                DEVICE_NAME_HTU,     \
                                DEVICE_NAME_GYRO,    \
                                DEVICE_NAME_LIGHT,   \
                                DEVICE_NAME_MIC,     \
                                DEVICE_NAME_BRIDGE,  \
                                DEVICE_NAME_IR,      \
                                DEVICE_NAME_CFG_APP, \
                              };

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef uint32_t beaconFrequency_t;
typedef uint32_t frequency_t;

#define DEVICE_UUID_LEN 16
#define DEVICE_PASS_LEN 8
typedef uint8_t  sensorID_t[DEVICE_UUID_LEN];
typedef uint8_t  passkey_t[DEVICE_PASS_LEN];

typedef bool     led_state_t;
typedef bool     security_level_t;

/////////////////////////////////////////////////////////////

/**@brief Threshold records. */

typedef struct
{
    uint8_t sbl;
    int8_t  low;
    int8_t  high;
}
__attribute__ ((__packed__)) threshold_int8_t;

typedef struct
{
    uint16_t sbl;
    int16_t  low;
    int16_t  high;
}
__attribute__ ((__packed__)) threshold_int16_t;

typedef struct
{
    uint32_t sbl;
    int32_t  low;
    int32_t  high;
}
__attribute__ ((__packed__)) threshold_int32_t;

typedef struct
{
    float sbl;
    float low;
    float high;
}
threshold_float_t;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**@brief HTU Sensor. */

typedef struct
{
    threshold_int16_t  temperature;
    threshold_int16_t  humidity;
}
__attribute__ ((__packed__)) sensor_htu_threshold_t;

/////////////////////////////////////////////////////////////

typedef enum
{
    HTU21D_RH_12_TEMP14 = 0,
    HTU21D_RH_8_TEMP12,
    HTU21D_RH_10_TEMP13,
    HTU21D_RH_11_TEMP11,
}
sensor_htu_config_t;

/////////////////////////////////////////////////////////////

typedef struct
{
    int16_t temperature;
    int16_t humidity;
}
__attribute__ ((__packed__)) sensor_htu_data_t;

/////////////////////////////////////////////////////////////

typedef struct
{
    sensorID_t              sensorID         __attribute__((aligned(4)));
    beaconFrequency_t       beaconFrequency  __attribute__((aligned(4)));
    frequency_t             frequency        __attribute__((aligned(4)));
    led_state_t             led_state        __attribute__((aligned(4)));
    sensor_htu_threshold_t  threshold        __attribute__((aligned(4)));
    sensor_htu_config_t     config           __attribute__((aligned(4)));
    passkey_t               passkey          __attribute__((aligned(4)));
    security_level_t        mitm_req_flag    __attribute__((aligned(4)));
    sensor_htu_data_t       data;
}
sensor_htu_t;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**@brief GYRO Sensor. */

typedef struct
{
    threshold_int32_t gyro;
    threshold_int16_t acc;
}
__attribute__ ((__packed__)) sensor_gyro_threshold_t;

/////////////////////////////////////////////////////////////

typedef enum
{
    GYRO_FULL_SCALE_250DPS = 0,
    GYRO_FULL_SCALE_500DPS,
    GYRO_FULL_SCALE_1000DPS,
    GYRO_FULL_SCALE_2000DPS
}
sensor_gyro_GyroFullScale_t;

typedef enum
{
    ACC_FULL_SCALE_2G = 0,
    ACC_FULL_SCALE_4G,
    ACC_FULL_SCALE_8G,
    ACC_FULL_SCALE_16G
}
sensor_gyro_AccFullScale_t;

typedef struct
{
    sensor_gyro_GyroFullScale_t gyro_full_scale;
    sensor_gyro_AccFullScale_t  acc_full_scale;
}
__attribute__ ((__packed__)) sensor_gyro_config_t;

/////////////////////////////////////////////////////////////

typedef struct
{
    int32_t x;
    int32_t y;
    int32_t z;
}
__attribute__ ((__packed__)) sensor_gyro_GyroCoord_t;

typedef struct
{
    int16_t x;
    int16_t y;
    int16_t z;
}
__attribute__ ((__packed__)) sensor_gyro_AccCoord_t;

typedef struct
{
    sensor_gyro_GyroCoord_t gyro;
    sensor_gyro_AccCoord_t acc;
}
__attribute__ ((__packed__)) sensor_gyro_data_t;

/////////////////////////////////////////////////////////////

typedef struct
{
    sensorID_t              sensorID         __attribute__((aligned(4)));
    beaconFrequency_t       beaconFrequency  __attribute__((aligned(4)));
    frequency_t             frequency        __attribute__((aligned(4)));
    led_state_t             led_state        __attribute__((aligned(4)));
    sensor_gyro_threshold_t threshold        __attribute__((aligned(4)));
    sensor_gyro_config_t    config           __attribute__((aligned(4)));
    passkey_t               passkey          __attribute__((aligned(4)));
    security_level_t        mitm_req_flag    __attribute__((aligned(4)));
    sensor_gyro_data_t      data;
}
sensor_gyro_t;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**@brief Color Proximity Sensor. */

typedef struct
{
    threshold_int16_t white;
    threshold_int16_t proximity;
}
__attribute__ ((__packed__)) sensor_lightprox_threshold_t;

/////////////////////////////////////////////////////////////

typedef enum
{
    RGBC_GAIN_1     = 0x00,
    RGBC_GAIN_4     = 0x01,
    RGBC_GAIN_16    = 0x02,
    RGBC_GAIN_60    = 0x03
}
sensor_lightprox_rgbc_gain_t;

typedef enum
{
    PROX_DRIVE_12_5_MA = 0xC0,
    PROX_DRIVE_25_MA   = 0x80,
    PROX_DRIVE_50_MA   = 0x40,
    PROX_DRIVE_100_MA  = 0x00
}
sensor_lightprox_prox_drive_t;

typedef struct
{
    sensor_lightprox_rgbc_gain_t    rgbc_gain;
    sensor_lightprox_prox_drive_t   prox_drive;
}
__attribute__ ((__packed__)) sensor_lightprox_config_t;

/////////////////////////////////////////////////////////////

typedef struct
{
    uint16_t r;
    uint16_t g;
    uint16_t b;
    uint16_t white;
    uint16_t proximity;
}
sensor_lightprox_data_t;

/////////////////////////////////////////////////////////////

typedef struct
{
    sensorID_t                   sensorID          __attribute__((aligned(4)));
    beaconFrequency_t            beaconFrequency   __attribute__((aligned(4)));
    frequency_t                  frequency         __attribute__((aligned(4)));
    led_state_t                  led_state         __attribute__((aligned(4)));
    sensor_lightprox_threshold_t threshold         __attribute__((aligned(4)));
    sensor_lightprox_config_t    config            __attribute__((aligned(4)));
    passkey_t                    passkey           __attribute__((aligned(4)));
    security_level_t             mitm_req_flag     __attribute__((aligned(4)));
    sensor_lightprox_data_t      data;
}
sensor_lightprox_t;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**@brief IR transmit board. */

typedef uint8_t sensor_ir_data_t;

typedef struct
{
    sensorID_t        sensorID           __attribute__((aligned(4)));
    beaconFrequency_t beaconFrequency    __attribute__((aligned(4)));
    led_state_t       led_state          __attribute__((aligned(4)));
    passkey_t         passkey            __attribute__((aligned(4)));
    security_level_t  mitm_req_flag      __attribute__((aligned(4)));
    sensor_ir_data_t  data;
}
sensor_ir_t;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**@brief Microphone Sensor. */

typedef struct
{
    threshold_int16_t mic_level;
}
sensor_microphone_threshold_t;

typedef struct
{
    int16_t mic_level;
}
sensor_microphone_data_t;

typedef struct
{
    sensorID_t                    sensorID         __attribute__((aligned(4)));
    beaconFrequency_t             beaconFrequency  __attribute__((aligned(4)));
    frequency_t                   frequency        __attribute__((aligned(4)));
    led_state_t                   led_state        __attribute__((aligned(4)));
    passkey_t                     passkey          __attribute__((aligned(4)));
    sensor_microphone_threshold_t threshold        __attribute__((aligned(4)));
    security_level_t              mitm_req_flag    __attribute__((aligned(4)));
    sensor_microphone_data_t      data;
}
sensor_microphone_t;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**@brief Bridge Sensor. */

#define BRIDGE_PAYLOAD_SIZE  19
#define BRIDGE_HEDER_SIZE    2
#define BRIDGE_CRC16_SIZE    2
#define BRIDGE_PACKET_SIZE   BRIDGE_PAYLOAD_SIZE + BRIDGE_HEDER_SIZE + BRIDGE_CRC16_SIZE

typedef struct
{
    uint8_t payload_length;
    uint8_t payload[BRIDGE_PAYLOAD_SIZE];
}
__attribute__ ((__packed__)) sensor_bridge_data_t;

typedef struct
{
    uint32_t baud_rate;
}
sensor_bridge_config_t;

typedef struct
{
    sensorID_t                 sensorID          __attribute__((aligned(4)));
    beaconFrequency_t          beaconFrequency   __attribute__((aligned(4)));
    led_state_t                led_state         __attribute__((aligned(4)));
    sensor_bridge_config_t     config            __attribute__((aligned(4)));
    passkey_t                  passkey           __attribute__((aligned(4)));
    security_level_t           mitm_req_flag     __attribute__((aligned(4)));
    sensor_bridge_data_t       data_up;
    sensor_bridge_data_t       data_down;
}
sensor_bridge_t;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef enum
{
    DATA_ID_DEV_HTU             = 0x0,
    DATA_ID_DEV_GYRO            = 0x1,
    DATA_ID_DEV_LIGHT           = 0x2,
    DATA_ID_DEV_SOUND           = 0x3,
    DATA_ID_DEV_BRIDGE          = 0x4,
    DATA_ID_DEV_IR              = 0x5,

    // DATA_ID_DEV_CFG_APP         = 0x6,

    DATA_ID_DEV_CENTRAL         = MAX_CLIENTS,

    DATA_ID_RESPONSE_OK         = 0x64,
    DATA_ID_RESPONSE_ERROR      = 0x65,
    DATA_ID_RESPONSE_BUSY       = 0x66,
    DATA_ID_RESPONSE_NOT_FOUND  = 0x67,

    DATA_ID_CONFIG              = 0xC8,

    DATA_ID_ERROR               = 0xFF,
}
data_id_t;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef enum
{
    FIELD_ID_CHAR_SENSOR_ID                  = 0,
    FIELD_ID_CHAR_SENSOR_BEACON_FREQUENCY    = 1,
    FIELD_ID_CHAR_SENSOR_FREQUENCY           = 2,
    FIELD_ID_CHAR_SENSOR_LED_STATE           = 3,
    FIELD_ID_CHAR_SENSOR_THRESHOLD           = 4,
    FIELD_ID_CHAR_SENSOR_CONFIG              = 5,
    FIELD_ID_CHAR_SENSOR_DATA_R              = 6,
    FIELD_ID_CHAR_SENSOR_DATA_W              = 7,
    FIELD_ID_CHAR_BATTERY_LEVEL              = 8,
    FIELD_ID_CHAR_MANUFACTURER_NAME          = 9,
    FIELD_ID_CHAR_HARDWARE_REVISION          = 10,
    FIELD_ID_CHAR_FIRMWARE_REVISION          = 11,
    FIELD_ID_SENSOR_STATUS                   = 12,
}
field_id_char_index_t;

typedef enum
{
    FIELD_ID_CONFIG_HTU_PASS                 = 0,
    FIELD_ID_CONFIG_GYRO_PASS                = 1,
    FIELD_ID_CONFIG_LIGHT_PASS               = 2,
    FIELD_ID_CONFIG_SOUND_PASS               = 3,
    FIELD_ID_CONFIG_BRIDGE_PASS              = 4,
    FIELD_ID_CONFIG_IR_PASS                  = 5,
    FIELD_ID_CONFIG_WIFI_SSID                = 6,
    FIELD_ID_CONFIG_WIFI_PASS                = 7,
    FIELD_ID_CONFIG_MASTER_MODULE_ID         = 8,
    FIELD_ID_CONFIG_MASTER_MODULE_SEC        = 9,
    FIELD_ID_CONFIG_MASTER_MODULE_URL        = 10,

    FIELD_ID_CONFIG_START                    = 11,
    FIELD_ID_CONFIG_COMPLETE                 = 12,
    FIELD_ID_CONFIG_STOP                     = 13,
    FIELD_ID_CONFIG_ACK                      = 14,
    FIELD_ID_CONFIG_ERROR                    = 15,

    FIELD_ID_RUN                             = 16,
    FIELD_ID_CONFIG_ADD_DISCOVERY_SERVICE    = 17,
    FIELD_ID_CONFIG_ADD_CLIENT_CHARACTERISTC = 18,
    FIELD_ID_CONFIG_CLIENT_NAME              = 19,
    FIELD_ID_CONFIG_CLIENT_PASS              = 20,
    FIELD_ID_CONFIG_CLIENT_UUID              = 21,
    FIELD_ID_CONFIG_START_DISCOVERY          = 22,
    FILED_ID_RUN_WRITE_RSP_OK                = 23,
    FILED_ID_CONFIG_DISCOVERY_COMPLETE       = 24,
    FILED_ID_CONFIG_CHAR_DONE                = 25,
    FILED_ID_READ_CHAR                       = 26,
    FILED_ID_WRITE_CHAR                      = 27,
    FIELD_ID_RUN_ERROR                       = 0xFF,
}
field_id_config_t;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef enum
{
    OPERATION_WRITE  = 0,
    OPERATION_READ   = 1,
    OPERATION_NONE   = 0xFF
}
operation_t;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum
{
    USE_NEVER = 0x0,
    USE_ONBOARD = 0x1,
    USE_RUN = 0x2,
    USE_ALWAYS = 0x3,
}
use_mode_t;

typedef struct
{
    uint16_t    uuid;
    uint8_t     type;
    uint8_t use_mode;
}
__attribute__((packed)) serivce_desc_t;

typedef enum
{
    ACCESS_NONE = 0x0,
    ACCESS_READ = 0x1,
    ACCESS_WRITE = 0x2,
    ACCESS_RW = 0x3,
    ACCESS_VERIFY = 0x4,
}
access_mode_t;

typedef struct
{
    uint16_t    uuid;
}
__attribute__((packed)) char_desc_t;

typedef struct
{
    uint8_t mac[6];
    uint8_t name[BLE_DEVNAME_MAX_LEN + 1];
} __attribute__((packed)) client_desc_t;

#define SPI_PACKET_DATA_SIZE 20

typedef struct
{
    data_id_t   data_id;
    uint8_t     field_id;
    uint8_t     client_index;
    operation_t operation;
    uint8_t     data[SPI_PACKET_DATA_SIZE];
}
__attribute__((packed)) spi_frame_t;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint8_t sensors_get_msg_size(data_id_t sens_name, field_id_char_index_t msg_type);
uint8_t sensor_get_name_index(const uint8_t * device_name);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // WUNDERBAR_COMMON_H_

/** @} */

