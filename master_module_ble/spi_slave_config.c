
/** @file   spi_slave_config.c
 *  @brief  This driver contains functions for working with spi module
 *          and corresponding macros, constants,and global variables.
 *
 *  @author MikroElektronika
 *  @bug    No known bugs.
 */

/* -- Includes -- */

#include "spi_slave_config.h"
#include "client_handling.h"
#include "spi_slave.h"
#include "app_error.h"
#include "app_util_platform.h"
#include "nrf_gpio.h"
#include "gpio.h"
#include "client_handling.h"
#include "onboard.h"
#include "debug.h"

#define APPL_LOG                   debug_log

#define DEF_CHARACTER 0xDDu             /**< SPI default character. Character clocked out in case of an ignored transaction. */
#define ORC_CHARACTER 0xCCu             /**< SPI over-read character. Character clocked out after an over-read of the transmit buffer. */

#define SPIS_MISO_PIN    0    // SPI MISO signal.
#define SPIS_CSN_PIN     3    // SPI CSN signal.
#define SPIS_MOSI_PIN    1    // SPI MOSI signal.
#define SPIS_SCK_PIN     5    // SPI SCK signal.
#define SPIS_RDY_TO_SEND 2    // SPI Ready To Send signal.

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**@brief Possible status of frame. */
typedef enum
{
   FRAME_DATA_STATUS_EMPTY = 0,
   FRAME_DATA_STATUS_FULL  = 1,
   FRAME_DATA_STATUS_LOCK  = 2,
}
frame_data_status_t;

/**@brief SPI frame record. */
typedef struct
{
    spi_frame_t           frame;
    frame_data_status_t   data_status;
}
spi_client_frame_buffer_t;

/**@brief SPI transmmiting possible status. */
typedef enum
{
   SPI_TX_STATUS_FREE = 0,
   SPI_TX_STATUS_BUSY = 1,
}
spi_tx_status_t;

spi_client_frame_buffer_t  spi_clients_frame_buffer[MAX_CLIENTS+1];
spi_client_frame_buffer_t *spi_curr_frame;
spi_client_frame_buffer_t *spi_onboard_frame = &spi_clients_frame_buffer[MAX_CLIENTS];

spi_client_frame_buffer_t  spi_response_frame;

spi_tx_status_t spi_tx_status = SPI_TX_STATUS_FREE;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static spi_frame_t  spi_rx_frame;
static spi_frame_t  spi_tx_frame;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//static void spi_slave_event_handle(spi_slave_evt_t event);
static bool spi_handler(data_id_t data_id, uint8_t field_id, uint8_t client_index, uint8_t read_write, uint8_t * data);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**@brief Extern variables. */

extern const uint8_t     client_device_names[MAX_CLIENTS][BLE_DEVNAME_MAX_LEN + 1];
// extern const char_desc_t client_char_uuids[MAX_CLIENTS][NUMBER_OF_RELAYR_CHARACTERISTICS + 4];
extern const uint8_t     CENTRAL_BLE_FIRMWARE_REV[20];

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**@brief Function prepares rx and tx SPI buffer pointers, and sets corresponding buffer size.
 *
 * @param
 */

