/*
 * Mram.cpp
 *
 * Скажу честно пишу драйвер для какого либо устройства в первый раз поэтому очень сильно хотелось бы получить от вас обратную связь
 */

#include "MR25H40.h"
//конструктор и деструктор
MR25H40::~MR25H40(){}
MR25H40::MR25H40(SPI_HandleTypeDef* mram_spi,GPIO_TypeDef* ss_port,uint16_t ss_pin):mram_spi_(mram_spi),ss_port_(ss_port),ss_pin_(ss_pin){}
//приватные методы
void MR25H40::Enable(){
	HAL_GPIO_WritePin(ss_port_,ss_pin_,GPIO_PIN_RESET);
}
void MR25H40::Disable(){
	HAL_GPIO_WritePin(ss_port_,ss_pin_,GPIO_PIN_SET);
}
void MR25H40::Transmit(uint8_t* data,uint16_t size){
	HAL_SPI_Transmit(mram_spi_,data,size,HAL_MAX_DELAY);
}
void MR25H40::Receive(uint8_t* data,uint16_t size){
	HAL_SPI_Receive(mram_spi_,data,size,HAL_MAX_DELAY);
}
//включение записи
void MR25H40::WriteEnable(){
	Enable();
	uint8_t buffer[1] = {WREN};
	Transmit(buffer,1);
	Disable();
}
//выключение записи
void MR25H40::WriteDisable(){
	Enable();
	uint8_t buffer[1] = {WRDI};
	Transmit(buffer,1);
	Disable();
}
//запись статус регистра
void MR25H40::WriteStatusRegister(uint8_t* status_register){
	Enable();
	uint8_t buffer[2]={WRSR,*status_register};
	Transmit(buffer,2);
	Disable();

}
//запись данных в mram
void MR25H40::WriteData(uint32_t address,uint8_t* data, uint16_t size){
	uint8_t* status_register;
	ReadStatusRegister(status_register);
	if(!((*status_register)&0x02)){
		WriteEnable();
	}
	delete status_register;
	Enable();
	uint8_t buffer[4]={WRITE,((address>>16)&0xFF),((address>>8)&0xFF),(address&0xFF)};
	Transmit(buffer,4);
	Transmit(data,size);
	Disable();
}
//чтение статус регистра
void MR25H40::ReadStatusRegister(uint8_t* status_register){
	Enable();
	uint8_t buffer[1] = {RDSR};
	Transmit(buffer,1);
	Receive(status_register,1);
	Disable();
}
//чтение данных из mram
void MR25H40::ReadData(uint32_t address,uint8_t* data, uint16_t size ){
	Enable();
	uint8_t buffer[4]={READ,((address>>16)&0xFF),((address>>8)&0xFF),(address&0xFF)};
	Transmit(buffer,4);
	Receive(data,size);
	Disable();
}
//переход в спящий режим
void MR25H40::Sleep(){
	Enable();
	uint8_t buffer[1] = {SLEEP};
	Transmit(buffer,1);
	Disable();
}
//выход из режима сна
void MR25H40::Wake(){
	Enable();
	uint8_t buffer[1] = {WAKE};
	Transmit(buffer,1);
	Disable();
}
