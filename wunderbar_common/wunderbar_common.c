
/** @file   wunderbar_common.c
 *  @brief
 *
 *  @author MikroElektronika
 *  @bug    No known bugs.
 */

#include "wunderbar_common.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**@brief Extern variables. */

extern const uint8_t     client_device_names[MAX_CLIENTS][BLE_DEVNAME_MAX_LEN + 1];
// extern const char_desc_t client_char_uuids[MAX_CLIENTS][NUMBER_OF_RELAYR_CHARACTERISTICS + 4];

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**@brief This function returns size of threshold characteristic for specific sensor.
 *
 * @param sens_id    Sensor ID.
 *
 * @return    Size of threshold characteristic.
 *
 */

static uint8_t sensors_get_threshold_size(data_id_t sens_id)
{
    switch (sens_id)
    {
    case DATA_ID_DEV_HTU :
        return (sizeof(sensor_htu_threshold_t));

    case DATA_ID_DEV_GYRO :
        return (sizeof(sensor_gyro_threshold_t));

    case DATA_ID_DEV_LIGHT :
        return (sizeof(sensor_lightprox_threshold_t));

    case DATA_ID_DEV_SOUND :
        return (sizeof(sensor_microphone_threshold_t));

    case DATA_ID_DEV_BRIDGE :
    case DATA_ID_DEV_IR :
    default :
        return 0;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**@brief This function returns size of config characteristic for specific sensor.
 *
 * @param sens_id    Sensor ID.
 *
 * @return    Size of config characteristic.
 *
 */

static uint8_t sensors_get_config_size(data_id_t sens_id)
{
    switch (sens_id)
    {
    case DATA_ID_DEV_HTU :
        return (sizeof(sensor_htu_config_t));

    case DATA_ID_DEV_GYRO :
        return (sizeof(sensor_gyro_config_t));

    case DATA_ID_DEV_LIGHT :
        return (sizeof(sensor_lightprox_config_t));

    case DATA_ID_DEV_BRIDGE :
        return (sizeof(sensor_bridge_config_t));

    case DATA_ID_DEV_SOUND :
    case DATA_ID_DEV_IR :
    default :
        return 0;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**@brief This function returns size of data characteristic for specific sensor.
 *
 * @param sens_id    Sensor ID.
 *
 * @return    Size of data characteristic.
 *
 */

static uint8_t sensors_get_data_size(data_id_t sens_id)
{
    switch (sens_id)
    {
    case DATA_ID_DEV_HTU :
        return (sizeof(sensor_htu_data_t));

    case DATA_ID_DEV_GYRO :
        return (sizeof(sensor_gyro_data_t));

    case DATA_ID_DEV_LIGHT :
        return (sizeof(sensor_lightprox_data_t));

    case DATA_ID_DEV_SOUND :
        return (sizeof(sensor_microphone_data_t));

    case DATA_ID_DEV_BRIDGE :
        return (sizeof(sensor_bridge_data_t));

    case DATA_ID_DEV_IR :
        return (sizeof(uint8_t));

    default :
        return 0;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**@brief This function returns message size of "msg_type" characteristic for specific sensor.
 *
 * @param sens_id    Sensor ID.
 * @param msg_type   Message type.
 *
 * @return    Message size.
 *
 */

uint8_t sensors_get_msg_size(data_id_t sens_id, field_id_char_index_t msg_type)
{
    switch (msg_type)
    {

    case FIELD_ID_CHAR_SENSOR_ID:
        return (sizeof(sensorID_t));

    case FIELD_ID_CHAR_SENSOR_BEACON_FREQUENCY:
        return (sizeof(beaconFrequency_t));

    case FIELD_ID_CHAR_SENSOR_FREQUENCY:
        return (sizeof(frequency_t));

    case FIELD_ID_CHAR_SENSOR_LED_STATE:
        return (sizeof(led_state_t));

    case FIELD_ID_CHAR_SENSOR_THRESHOLD:
        return (sensors_get_threshold_size(sens_id));

    case FIELD_ID_CHAR_SENSOR_CONFIG:
        return (sensors_get_config_size(sens_id));

    case FIELD_ID_CHAR_SENSOR_DATA_R:
        return (sensors_get_data_size(sens_id));

    case FIELD_ID_CHAR_SENSOR_DATA_W:
        return (sensors_get_data_size(sens_id));

    case FIELD_ID_CHAR_BATTERY_LEVEL:
        return (1);

    case FIELD_ID_SENSOR_STATUS :
        return (16);

    case FIELD_ID_CHAR_MANUFACTURER_NAME :
    case FIELD_ID_CHAR_HARDWARE_REVISION :
    case FIELD_ID_CHAR_FIRMWARE_REVISION :
        return SPI_PACKET_DATA_SIZE;

    default :
        return 0;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**@brief This function returns index of entry in client_device_names array based on given address.
 *
 * @param device_name Pointer to entry in client_device_names array.
 *
 * @return    Index of entry.
 * @return    0xFF if there is no such entry.
 *
 */

uint8_t sensor_get_name_index(const uint8_t * device_name)
{
    data_id_t cnt;
    for(cnt = 0; cnt <= MAX_CLIENTS; cnt++)
    {
        if(device_name == client_device_names[cnt])
        {
            return (uint8_t)cnt;
        }
    }
    return 0xFF;
}