void spi_create_tx_packet(data_id_t data_id, uint8_t field_id, uint8_t client_index, uint8_t operation, uint8_t * data, uint8_t len)
{
    spi_client_frame_buffer_t* frame_buff;

    if( (data_id >= DATA_ID_RESPONSE_OK) && (data_id <= DATA_ID_RESPONSE_NOT_FOUND) )
    {
        frame_buff = &spi_response_frame;
    }
    else
    {
        frame_buff = &spi_clients_frame_buffer[data_id];
        if(frame_buff->data_status == FRAME_DATA_STATUS_LOCK)
        {
            return;
        }
    }

    // "Clear" tx buffer.
    memset((uint8_t *)&frame_buff->frame, 0xFF, sizeof(spi_tx_frame));


    // if(data_id == DATA_ID_DEV_CENTRAL)
    // {
    //     data_id = DATA_ID_CONFIG;
    // }

    frame_buff->frame.data_id   = data_id;
    frame_buff->frame.field_id  = field_id;
    frame_buff->frame.client_index  = client_index;
    frame_buff->frame.operation = (operation_t)operation;
    if(data != NULL)
    {
        memcpy(frame_buff->frame.data, data, len);
    }

    frame_buff->data_status = FRAME_DATA_STATUS_FULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void spi_lock_tx_packet(data_id_t data_id)
{
    spi_clients_frame_buffer[data_id].data_status = FRAME_DATA_STATUS_LOCK;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void spi_clear_tx_packet(data_id_t data_id)
{
    spi_client_frame_buffer_t * frame_buff = &spi_clients_frame_buffer[data_id];

    memset((uint8_t *)&frame_buff->frame, 0xFF, sizeof(spi_tx_frame));
    frame_buff->data_status = FRAME_DATA_STATUS_EMPTY;
    spi_tx_status = SPI_TX_STATUS_FREE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void set_next_frame(void)
{
    spi_curr_frame++;
    if(spi_curr_frame > &spi_clients_frame_buffer[NUMBER_OF_SENSORS-1])
    {
        spi_curr_frame = &spi_clients_frame_buffer[0];
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void spi_check_tx_ready(void)
{
    // Check if data sends or receives.
    // APPL_LOG("[SPI]: spi_tx_status = 0x%x, gpio_read = 0x%x\r\n", spi_tx_status, gpio_read(SPIS_CSN_PIN));
    if(
       (spi_tx_status == SPI_TX_STATUS_BUSY) ||
       (gpio_read(SPIS_CSN_PIN) == 0)
      )
    {
        return;
    }
    else
    {
        if(spi_onboard_frame->data_status == FRAME_DATA_STATUS_FULL)
        {
            spi_tx_status = SPI_TX_STATUS_BUSY;
            memcpy((uint8_t *)&spi_tx_frame, (uint8_t *)&spi_onboard_frame->frame, sizeof(spi_frame_t));
            gpio_write(SPIS_RDY_TO_SEND, true);
            return;
        }

        // Check if there is some RESPONSE to send.
        else if(
                (spi_response_frame.data_status == FRAME_DATA_STATUS_FULL)
               )
        {
            spi_tx_status = SPI_TX_STATUS_BUSY;
            memcpy((uint8_t *)&spi_tx_frame, (uint8_t *)&spi_response_frame.frame, sizeof(spi_frame_t));
            gpio_write(SPIS_RDY_TO_SEND, true);
            return;
        }

        // Search next client with data ready.
        else
        {
            uint8_t cnt;
            for(cnt = 0; cnt < NUMBER_OF_SENSORS; cnt++)
            {
                set_next_frame();

                if(
                    ((spi_curr_frame->data_status == FRAME_DATA_STATUS_FULL) || (spi_curr_frame->data_status == FRAME_DATA_STATUS_LOCK))&&
                    (gpio_read (SPIS_CSN_PIN) != 0)
                  )
                {
                    spi_tx_status = SPI_TX_STATUS_BUSY;
                    memcpy((uint8_t *)&spi_tx_frame, (uint8_t *)&spi_curr_frame->frame, sizeof(spi_frame_t));
                    gpio_write(SPIS_RDY_TO_SEND, true);
                    break;
                }
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**@brief Function for SPI slave event callback.
 *
 * Upon receiving an SPI transaction complete event, LED1 will blink and the buffers will be set.
 *
 * @param[in] event SPI slave driver event.
 */
void SPI1_TWI1_IRQHandler(void)
{
     if (NRF_SPIS1->EVENTS_END != 0)
     {
          NRF_SPIS1->EVENTS_END = 0;

          if(spi_onboard_frame->data_status == FRAME_DATA_STATUS_FULL)
          {
              spi_onboard_frame->data_status = FRAME_DATA_STATUS_EMPTY;
              onboard_on_send_complete();
          }
          else if(onboard_get_state() == ONBOARD_STATE_IDLE)
          {
              if(spi_response_frame.data_status == FRAME_DATA_STATUS_FULL)
              {
                  spi_response_frame.data_status = FRAME_DATA_STATUS_EMPTY;
              }
              else if(
                       (spi_curr_frame->data_status == FRAME_DATA_STATUS_FULL) ||
                       (spi_curr_frame->data_status == FRAME_DATA_STATUS_LOCK)
                     )
              {
                  spi_curr_frame->data_status = FRAME_DATA_STATUS_EMPTY;
              }
          }

          memset((uint8_t *)&spi_tx_frame, 0xFF, sizeof(spi_tx_frame));
          spi_tx_status = SPI_TX_STATUS_FREE;

          // gpio_write(SPIS_RDY_TO_SEND, false);

          spi_handler(spi_rx_frame.data_id, spi_rx_frame.field_id, spi_rx_frame.client_index, spi_rx_frame.operation, spi_rx_frame.data);

          gpio_write(SPIS_RDY_TO_SEND, false);
     }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**@brief
 *
 * @param[in]
 */

static bool spi_handler(data_id_t data_id, uint8_t field_id, uint8_t client_index, uint8_t read_write, uint8_t * data)
{
    // APPL_LOG("[SPI] handler data_id = %x, field_id = %d, client_index = %d\r\n", data_id, field_id, client_index);
    // APPL_LOG("\r\n[SPI] handler data_id = %x, field_id = %d, client_index = %d, data = %x", data_id, field_id, client_index, *(uint16_t*)data);
    if(
       (data_id <= MAX_CLIENTS) &&
       ((field_id == FILED_ID_READ_CHAR) ||
       (field_id == FILED_ID_WRITE_CHAR))
      )
    {
        client_t* p_client;
        p_client = find_client_by_id(data_id);

        // Check if sensor is connected.
        if(p_client == NULL)
        {
            spi_create_tx_packet(DATA_ID_RESPONSE_NOT_FOUND, FIELD_ID_RUN_ERROR, DATA_ID_DEV_CENTRAL, OPERATION_NONE, NULL, 0);
            return true;
        }
        // Check if sensor is in running state.
        else if((p_client->state != STATE_RUNNING) && (p_client->state != STATE_WAIT))
        {
            spi_create_tx_packet(DATA_ID_RESPONSE_BUSY, FIELD_ID_RUN_ERROR, DATA_ID_DEV_CENTRAL, OPERATION_NONE, NULL, 0);
            return true;
        }


        uint16_t charUuid = *(uint16_t*)data;
        // Sensor is in RUNNING state.
        APPL_LOG("[SPI] handler data_id = %x, field_id = %d, client_index = %d device_name = %s, read_write = %d, charUuid = 0x%x\r\n",
                 data_id,
                 field_id,
                 client_index,
                 p_client->device_name,
                 read_write,
                 charUuid);
        if(read_write == OPERATION_WRITE)
        {
            uint8_t len = *(data + sizeof(uint16_t));
            return write_characteristic_value(p_client, charUuid, data + sizeof(uint16_t), len);
        }
        else
        {
            return read_characteristic_value(p_client, charUuid);
        }
    }

    // Check if config data received.
    else if(data_id == DATA_ID_CONFIG)
    {
        switch(field_id)
        {

            case FIELD_ID_RUN:
            {
                onboard_set_run_security_params();
                onboard_set_mode(ONBOARD_MODE_RUN);
                return true;
            }

            case FIELD_ID_CONFIG_START:
            {
                onboard_set_mode(ONBOARD_MODE_CONFIG);
                // if(onboard_get_state() == ONBOARD_STATE_IDLE)
                // {
                //     onboard_set_state(ONBOARD_STATE_START);
                // }
                // spi_create_tx_packet(DATA_ID_DEV_CENTRAL, FIELD_ID_CONFIG_ACK, 0, OPERATION_WRITE, NULL, 0);
                return true;
            }

            case FIELD_ID_CONFIG_START_DISCOVERY:
            {
                onboard_set_discovery_security_params();
                onboard_set_mode(ONBOARD_MODE_DISCOVERY);
                // spi_create_tx_packet(DATA_ID_DEV_CENTRAL, FIELD_ID_CONFIG_ACK, 0, OPERATION_WRITE, NULL, 0);
                return true;
            }

            case FIELD_ID_CONFIG_STOP:
            {
                onboard_set_state(ONBOARD_STATE_IDLE);
                return true;
            }

            case FIELD_ID_CONFIG_ADD_DISCOVERY_SERVICE:
            {
                // spi_create_tx_packet(DATA_ID_DEV_CENTRAL, FIELD_ID_CONFIG_ACK, 0, OPERATION_WRITE, NULL, 0);
                return onboard_store_discovery_service(data);
            }

            case FIELD_ID_CONFIG_ADD_CLIENT_CHARACTERISTC:
            {
                // return onboard_store_client_char_uuid(client_index, data);
                return false;
            }

            case FIELD_ID_CONFIG_CLIENT_NAME:
            {
                // spi_create_tx_packet(DATA_ID_DEV_CENTRAL, FIELD_ID_CONFIG_ACK, 0, OPERATION_WRITE, NULL, 0);
                return onboard_store_client_device_name(client_index, data);
            }

            case FIELD_ID_CONFIG_CLIENT_PASS:
            {
                return onboard_store_passkey_from_wifi(client_index, data);
            }

            case FIELD_ID_CONFIG_CLIENT_UUID:
            {
                return onboard_store_client_uuid(client_index, data);
            }
            // case FIELD_ID_CONFIG_HTU_PASS:
            // case FIELD_ID_CONFIG_GYRO_PASS:
            // case FIELD_ID_CONFIG_LIGHT_PASS:
            // case FIELD_ID_CONFIG_SOUND_PASS:
            // case FIELD_ID_CONFIG_BRIDGE_PASS:
            // case FIELD_ID_CONFIG_IR_PASS:
            // {
            //     return onboard_store_passkey_from_wifi(field_id, data);
            // }

        }
    }

    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**@brief Function for set SPI config params.
 *
 * @param[in] None.
 */

bool spi_slave_app_init(void)
{
    uint32_t mode_mask;
    uint8_t cnt;

    for(cnt = 0; cnt < MAX_CLIENTS; cnt++)
    {
        spi_clear_tx_packet((data_id_t)cnt);
    }
    spi_curr_frame = &spi_clients_frame_buffer[0];

    memset((uint8_t *)&spi_tx_frame, 0xFF, sizeof(spi_tx_frame));
    spi_tx_frame.data_id   = DATA_ID_DEV_CENTRAL;
    spi_tx_frame.field_id  = FIELD_ID_CHAR_FIRMWARE_REVISION;
    spi_tx_frame.operation = OPERATION_WRITE;
    memcpy((uint8_t *)&spi_tx_frame.data, (uint8_t *)CENTRAL_BLE_FIRMWARE_REV, strlen((const char *)CENTRAL_BLE_FIRMWARE_REV));

    memset((uint8_t *)&spi_rx_frame, 0xFF, sizeof(spi_rx_frame));

    // Configure the SPI pins for input.
    NRF_GPIO->PIN_CNF[SPIS_MISO_PIN] =
        (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos) |
        (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)     |
        (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)   |
        (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)  |
        (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos);

    NRF_GPIO->PIN_CNF[SPIS_CSN_PIN] =
        (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos) |
        (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)     |
        (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)   |
        (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)  |
        (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos);

    NRF_GPIO->PIN_CNF[SPIS_MOSI_PIN] =
        (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos) |
        (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)     |
        (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)   |
        (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)  |
        (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos);

    NRF_GPIO->PIN_CNF[SPIS_SCK_PIN] =
        (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos) |
        (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)     |
        (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)   |
        (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)  |
        (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos);

    NRF_SPIS1->PSELCSN  = SPIS_CSN_PIN;
    NRF_SPIS1->PSELSCK  = SPIS_SCK_PIN;
    NRF_SPIS1->PSELMOSI = SPIS_MOSI_PIN;
    NRF_SPIS1->PSELMISO = SPIS_MISO_PIN;
    NRF_SPIS1->MAXRX    = 0;
    NRF_SPIS1->MAXTX    = 0;

    mode_mask = ((SPIS_CONFIG_CPOL_ActiveHigh << SPIS_CONFIG_CPOL_Pos) | (SPIS_CONFIG_CPHA_Trailing << SPIS_CONFIG_CPHA_Pos));

    NRF_SPIS1->CONFIG = (mode_mask | (SPIS_CONFIG_ORDER_MsbFirst << SPIS_CONFIG_ORDER_Pos));
    NRF_SPIS1->DEF    = DEF_CHARACTER;
    NRF_SPIS1->ORC    = ORC_CHARACTER;

    // Clear possible pending events.
    NRF_SPIS1->EVENTS_END      = 0;
    NRF_SPIS1->EVENTS_ACQUIRED = 0;

    // Disable END_ACQUIRE shortcut.
    NRF_SPIS1->SHORTS = 0;

    // Set correct IRQ priority and clear any possible pending interrupt.
    NVIC_SetPriority(SPI1_TWI1_IRQn, APP_IRQ_PRIORITY_LOW);
    NVIC_ClearPendingIRQ(SPI1_TWI1_IRQn);

    // Enable IRQ.
    NRF_SPIS1->INTENSET = (SPIS_INTENSET_END_Enabled << SPIS_INTENSET_END_Pos);
    NVIC_EnableIRQ(SPI1_TWI1_IRQn);

    // Enable SPI slave device.
    NRF_SPIS1->ENABLE = (SPIS_ENABLE_ENABLE_Enabled << SPIS_ENABLE_ENABLE_Pos);

    // Acquire semaphore.
    NRF_SPIS1->TASKS_ACQUIRE = 1;
    while(NRF_SPIS1->EVENTS_ACQUIRED == 0);
    NRF_SPIS1->EVENTS_ACQUIRED = 0;

    // Set Tx and Rx buffers.
    NRF_SPIS1->TXDPTR = (uint32_t)&spi_tx_frame;
    NRF_SPIS1->RXDPTR = (uint32_t)&spi_rx_frame;
    NRF_SPIS1->MAXTX  = sizeof(spi_tx_frame);
    NRF_SPIS1->MAXRX  = sizeof(spi_rx_frame);

    NRF_SPIS1->TASKS_RELEASE = 1u;

    gpio_write(SPIS_RDY_TO_SEND, false);
    gpio_set_pin_digital_output(SPIS_RDY_TO_SEND, PIN_DRIVE_S0S1);
    // gpio_set_pin_digital_output(SPIS_RDY_TO_SEND, PIN_DRIVE_H0H1);

    spi_tx_status = SPI_TX_STATUS_BUSY;
    gpio_write(SPIS_RDY_TO_SEND, true);

    return true;
}
