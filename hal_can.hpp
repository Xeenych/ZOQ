#pragma once
#include "zoq.hpp"

namespace ZOQ::Stm32_HAL {

	struct CAN_TxMsg {
  		CAN_TxHeaderTypeDef header;
  		uint8_t data[8];
	};

	struct CAN_RxMsg {
  		CAN_RxHeaderTypeDef header;
  		uint8_t data[8];
	};

	using rx_callback_t = void (*)(CAN_RxMsg const& message);
	using tx_callback_t = void (*)(CAN_TxMsg const& message);

	class hal_can {

	public:
		hal_can(CAN_HandleTypeDef* hcan);
		void ActivateDmaRx(rx_callback_t onMessageReceived);
		void PauseNotification();
		void ResumeNotification();
		uint32_t messagesAvailable();
		void configureFilters(CAN_FilterTypeDef* filter_config);
		bool readMessage(CAN_RxMsg& msgout);
		bool sendMessage(CAN_TxMsg& txmsg);
		rx_callback_t OnMessageReceived = nullptr;
		HAL_StatusTypeDef Stop();
		HAL_StatusTypeDef Start();
		tx_callback_t OnSend = nullptr;

	private:
		CAN_HandleTypeDef* const hcan;
		static hal_can* hal_can1;
		static hal_can* hal_can2;
		static hal_can* select_instance(CAN_HandleTypeDef const* hcan);

		friend void ::HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
		friend void ::HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan);
	};

	inline void hal_can::PauseNotification() {
		static const uint32_t ints = CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_RX_FIFO1_MSG_PENDING;
		auto r1 = HAL_CAN_DeactivateNotification(hcan, ints);
		assert(r1 == HAL_OK);
	}

	inline void hal_can::ResumeNotification() {
		static const uint32_t ints = CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_RX_FIFO1_MSG_PENDING;
		auto r1 = HAL_CAN_ActivateNotification(hcan, ints);
		assert(r1 == HAL_OK);
	}

	inline void hal_can::ActivateDmaRx(rx_callback_t onMessageReceived) {
		OnMessageReceived = onMessageReceived;
		static const uint32_t ints = CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_RX_FIFO1_MSG_PENDING;
		auto r1 = HAL_CAN_ActivateNotification(hcan, ints);
		assert(r1 == HAL_OK);
	}

	inline HAL_StatusTypeDef hal_can::Stop() {
		return HAL_CAN_Stop(hcan);
	}
	inline HAL_StatusTypeDef hal_can::Start() {
		return HAL_CAN_Start(hcan);
	}
	
	inline hal_can::hal_can(CAN_HandleTypeDef* _hcan) : hcan(_hcan) {
		if (_hcan->Instance == CAN1)
			hal_can1 = this;
#ifdef CAN2
		else if (_hcan->Instance == CAN2)
			hal_can2 = this;
#endif

		HAL_CAN_Start(hcan);
	}

	inline uint32_t hal_can::messagesAvailable() {
		uint32_t fifo0 = HAL_CAN_GetRxFifoFillLevel(hcan, CAN_RX_FIFO0);
  		uint32_t fifo1 = HAL_CAN_GetRxFifoFillLevel(hcan, CAN_RX_FIFO1);
  		return fifo0 + fifo1;
	}

	inline bool hal_can::readMessage(CAN_RxMsg& msgout) {
  		HAL_StatusTypeDef result0 = HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &msgout.header, msgout.data);
  		if (result0 == HAL_OK)
		  	return true;

  		HAL_StatusTypeDef result1 = HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &msgout.header, msgout.data);
  		if (result1 == HAL_OK)
			return true;

		return false;
	}

	inline bool hal_can::sendMessage(CAN_TxMsg& txmsg) {
  		if (OnSend !=nullptr)
			OnSend(txmsg);
		uint32_t unused_pTxMailbox;
  		HAL_StatusTypeDef result = HAL_CAN_AddTxMessage(hcan, &txmsg.header, txmsg.data, &unused_pTxMailbox);
  		if (result == HAL_OK)
    		return true;

		return false;
	}

	inline void hal_can::configureFilters(CAN_FilterTypeDef* filter_config) {
		 HAL_StatusTypeDef result = HAL_CAN_ConfigFilter(hcan, filter_config);
		 assert(result==HAL_OK);
	}

}	// namespace
