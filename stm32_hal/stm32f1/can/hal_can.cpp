#include "hal_can.hpp"

#if __has_include("stm32f1xx_hal.h")

namespace ZOQ::Stm32_HAL {

	hal_can* hal_can::hal_can0 = nullptr;
	hal_can* hal_can::hal_can1 = nullptr;
	hal_can* hal_can::hal_can2 = nullptr;

hal_can* hal_can::select_instance(CAN_HandleTypeDef const* hcan) {

#ifdef CAN
		if (hcan->Instance == CAN)
			return hal_can::hal_can0;
#endif

#ifdef CAN1
		if (hcan->Instance == CAN1)
			return hal_can::hal_can1;
#endif
#ifdef CAN2
		if (hcan->Instance == CAN2)
			return hal_can::hal_can2;
#endif
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

	if (instance->OnMessageReceived != nullptr)
		instance->OnMessageReceived(msg);
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	HAL_CAN_RxFifo0MsgPendingCallback(hcan);
}

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan) {
	auto instance = hal_can::select_instance(hcan);
	if (instance == nullptr)
		 return;

	if (instance->OnMessageSent != nullptr)
		instance->OnMessageSent(0);
}
void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan) {
	auto instance = hal_can::select_instance(hcan);
	if (instance == nullptr)
		 return;

	if (instance->OnMessageSent != nullptr)
		instance->OnMessageSent(10);
}

void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan) {
	auto instance = hal_can::select_instance(hcan);
	if (instance == nullptr)
		 return;

	if (instance->OnMessageSent != nullptr)
		instance->OnMessageSent(2);
}

#endif
