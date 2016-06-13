#include "packet.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//int phy_to_data(struct PhysicalPacket * phy, struct DataPacket * data)
//{
//	int ret = 0;
//	data->type = phy->type;
//	data->id = phy->id;
//	memcpy(data->unique_number, phy->unique_number, 4);
//	data->data_type = phy->data_type;
//	if (data->data_type != DATA_TYPE_NULL)
//	{
//		data->data_length = get_data_length(data->data_type);
//
//		memcpy(data->data, phy->data, data->data_length);
//	}
//
//	return ret;
//}
//int data_to_phy(struct DataPacket * data, struct PhysicalPacket *phy)
//{
//	int ret = 0;
//
//	phy->type = data->type;
//	phy->id = data->id;
//
//	memcpy(phy->unique_number, data->unique_number, 4);
//
//	phy->data_type = data->data_type;
//
//	memcpy(phy->data, data->data, data->data_length);
//
//	phy->length = data->data_length + 1	// data
//			+ 4 					// unique number
//			+ 1 					// id
//			+ 2; 					// type and length
//
//	return ret;
//}


int get_data_length(int data_type)
{
	int ret = 0;
	if (IS_SUPPORT_DATA_TYPE(data_type))
	{
		switch(data_type)
		{
		case DATA_TYPE_U8:
		case DATA_TYPE_S8:		ret = 1; break;
		case DATA_TYPE_U16:
		case DATA_TYPE_S16:		ret = 2; break;
		case DATA_TYPE_U32:
		case DATA_TYPE_S32:		ret = 4; break;
		case DATA_TYPE_U64:
		case DATA_TYPE_S64:		ret = 8; break;
		case DATA_TYPE_FLOAT: 	ret = 4; break;
		case DATA_TYPE_DOUBLE: 	ret = 8; break;

		case DATA_TYPE_ULTRA_SONIC:
			ret = sizeof(struct UltraSonic);
			break;
		case DATA_TYPE_TEMPERATURE:
			break;
		case DATA_TYPE_TIME:
			break;
		case DATA_TYPE_CALENDER:
			break;
		default: break;
		}
	}

	return ret;
}
uint8_t checksum(struct PhysicalPacket * phy)
{
	uint8_t checksum = 0, i;
	uint8_t * packet = (uint8_t *)phy;
	for (i = 0; i < phy->length; i++)
		checksum += packet[i];
	checksum = ((!checksum) & 0xff) + 1;
	return checksum;
}

