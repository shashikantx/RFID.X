/* 
 * File:   rfid.h
 * Author: Shashikant
 *
 * Created on 2 July, 2018, 1:34 PM
 */

#ifndef RFID_H
#define	RFID_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* RFID_H */

#define     MAX_LEN               16        // Maximum length of an array. CHECK IF IT IS MAXIMUM LENGTH/SIZE OF FIFO!!!!!!!!!!!

//MF522 MFRC522 error codes.
#define     MI_OK                 0         // Everything A-OK.
#define     MI_NOTAGERR           1         // No tag error
#define     MI_ERR                2         // General error

//MF522 Command word
#define     MFRC522_IDLE          0x00      // NO action; Cancel the current command
#define     MFRC522_MEM           0x01      // Store 25 byte into the internal buffer.
#define     MFRC522_GENID         0x02      // Generates a 10 byte random ID number.
#define     MFRC522_CALCCRC       0x03      // CRC Calculate or selftest.
#define     MFRC522_TRANSMIT      0x04      // Transmit data
#define     MFRC522_NOCMDCH       0x07      // No command change.
#define     MFRC522_RECEIVE       0x08      // Receive Data
#define     MFRC522_TRANSCEIVE    0x0C      // Transmit and receive data,
#define     MFRC522_AUTHENT       0x0E      // Authentication Key
#define     MFRC522_SOFTRESET     0x0F      // Reset

//Mifare_One tag command word
#define     MF1_REQIDL            0x26      // find the antenna area does not enter hibernation
#define     MF1_REQALL            0x52      // find all the tags antenna area
#define     MF1_ANTICOLL          0x93      // anti-collision
#define     MF1_SELECTTAG         0x93      // election tag
#define     MF1_AUTHENT1A         0x60      // authentication key A
#define     MF1_AUTHENT1B         0x61      // authentication key B
#define     MF1_READ              0x30      // Read Block
#define     MF1_WRITE             0xA0      // write block
#define     MF1_DECREMENT         0xC0      // debit
#define     MF1_INCREMENT         0xC1      // recharge
#define     MF1_RESTORE           0xC2      // transfer block data to the buffer
#define     MF1_TRANSFER          0xB0      // save the data in the buffer
#define     MF1_HALT              0x50      // Sleep


//------------------ MFRC522 registers---------------
//Page 0:Command and Status
#define     Reserved00            0x00
#define     CommandReg            0x01
#define     CommIEnReg            0x02
#define     DivIEnReg             0x03
#define     CommIrqReg            0x04
#define     DivIrqReg             0x05
#define     ErrorReg              0x06
#define     Status1Reg            0x07
#define     Status2Reg            0x08
#define     FIFODataReg           0x09
#define     FIFOLevelReg          0x0A
#define     WaterLevelReg         0x0B
#define     ControlReg            0x0C
#define     BitFramingReg         0x0D
#define     CollReg               0x0E
#define     Reserved01            0x0F
//Page 1:Command
#define     Reserved10            0x10
#define     ModeReg               0x11
#define     TxModeReg             0x12
#define     RxModeReg             0x13
#define     TxControlReg          0x14
#define     TxAutoReg             0x15
#define     TxSelReg              0x16
#define     RxSelReg              0x17
#define     RxThresholdReg        0x18
#define     DemodReg              0x19
#define     Reserved11            0x1A
#define     Reserved12            0x1B
#define     MifareReg             0x1C
#define     Reserved13            0x1D
#define     Reserved14            0x1E
#define     SerialSpeedReg        0x1F
//Page 2:CFG
#define     Reserved20            0x20
#define     CRCResultRegM         0x21
#define     CRCResultRegL         0x22
#define     Reserved21            0x23
#define     ModWidthReg           0x24
#define     Reserved22            0x25
#define     RFCfgReg              0x26
#define     GsNReg                0x27
#define     CWGsPReg              0x28
#define     ModGsPReg             0x29
#define     TModeReg              0x2A
#define     TPrescalerReg         0x2B
#define     TReloadRegH           0x2C
#define     TReloadRegL           0x2D
#define     TCounterValueRegH     0x2E
#define     TCounterValueRegL     0x2F
//Page 3:TestRegister
#define     Reserved30            0x30
#define     TestSel1Reg           0x31
#define     TestSel2Reg           0x32
#define     TestPinEnReg          0x33
#define     TestPinValueReg       0x34
#define     TestBusReg            0x35
#define     AutoTestReg           0x36
#define     VersionReg            0x37
#define     AnalogTestReg         0x38
#define     TestDAC1Reg           0x39
#define     TestDAC2Reg           0x3A
#define     TestADCReg            0x3B
#define     Reserved31            0x3C
#define     Reserved32            0x3D
#define     Reserved33            0x3E
#define     Reserved34            0x3F
//----------------------------------------------- 


