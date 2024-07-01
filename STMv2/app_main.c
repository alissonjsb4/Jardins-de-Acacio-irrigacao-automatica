/* USER CODE BEGIN Header */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
extern RTC_HandleTypeDef hrtc;

extern UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
uint8_t general_buffer[7] = { 0, 0, 0, 0, 0, 0, 0 };
uint8_t on = 1;
uint8_t time_parameters[6] = { 9, 9, 9, 9, 9, 9 };
/* USER CODE END PV */

//Atualiza o RTC com uma data e horário fornecidos pelo usuário
void SetRtc(unsigned int time_buffer[]) {

	RTC_DateTypeDef sDate;
	sDate.Date = time_buffer[0];
	sDate.Month = time_buffer[1];
	sDate.Year = time_buffer[2];

	HAL_RTC_SetDate(hrtc, sDate, RTC_FORMAT_BCD);

	RTC_TimeTypeDef sTime;
	sTime.Hours = time_buffer[3];
	sTime.Minutes = time_buffer[4];
	sTime.Seconds = time_buffer[5];

	HAL_RTC_SetTime(&hrtc, sTime, RTC_FORMAT_BCD);
}

//Função de marcar uma irrigação futura
void SetIrrigationTime(unsigned int time_buffer[]) {

	for (int i = 0; i < 6; i++) {

		for (int ii = 0; ii < time_buffer[i]; ii++) {
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
			HAL_Delay(500);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
		}

		HAL_Delay(2000);
	}

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);

}

//Função de iniciar a irrigação. Vai configurar um pino como High
void TurnOn() {
//Função de ligar

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);

}

//Função de desligar a irrigação. Vai configurar um pino como Low
void TurnOff() {
//Função de desligar

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);

}

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	while (1) {

	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {

	//Deve ligar a irrigação automaticamente
	if (general_buffer[0] == 'l') {
		TurnOn();
		on = 0;
	}
	//Deve desligar a irrigação automaticamente
	else if (general_buffer[0] == 'd') {
		TurnOff();
		on = 1;
	}
	//Deve marcar, usando o RTC, um horário para a próxima irrigação
	else if (general_buffer[0] == 'm') {
		for (int i = 0; i < 6; i++) {
			time_parameters[i] = general_buffer[i + 1];
		}
		SetIrrigationTime(time_parameters);
	}
	//Deve retornar o status de irrigação para a esp
	else if (general_buffer[0] == 's') {
		HAL_UART_Transmit(&huart1, &on, 1, 100);
	}
	//Deve retornar os parâmetros de irrigação para a esp
	else if (general_buffer[0] == 'h') {
		HAL_UART_Transmit(&huart1, time_parameters, 6, 100);
	} else {
		Error_Handler();
	}

}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {

}
