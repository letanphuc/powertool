#ifndef _packet_h_
#define _packet_h_

/*
 * predefine header used for standard communication's protocol,
 * this use bold on master and slave program.
 */

#include <stdint.h>

/* packet type */
/* command types */
#define CMD_TYPE_CONTROL	0x01
#define CMD_TYPE_QUERY		0x02
#define CMD_TYPE_ANSWER		0x03

#define CMD_TYPE_MASK		0x03
#define GET_CMD_TYPE(x)		(x & CMD_TYPE_MASK)
#define IS_CMD_TYPE(x)		(GET_CMD_TYPE(x) == CMD_TYPE_CONTROL || \
		GET_CMD_TYPE(x) == CMD_TYPE_QUERY || GET_CMD_TYPE(x) == CMD_TYPE_ANSWER)
/* command types */

/* commands */
#define CMD_ULTRA_SONIC		(0x01 << 2)
#define CMD_TEMPERATURE		(0x02 << 2)
#define CMD_TIME			(0x03 << 2)
#define CMD_CALENDER		(0x04 << 2)
#define CMD_UNIQUE_NUMBER	(0x05 << 2)

#define CMD_SEARCH_LEVEL_1 (0x06 << 2)
#define CMD_SEARCH_LEVEL_2 (0x07 << 2)
#define CMD_SEARCH_LEVEL_3 (0x08 << 2)

#define CMD_MASK			0xfc
#define GET_CMD(x)			(x & CMD_MASK)
#define IS_CMD(x)			(GET_CMD(x) == CMD_ULTRA_SONIC || \
GET_CMD(x) == CMD_TEMPERATURE || \
GET_CMD(x) == CMD_TIME || \
GET_CMD(x) == CMD_UNIQUE_NUMBER || \
GET_CMD(x) == CMD_SEARCH_LEVEL_1 || \
GET_CMD(x) == CMD_SEARCH_LEVEL_2 || \
GET_CMD(x) == CMD_SEARCH_LEVEL_3 || \
GET_CMD(x) == CMD_CALENDER)
/* commands */
/* packet type */

/*
 * device types for id field
 */
#define DEV_ULTRA_SONIC		0x01
#define DEV_TEMPERATURE		0x02

#define DEV_UNKNOWN 		0xff
/*
 *
 */

/* data and data type*/
#define DATA_TYPE_U8			0x01
#define DATA_TYPE_U16			0x02
#define DATA_TYPE_U32			0x03
#define DATA_TYPE_U64			0x04
#define DATA_TYPE_S8			0x05
#define DATA_TYPE_S16			0x06
#define DATA_TYPE_S32			0x07
#define DATA_TYPE_S64			0x08
#define DATA_TYPE_FLOAT			0x09
#define DATA_TYPE_DOUBLE		0x0a

// structured data type
#define DATA_TYPE_ULTRA_SONIC	0x10
#define DATA_TYPE_TEMPERATURE	0x20
#define DATA_TYPE_TIME			0x30
#define DATA_TYPE_CALENDER		0x40

#define DATA_TYPE_NULL			0xfe

#define IS_SUPPORT_DATA_TYPE(x) \
	(x == DATA_TYPE_U8 || \
			x == DATA_TYPE_U16 || \
			x == DATA_TYPE_U32 || \
			x == DATA_TYPE_U64 || \
			x == DATA_TYPE_S8 || \
			x == DATA_TYPE_S16 || \
			x == DATA_TYPE_S32 || \
			x == DATA_TYPE_S64 || \
			x == DATA_TYPE_FLOAT || \
			x == DATA_TYPE_DOUBLE || \
			x == DATA_TYPE_ULTRA_SONIC || \
			x == DATA_TYPE_TEMPERATURE || \
			x == DATA_TYPE_TIME || \
			x == DATA_TYPE_CALENDER || \
			x == DATA_TYPE_NULL)

/* some data structure */
struct UltraSonic
{
	float distance;
};

// TODO: add structured data here

/* some data structure */
/* data and data type*/
#define DATA_SIZE 32

struct PacketData
{
	uint8_t id;					// packet identification
	uint8_t unique_number[4];	// devices identification
	uint8_t data_type;			// type of data, or data's format
	uint8_t data[DATA_SIZE]; 			// data, not include checksum's byte
};

/*
 * Packet structure
 */
struct PhysicalPacket
{
	uint8_t length;				// The length of physical packet, not include checksum
	uint8_t type; 				// include commands and command type
	struct PacketData data;
};


/*
 * public functions
 */
//extern int phy_to_data(struct PhysicalPacket * phy, struct DataPacket * data);
//extern int data_to_phy(struct DataPacket * data, struct PhysicalPacket *phy);
extern uint8_t checksum(struct PhysicalPacket * phy);
extern int get_data_length(int data_type);

#endif
