/*                                MRF24J40.c                               */
/* Controls the MRF24J40 transceiver                                       */
/* 2008/03/12 - V0.2  - GP                                                 */

#include "MRF24J40.h"

unsigned char myLongAddres[8] = {EUI_0,EUI_1,EUI_2,EUI_3,EUI_4,EUI_5,EUI_6,EUI_7};

static BUFFER        G_mrf24j40rxBuf;
static BOOL          G_mrf24j40_newMsg;

/*------------------------------= FUNCTIONS =------------------------------*/

static void PHYSetLongRAMAddr(WORD address, BYTE value)
{
    PHY_CS = 0;
    SPIPut((((BYTE)(address>>3))&0b01111111)|0x80);
    SPIPut((((BYTE)(address<<5))&0b11100000)|0x10);
    SPIPut(value);
    PHY_CS = 1;
}

static void PHYSetShortRAMAddr(BYTE address, BYTE value)
{
    PHY_CS = 0;       
    SPIPut(address);
    SPIPut(value);
    PHY_CS = 1;
}

static BYTE PHYGetShortRAMAddr(BYTE address)
{
    BYTE toReturn;
    PHY_CS = 0;
    SPIPut(address);
    toReturn = SPIGet();
    PHY_CS = 1;
    return toReturn;
}

static BYTE PHYGetLongRAMAddr(WORD address)
{
    BYTE toReturn;
    PHY_CS = 0;
    SPIPut(((address>>3)&0b01111111)|0x80);
    SPIPut(((address<<5)&0b11100000));
    toReturn = SPIGet();
    PHY_CS = 1;
    return toReturn;
}

/*-------------------------------------------------------------------------*/
/**
  *  @brief Sends a message
  *
  *  @param txPck - pointer to buffer
  *  @param len   - lenght of the buffers
*/
void MRF24J40_send(BYTE* txPck, BYTE len)
{
  BYTE it;
  
  PHYSetLongRAMAddr(0x000,len+1);
  PHYSetLongRAMAddr(0x001,len+1);
  PHYSetLongRAMAddr(0x002, 0xAA);
  for(it=1; it<len+1; it++)
    PHYSetLongRAMAddr(it+2, *txPck++);
    
  PHYSetShortRAMAddr(WRITE_TXNMTRIG,0b00000001);  //transmit packet without ACK requested   
  return;
}


/*-------------------------------------------------------------------------*/
/**
  *  @brief Checks if a new message have arrived
  *
  *  @return TRUE if new message ; FALSE if not
*/
BOOL MRF24J40_newMsg( void )
{
  return G_mrf24j40_newMsg;
}

/*-------------------------------------------------------------------------*/
/**
  *  @brief Gets the received message
  *
  *  @return the reveiving buffer
*/
BUFFER MRF24J40_get( void )
{
  if(G_mrf24j40_newMsg)
  {
    G_mrf24j40_newMsg = FALSE;
    return G_mrf24j40rxBuf;
  }  
}

/*-------------------------------------------------------------------------*/
/**
  *  @brief Sets the current operating channel of the MRF24J40
  *  
  *  @param channel - the channel to be set (CHANNEL_11 -> CHANNEL_26)
*/
void MRF24J40_setChannel(BYTE channel)
{
    PHYSetLongRAMAddr(RFCTRL0,channel);
    PHYSetShortRAMAddr(WRITE_RFCTL,0x04);
    PHYSetShortRAMAddr(WRITE_RFCTL,0x00);
}

void __attribute__((interrupt, auto_psv)) _INT1Interrupt(void)
{  
  BYTE size, i;
  
  if(RFIE && RFIF)
  {    
    RFIF = 0;
    MRF24J40_IFREG flags;        
    flags.Val=PHYGetShortRAMAddr(READ_ISRSTS);
    if(flags.bits.RF_TXIF)
      PHYSetShortRAMAddr(WRITE_RXFLUSH,0b00000001);
    else if(flags.bits.RF_RXIF)
    {
      size = PHYGetLongRAMAddr(0x300)-4;
      if(PHYGetLongRAMAddr(0x301) == 0xAA)
      {
        for(i=0;i<=size;i++)
        {
            G_mrf24j40rxBuf.byte[i] = PHYGetLongRAMAddr(0x302+i);
        }
        PHYSetShortRAMAddr(WRITE_RXFLUSH,0b00000001);
        G_mrf24j40_newMsg = TRUE;
      }  
    }
    RFIE = 1;
  }
}

