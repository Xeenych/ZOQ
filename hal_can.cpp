#include "hal_can.hpp"

namespace ZOQ::Stm32_HAL {
	hal_can* hal_can::hal_can1 = nullptr;
	hal_can* hal_can::hal_can2 = nullptr;


	hal_can* hal_can::select_instance(CAN_HandleTypeDef const* hcan) {
		if (hcan->Instance == CAN1)
			return hal_can::hal_can1;
		else if (hcan->Instance == CAN2)
			return hal_can::hal_can2;
		return nullptr;
	}
}	// namespace

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	CAN_RxMsg msg;
	auto instance = hal_can::select_instance(hcan);
	if (instance == nullptr)
		 return;

	auto msg_result = instance->readMessage(msg);
	if (msg_result == false)
		return;

	instance->OnMessageReceived(msg);
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	HAL_CAN_RxFifo0MsgPendingCallback(hcan);
}