/**************************************************************************/
/*!
  @brief   Writes value to a register.
  @param   addr  The address a register.
  @param   val   The value to write to a register.
 */
/**************************************************************************/
void writeToRegister(byte addr, byte val) {
  output_low(CS);
  //Address format: 0XXXXXX0
  spi_write((addr<<1)&0x7E);
  spi_write(val);
  output_high(CS);
}

/**************************************************************************/
/*!
  @brief   Reads the value at a register.
  @param   addr  The address a register.
  @returns The byte at the register.
 */
/**************************************************************************/
byte readFromRegister(byte addr) {
  byte val;
  output_low(CS);
  spi_write(((addr<<1)&0x7E) | 0x80);
  val =spi_read(0x00);
  output_high(CS);
  return val;
}


/**************************************************************************/
/*!
  @brief   Adds a bitmask to a register.
  @param   addr   The address a register.
  @param   mask  The mask to update the register with.
 */
/**************************************************************************/
void setBitMask(byte addr, byte mask) {
  byte current;
  current = readFromRegister(addr);
  writeToRegister(addr, current | mask);
}

/**************************************************************************/
/*!
  @brief   Removes a bitmask from the register.
  @param   reg   The address a register.
  @param   mask  The mask to update the register with.
 */
/**************************************************************************/
void clearBitMask(byte addr, byte mask) {
  byte current;
  current = readFromRegister(addr);
  writeToRegister(addr, current & (~mask));
}


/**************************************************************************/
/*!
  @brief   Sends a SOFTRESET command to the MFRC522 chip.
 */
/**************************************************************************/
void reset() {
  writeToRegister(CommandReg, MFRC522_SOFTRESET);
}



/**************************************************************************/
/*!
  @brief   Does the setup for the MFRC522.
 */
/**************************************************************************/
void MFRC_begin() {
  output_high(CS);
  reset();
  //Timer: TPrescaler*TreloadVal/6.78MHz = 24ms
  writeToRegister(TModeReg, 0x8D);   
  delay_ms(20); // Tauto=1; f(Timer) = 6.78MHz/TPreScaler
  writeToRegister(TPrescalerReg, 0x3E);  // TModeReg[3..0] + TPrescalerReg
  delay_ms(20);
  writeToRegister(TReloadRegL, 30);
  delay_ms(20);
  writeToRegister(TReloadRegH, 0);
  delay_ms(20);
  writeToRegister(TxAutoReg, 0x40);      // 100%ASK
  delay_ms(20);
  writeToRegister(ModeReg, 0x3D);        // CRC initial value 0x6363
  delay_ms(20);
 //writeToRegister(TxControlReg, 0x82);     
  delay_ms(20);
 setBitMask(TxControlReg, 0x03);        // Turn antenna on.
}

/**************************************************************************/
/*!
  @brief   Checks the firmware version of the chip.
  @returns The firmware version of the MFRC522 chip.
 */
/**************************************************************************/
byte getFirmwareVersion() {
  byte response;
  response = readFromRegister(VersionReg);
  return response;
}


/**************************************************************************/
/*!

  @brief   Sends a command to a tag.

  @param   cmd     The command to the MFRC522 to send a command to the tag.
  @param   data    The data that is needed to complete the command.
  @param   dlen    The length of the data.
  @param   result  The result returned by the tag.
  @param   rlen    The number of valid bits in the resulting value.

  @returns Returns the status of the calculation.
           MI_ERR        if something went wrong,
           MI_NOTAGERR   if there was no tag to send the command to.
           MI_OK         if everything went OK.

 */
/**************************************************************************/