void SPIPut(unsigned char v)
{
    unsigned char dummy;
    
    IFS0bits.SPI1IF = 0;
    dummy = SPI1BUF;
    SPI1BUF = v;
    while(IFS0bits.SPI1IF == 0){}
}

unsigned char SPIGet(void)
{
    SPIPut(0x00);
    return SPI1BUF;
}


/*-------------------------------------------------------------------------*/
/**
  *  @brief MRF24J40 is initialized
*/
void MRF24J40_initialize(void)
{
    BYTE i;
    WORD j;
    
    SPI1CON1 = 0b0000000100111110;
    SPI1STAT = 0x8000;
    PHY_RESETn = 0;
    PHY_RESETn_TRIS = 0;
    PHY_CS = 1;
    PHY_CS_TRIS = 0;
    LED_1_TRIS = 0;
    LED_2_TRIS = 0;
    PUSH_BUTTON_1_TRIS = 1;
    PUSH_BUTTON_2_TRIS = 1;
    RFIF = 0;
    RFIE = 1;
    G_mrf24j40_newMsg = FALSE;
    PHY_RESETn = 0;
    for(j=0;j<(WORD)300;j++){}

    PHY_RESETn = 1;
    for(j=0;j<(WORD)300;j++){}

    PHYSetShortRAMAddr(WRITE_SOFTRST,0x07);
    do
    {
        i = PHYGetShortRAMAddr(READ_SOFTRST);
    }
    while((i&0x07) != (BYTE)0x00);   

    for(j=0;j<(WORD)1000;j++){}

    PHYSetShortRAMAddr(WRITE_RXFLUSH,0x01);
    PHYSetShortRAMAddr(WRITE_RXFLUSH,0x01);
    PHYSetShortRAMAddr(WRITE_SADRL,0xFF);
    PHYSetShortRAMAddr(WRITE_SADRH,0xFF);
    PHYSetShortRAMAddr(WRITE_PANIDL,0xFF);
    PHYSetShortRAMAddr(WRITE_PANIDH,0xFF);
    for(i=0;i<(BYTE)8;i++)
    {
        PHYSetShortRAMAddr(WRITE_EADR0+i*2,myLongAddres[i]);
    }
    for(j=0;j<(WORD)1000;j++){}
             
    PHYSetShortRAMAddr(WRITE_RFCTL,0x04);
    PHYSetShortRAMAddr(WRITE_RFCTL,0x00);
    PHYSetLongRAMAddr(RFCTRL1,0x01);
    PHYSetLongRAMAddr(RFCTRL2,0x80);
    PHYSetLongRAMAddr(RFCTRL3,0x00);
    PHYSetLongRAMAddr(RFCTRL6,0x90);
    PHYSetLongRAMAddr(RFCTRL7,0b00000000);    
    PHYSetLongRAMAddr(RFCTRL8,0b00010000);
    PHYSetShortRAMAddr(WRITE_BBREG2,0xB8);
    PHYSetShortRAMAddr(WRITE_BBREG6,0x40);
    PHYSetShortRAMAddr(WRITE_RSSITHCCA,0x00);
    do
    {
        i = PHYGetLongRAMAddr(RFSTATE);
    }
    while((i&0xA0) != 0xA0);
    
    MRF24J40_setChannel(CHANNEL_18);
    PHYSetShortRAMAddr(WRITE_INTMSK,0xF6);
    PHYSetShortRAMAddr(WRITE_RXMCR,0x23);   
    PHYSetShortRAMAddr(WRITE_RXFLUSH,0x01); 
}


