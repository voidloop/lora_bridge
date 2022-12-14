#ifndef _LORA_BRIDGE_RADIO_H_
#define _LORA_BRIDGE_RADIO_H_

// Commands to alter module behaviour
#define RADIO_COMMAND_WRITE_PARAMS_SAVE         0xC0
#define RADIO_COMMAND_READ_PARAMS               0xC1
#define RADIO_COMMAND_WRITE_PARAMS_NOSAVE       0xC2

// Various flags and masks for param bytes
#define RADIO_PARAM_SPED_UART_BAUD_MASK         0xE0
#define RADIO_PARAM_SPED_UART_BAUD_1200         0x00
#define RADIO_PARAM_SPED_UART_BAUD_2400         0x20
#define RADIO_PARAM_SPED_UART_BAUD_4800         0x40
#define RADIO_PARAM_SPED_UART_BAUD_9600         0x60
#define RADIO_PARAM_SPED_UART_BAUD_19200        0x80
#define RADIO_PARAM_SPED_UART_BAUD_38400        0xA0
#define RADIO_PARAM_SPED_UART_BAUD_57600        0xC0
#define RADIO_PARAM_SPED_UART_BAUD_115200       0xE0

#define RADIO_PARAM_SPED_UART_MODE_MASK         0x18
#define RADIO_PARAM_SPED_UART_MODE_8N1          0x00
#define RADIO_PARAM_SPED_UART_MODE_8O1          0x08
#define RADIO_PARAM_SPED_UART_MODE_8E1          0x18

#define RADIO_PARAM_SPED_DATA_RATE_MASK         0x07
#define RADIO_PARAM_SPED_DATA_RATE_2400         0x02
#define RADIO_PARAM_SPED_DATA_RATE_4800         0x03
#define RADIO_PARAM_SPED_DATA_RATE_9600         0x04
#define RADIO_PARAM_SPED_DATA_RATE_19200        0x05
#define RADIO_PARAM_SPED_DATA_RATE_38400        0x06
#define RADIO_PARAM_SPED_DATA_RATE_62500        0x07

#define RADIO_PARAM_OPT1_PACKET_LEN_MASK        0xC0
#define RADIO_PARAM_OPT1_PACKET_LEN_200         0x00
#define RADIO_PARAM_OPT1_PACKET_LEN_128         0x40
#define RADIO_PARAM_OPT1_PACKET_LEN_64          0x80
#define RADIO_PARAM_OPT1_PACKET_LEN_32          0xC0

#define RADIO_PARAM_OPT1_RSSI_NOISE_MASK        0x20
#define RADIO_PARAM_OPT1_RSSI_NOISE_DISABLE     0x00
#define RADIO_PARAM_OPT1_RSSI_NOISE_ENABLE      0x20

#define RADIO_PARAM_OPT1_TX_POWER_MASK          0x03
#define RADIO_PARAM_OPT1_TX_POWER_22            0x00
#define RADIO_PARAM_OPT1_TX_POWER_17            0x01
#define RADIO_PARAM_OPT1_TX_POWER_13            0x02
#define RADIO_PARAM_OPT1_TX_POWER_10            0x03

#define RADIO_PARAM_OPT2_RSSI_BYTE_MASK         0x80
#define RADIO_PARAM_OPT2_RSSI_BYTE_DISABLE      0x00
#define RADIO_PARAM_OPT2_RSSI_BYTE_ENABLE       0x80

#define RADIO_PARAM_OPT2_TX_METHOD_MASK         0x40
#define RADIO_PARAM_OPT2_TX_METHOD_TRANSPARENT  0x00
#define RADIO_PARAM_OPT2_TX_METHOD_FIXED        0x40

#define RADIO_PARAM_OPT2_LTB_MASK               0x10
#define RADIO_PARAM_OPT2_LTB_DISABLE            0x00
#define RADIO_PARAM_OPT2_LTB_ENABLE             0x10

#define RADIO_PARAM_OPT2_WOR_CYCLE_MASK         0x07
#define RADIO_PARAM_OPT2_WOR_CYCLE_500          0x00
#define RADIO_PARAM_OPT2_WOR_CYCLE_1000         0x01
#define RADIO_PARAM_OPT2_WOR_CYCLE_1500         0x02
#define RADIO_PARAM_OPT2_WOR_CYCLE_2000         0x03
#define RADIO_PARAM_OPT2_WOR_CYCLE_2500         0x04
#define RADIO_PARAM_OPT2_WOR_CYCLE_3000         0x05
#define RADIO_PARAM_OPT2_WOR_CYCLE_3500         0x06
#define RADIO_PARAM_OPT2_WOR_CYCLE_4000         0x07

// Defaults
#define RADIO_DEFAULT_ADDRESS_HIGH    0xFF
#define RADIO_DEFAULT_ADDRESS_LOW     0xFF
#define RADIO_DEFAULT_CHANNEL         0x17
#define RADIO_DEFAULT_WOR_CYCLE       RADIO_PARAM_OPT2_WOR_CYCLE_2000
#define RADIO_DEFAULT_TX_POWER        RADIO_PARAM_OPT1_TX_POWER_10
#define RADIO_DEFAULT_DATA_RATE       RADIO_PARAM_SPED_DATA_RATE_2400
#define RADIO_DEFAULT_UART_MODE       RADIO_PARAM_SPED_UART_MODE_8N1
#define RADIO_DEFAULT_UART_BAUD       RADIO_PARAM_SPED_UART_BAUD_9600

#include <hardware/uart.h>

/// Structure for a radio instance
typedef struct {
    uart_inst_t *uart;
    uint tx_pin;
    uint rx_pin;
    uint m0_pin;
    uint m1_pin;
    uint aux_pin;
} radio_inst_t;

/// Defines values to be passed to set_operating_mode
typedef enum {
    MODE_NORMAL = 0,    ///< Normal mode for sending and receiving messages
    MODE_WAKE_UP,       ///< Adds a long preamble to transmission to allow destination receivers to wake up
    MODE_POWER_SAVING,  ///< Receiver sleeps until a message is received
    MODE_SLEEP          ///< Use during parameter setting
} operating_mode_t;

/// Structure for reading and writing radio control parameters
typedef struct {
    uint8_t addh;      ///< High address byte
    uint8_t addl;      ///< Low address byte
    uint8_t sped;      ///< Data and baud rate parameters
    uint8_t opt1;      ///< Various control options
    uint8_t chan;      ///< Radio channel
    uint8_t opt2;      ///< Various control options
} parameters_t;

bool radio_init(radio_inst_t const *radio);

bool read_parameters(radio_inst_t const *radio, parameters_t *params);

bool write_parameters(radio_inst_t const *radio, parameters_t const *params, bool save);

void set_operating_mode(radio_inst_t const *radio, operating_mode_t mode);

void wait_aux_high(radio_inst_t const *radio);

void set_radio_uart_config_mode(radio_inst_t const *radio);

void set_radio_uart(radio_inst_t const *radio, uint8_t sped);

#endif //_LORA_BRIDGE_RADIO_H_