int commandTag(byte cmd, byte *data, int dlen, byte *result, int *rlen) {
  int status = MI_ERR;
  byte irqEn = 0x00;
  byte waitIRq = 0x00;
  byte lastBits, n;
  int i;

  switch (cmd) {
  case MFRC522_AUTHENT:
    irqEn = 0x12;
    waitIRq = 0x10;
    break;
  case MFRC522_TRANSCEIVE:
    irqEn = 0x77;
    waitIRq = 0x30;
    break;
  default:
    break;
  }

  writeToRegister(CommIEnReg, irqEn|0x80);    // interrupt request
  clearBitMask(CommIrqReg, 0x80);             // Clear all interrupt requests bits.
  setBitMask(FIFOLevelReg, 0x80);             // FlushBuffer=1, FIFO initialization.

  writeToRegister(CommandReg, MFRC522_IDLE);  // No action, cancel the current command.

  // Write to FIFO
  for (i=0; i < dlen; i++) {
    writeToRegister(FIFODataReg, data[i]);
  }

  // Execute the command.
  writeToRegister(CommandReg, cmd);
  if (cmd == MFRC522_TRANSCEIVE) {
    setBitMask(BitFramingReg, 0x80);  // StartSend=1, transmission of data starts
  }

  // Waiting for the command to complete so we can receive data.
  i = 25; // Max wait time is 25ms.
  do {
    delay_ms(1);
    // CommIRqReg[7..0]
    // Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
    n = readFromRegister(CommIrqReg);
    i--;
  } while ((i!=0) && !(n&0x01) && !(n&waitIRq));

  clearBitMask(BitFramingReg, 0x80);  // StartSend=0

  if (i != 0) { // Request did not time out.
    if(!(readFromRegister(ErrorReg) & 0x1D)) {  // BufferOvfl Collerr CRCErr ProtocolErr
      status = MI_OK;
      if (n & irqEn & 0x01) {
        status = MI_NOTAGERR;
      }

      if (cmd == MFRC522_TRANSCEIVE) {
        n = readFromRegister(FIFOLevelReg);
        lastBits = readFromRegister(ControlReg) & 0x07;
        if (lastBits) {
          *rlen = (n-1)*8 + lastBits;
        } else {
          *rlen = n*8;
        }

        if (n == 0) {
          n = 1;
        }

        if (n > MAX_LEN) {
          n = MAX_LEN;
        }

        // Reading the recieved data from FIFO.
        for (i=0; i<n; i++) {
          result[i] = readFromRegister(FIFODataReg);
        }
      }
    } else {
      status = MI_ERR;
    }
  }
  return status;
}


/**************************************************************************/
/*

  @brief   Checks to see if there is a tag in the vicinity.

  @param   mode  The mode we are requsting in.
  @param   type  If we find a tag, this will be the type of that tag.
                 0x4400 = Mifare_UltraLight
                 0x0400 = Mifare_One(S50)
                 0x0200 = Mifare_One(S70)
                 0x0800 = Mifare_Pro(X)
                 0x4403 = Mifare_DESFire

  @returns Returns the status of the request.
           MI_ERR        if something went wrong,
           MI_NOTAGERR   if there was no tag to send the command to.
           MI_OK         if everything went OK.

 */
/**************************************************************************/
int requestTag(byte mode, byte *data) {
  int status, len;
  writeToRegister(BitFramingReg, 0x07);  // TxLastBists = BitFramingReg[2..0]

  data[0] = mode;
  status = commandTag(MFRC522_TRANSCEIVE, data, 1, data, &len);

  if ((status != MI_OK) || (len != 0x10)) {
    status = MI_ERR;
  }

  return status;
}

/**************************************************************************/
/*!

  @brief   Handles collisions that might occur if there are multiple
           tags available.

  @param   serial  The serial nb of the tag.

  @returns Returns the status of the collision detection.
           MI_ERR        if something went wrong,
           MI_NOTAGERR   if there was no tag to send the command to.
           MI_OK         if everything went OK.

 */
/**************************************************************************/
int antiCollision(byte *serial) {
  int status, i, len;
  byte check = 0x00;

  writeToRegister(BitFramingReg, 0x00);  // TxLastBits = BitFramingReg[2..0]

  serial[0] = MF1_ANTICOLL;
  serial[1] = 0x20;
  status = commandTag(MFRC522_TRANSCEIVE, serial, 2, serial, &len);
  len = len / 8; // len is in bits, and we want each byte.
  if (status == MI_OK) {
    // The checksum of the tag is the ^ of all the values.
    for (i = 0; i < len-1; i++) {
      check ^= serial[i];
    }
    // The checksum should be the same as the one provided from the
    // tag (serial[4]).
    if (check != serial[i]) {
      status = MI_ERR;
    }
  }

  return status;
}

/**************************************************************************/
/*!

  @brief   Calculates the CRC value for some data that should be sent to
           a tag.

  @param   data    The data to calculate the value for.
  @param   len     The length of the data.
  @param   result  The result of the CRC calculation.

 */
/**************************************************************************/
void calculateCRC(byte *data, int len, byte *result) {
  int i;
  byte n;

  clearBitMask(DivIrqReg, 0x04);   // CRCIrq = 0
  setBitMask(FIFOLevelReg, 0x80);  // Clear the FIFO pointer

  //Writing data to the FIFO.
  for (i = 0; i < len; i++) {
    writeToRegister(FIFODataReg, data[i]);
  }
  writeToRegister(CommandReg, MFRC522_CALCCRC);

  // Wait for the CRC calculation to complete.
  i = 0xFF;
  do {
    n = readFromRegister(DivIrqReg);
    i--;
  } while ((i != 0) && !(n & 0x04));  //CRCIrq = 1

  // Read the result from the CRC calculation.
  result[0] = readFromRegister(CRCResultRegL);
  result[1] = readFromRegister(CRCResultRegM);
}

