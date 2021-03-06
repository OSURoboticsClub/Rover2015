/*
 * ComputerInterface.cpp
 *
 * Created: 3/5/2015 8:45:32 AM
 *  Author: Nick McComb
 */ 

#include "ComputerInterface.h"

//Workaround to make current ID Global and initializes the PC Interface variables
void initPCInterface(XMEGAID InputCurrentID){
	GlobalCurrentID = InputCurrentID;
	
	if (InputCurrentID == DRIVE) {
		targetPacketLength = DRIVE_PACKET_LENGTH;
	}
	else if (InputCurrentID == ARM) {
		targetPacketLength = ARM_PACKET_LENGTH;
	}
	
	freshData = 0;
	packetIndex = 0;
	invalidPacketCount = 0;
	processPackets = false;
}

//Interrupt when anything is received (handles all of the packet interpreting)
ISR(USARTC0_RXC_vect){
	USART_RXComplete(&USART_PC_Data);
	
	if(processPackets){
		
		recievedData[packetIndex] = USART_RXBuffer_GetByte(&USART_PC_Data);  //Read character off of buffer
		
		if(packetIndex == (targetPacketLength-1) ){
			FlushSerialBuffer(&USART_PC_Data);
			packetIndex = 0;   //Reset the packet index
			
			
			//Process packets
			//RGBSetColor(BLUE); //Read packet color
			
			if(!freshData) {  //Only change the values if the running program has recognized that they should be changed
						
				switch(GlobalCurrentID){
					case DRIVE: //Parse drive packet
						//LoL, ignore checksum
						if(recievedData[DRIVE_HEAD] == 255 && recievedData[DRIVE_FOOTER] == 255){ //basic verification, TODO: Add checksum verification
							//SendStringPC("Valid Drive Packet. \r\n");
							//RGBSetColor(ORANGE);
							freshData = 1;     //There is new data to process
							//Process the data:
							driveData.leftSpeed = recievedData[LEFT_SPEED];
							driveData.rightSpeed = recievedData[RIGHT_SPEED];
							driveData.gimbalPitch = recievedData[GIMBAL_PITCH];
							driveData.gimbalRoll = recievedData[GIMBAL_ROLL];
							driveData.gimbalYaw = recievedData[GIMBAL_YAW];
						}
						else {
							//++invalidPacketCount;
							RGBSetColor(PURPLE); //Something went pretty wrong...
							//flush buffer? Nah, this shouldn't be necessary because it will have just the right amount of
							//stuffz in the array when we get to this point
						}
						break;
					case ARM:  //Parse arm packet
						//Lol, ignore checksum
						if(recievedData[ARM_HEAD] == 255 && recievedData[ARM_FOOTER] == 255){
							RGBSetColor(ORANGE);
							freshData = 1;     //There is new data to process
							//Process the data:
							armData.commandByte = recievedData[ARM_COMMAND];
							armData.xAxisValue = recievedData[X_AXIS_VALUE];
							armData.yAxisValue = recievedData[Y_AXIS_VALUE];
							armData.zAxisValue = recievedData[Z_AXIS_VALUE];
							armData.gripperRotation = recievedData[GRIPPER_ROTATION];
						
							//Check if the robot need to init, aka home
							if(armData.commandByte & 4){
								armData.initRobot = 1;
							}
							else{
								armData.initRobot = 0;
							}
						
							//Checks if the arm needs to power down 
							if(armData.commandByte & 2){
								armData.powerdown = 1;
							}
							else{
								armData.powerdown = 0;
							}
						
							//Checks if a grip is desired
							if(armData.commandByte & 1 ){
								armData.shouldGrip = 1;
							}
							else {
								armData.shouldGrip = 0;
							}
						
						}
						break;
				}  //End switch statement
			}  //End fresh data check
		}
		else {  //We only want to increase the packet index if it hasn't just been set to 0
				//by the reset functionality.
			++packetIndex;
		}
		
	}
	
	
}

void sendDriveResponse(const DRIVE_RESPONSE & input){
	char stagingArray[DRIVE_RESPONSE_PACKET_LENGTH];
	
	stagingArray[DRIVE_RESPONSE_HEAD] = 255;
	stagingArray[IS_PAUSED_BYTE] = input.isPaused;
	stagingArray[LEFT_ABS_POSITION_B1] = input.leftAbsPosition && 0x0000FF;
	stagingArray[LEFT_ABS_POSITION_B2] = (input.leftAbsPosition && 0x00FF00) >> 2;
	stagingArray[LEFT_ABS_POSITION_B3] = (input.leftAbsPosition && 0xFF0000) >> 4;
	stagingArray[RIGHT_ABS_POSITION_B1] = input.rightAbsPosition && 0x0000FF;
	stagingArray[RIGHT_ABS_POSITION_B2] = (input.rightAbsPosition && 0x00FF00) >> 2;
	stagingArray[RIGHT_ABS_POSITION_B3] = (input.rightAbsPosition && 0xFF0000) >> 4;
	stagingArray[DRIVE_RESPONSE_CHECKSUM] = 0x76;    //Set by specification
	stagingArray[DRIVE_RESPONSE_FOOTER] = 255;

	//Actually 	
	for(int i = 0; i <= DRIVE_RESPONSE_PACKET_LENGTH; ++i){
		while(!USART_IsTXDataRegisterEmpty(&USARTC0));
		USART_PutChar(&USARTC0, stagingArray[i]);
	}
	
}

//Sends the string back to the computer signifying motion is complete
void setActionsComplete(void) {
	SendStringPC("c");
}