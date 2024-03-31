/*
 * Mram.h
 *
 *Author: Kolosov George
 */

#ifndef SRC_MR25H40_H_
#define SRC_MR25H40_H_

#include "stm32f4xx_hal.h"

class MR25H40 {
private:
	enum CommandCodes{
		WREN  = 0x06,//включение записи
		WRDI  = 0x04,//выключение записи
		RDSR  = 0x05,//прочитать status register
		WRSR  = 0x01,//написать status register
		READ  = 0x03,//чтение
		WRITE = 0x02,//запись
		SLEEP = 0xB9,//спящий режим
		WAKE  = 0xAB //выход из спящего режима
	};
private:
	SPI_HandleTypeDef* mram_spi_;//Структура SPI из HAL
	GPIO_TypeDef* ss_port_;
	uint16_t   ss_pin_;
public:
	//Конструкторы и Деструкторы
	MR25H40          ();
	MR25H40          (SPI_HandleTypeDef* mram_spi,GPIO_TypeDef* ss_port,uint16_t ss_pin);
	virtual ~MR25H40 ();
public:
	//методы
	void WriteData(uint32_t address,uint8_t* data, uint16_t size );//метод записи данных на mram
	void WriteEnable         ();//включение записи
	void WriteDisable        ();//выключение записи
	void WriteStatusRegister (uint8_t* status_register);//запись статус регистра
	void ReadData            (uint32_t address,uint8_t* data, uint16_t size );// чтение данных из mram
	void ReadStatusRegister  (uint8_t* status_register);//чтение из статуса регистра
	void Sleep               ();//сон
	void Wake                ();//выход из сна
private:
	//приватные методы для упрощения написания кода
	void Enable      ();
	void Disable     ();
	void Transmit    (uint8_t* data,uint16_t size);
	void Receive     (uint8_t* data,uint16_t size);

};

#endif /* SRC_MR25H40_H_ */