/**************************************************************************/
/*!
  @brief   Selects a tag for processing.
  @param   serial  The serial number of the tag that is to be selected.
  @returns The SAK response from the tag.
 */
/**************************************************************************/
byte selectTag(byte *serial) {
  int i, status, len;
  byte sak;
  byte buffer[9];

  buffer[0] = MF1_SELECTTAG;
  buffer[1] = 0x70;
  for (i = 0; i < 5; i++) {
    buffer[i+2] = serial[i];
  }
  calculateCRC(buffer, 7, &buffer[7]);

  status = commandTag(MFRC522_TRANSCEIVE, buffer, 9, buffer, &len);

  if ((status == MI_OK) && (len == 0x18)) {
    sak = buffer[0];
  }
  else {
    sak = 0;
  }

  return sak;
}

/**************************************************************************/
/*!
  @brief   Handles the authentication between the tag and the reader.
  @param   mode    What authentication key to use.
  @param   block   The block that we want to read.
  @param   key     The authentication key.
  @param   serial  The serial of the tag.
  @returns Returns the status of the collision detection.
           MI_ERR        if something went wrong,
           MI_OK         if everything went OK.
 */
/**************************************************************************/
int authenticate(byte mode, byte block, byte *key, byte *serial) {
  int i, status, len;
  byte buffer[12];

  //Verify the command block address + sector + password + tag serial number
  buffer[0] = mode;          // 0th byte is the mode
  buffer[1] = block;         // 1st byte is the block to address.
  for (i = 0; i < 6; i++) {  // 2nd to 7th byte is the authentication key.
    buffer[i+2] = key[i];
  }
  for (i = 0; i < 4; i++) {  // 8th to 11th byte is the serial of the tag.
    buffer[i+8] = serial[i];
  }

  status = commandTag(MFRC522_AUTHENT, buffer, 12, buffer, &len);

  if ((status != MI_OK) || (!(readFromRegister(Status2Reg) & 0x08))) {
    status = MI_ERR;
  }

  return status;
}


/**************************************************************************/
/*!
  @brief   Tries to read from the current (authenticated) tag.
  @param   block   The block that we want to read.
  @param   result  The resulting value returned from the tag.
  @returns Returns the status of the collision detection.
           MI_ERR        if something went wrong,
           MI_OK         if everything went OK.
 */
/**************************************************************************/
int readFromTag(byte block, byte *result) {
  int status, len;

  result[0] = MF1_READ;
  result[1] = block;
  calculateCRC(result, 2, &result[2]);
  status = commandTag(MFRC522_TRANSCEIVE, result, 4, result, &len);

  if ((status != MI_OK) || (len != 0x90)) {
    status = MI_ERR;
  }

  return status;
}

/**************************************************************************/
/*!
  @brief   Tries to write to a block on the current tag.
  @param   block  The block that we want to write to.
  @param   data   The data that we shoudl write to the block.
  @returns Returns the status of the collision detection.
           MI_ERR        if something went wrong,
           MI_OK         if everything went OK.
 */
/**************************************************************************/
int writeToTag(byte block, byte *data) {
  int status, i, len;
  byte buffer[18];

  buffer[0] = MF1_WRITE;
  buffer[1] = block;
  calculateCRC(buffer, 2, &buffer[2]);
  status = commandTag(MFRC522_TRANSCEIVE, buffer, 4, buffer, &len);

  if ((status != MI_OK) || (len != 4) || ((buffer[0] & 0x0F) != 0x0A)) {
    status = MI_ERR;
  }

  if (status == MI_OK) {
    for (i = 0; i < 16; i++) {
      buffer[i] = data[i];
    }
    calculateCRC(buffer, 16, &buffer[16]);
    status = commandTag(MFRC522_TRANSCEIVE, buffer, 18, buffer, &len);

    if ((status != MI_OK) || (len != 4) || ((buffer[0] & 0x0F) != 0x0A)) {
      status = MI_ERR;
    }
  }

  return status;
}


/**************************************************************************/
/*!
  @brief   Sends a halt command to the current tag.
  @returns Returns the result of the halt.
           MI_ERR        If the command didn't complete properly.
           MI_OK         If the command completed.
 */
/**************************************************************************/
int haltTag() {
  int status, len;
  byte buffer[4];

  buffer[0] = MF1_HALT;
  buffer[1] = 0;
  calculateCRC(buffer, 2, &buffer[2]);
  status = commandTag(MFRC522_TRANSCEIVE, buffer, 4, buffer, &len);
  clearBitMask(Status2Reg, 0x08);  // turn off encryption
  return status;
}
