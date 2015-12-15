extern unsigned char data_receive[20];
extern unsigned char data_send[20];
extern unsigned char LevelLight[10];
extern unsigned char radio_str;
extern unsigned char IDL;
extern unsigned char IDH;
extern unsigned char meshretrans;
extern char start_program;
extern char power_radio_flag;
extern char dimmer[6];
extern char run;
extern char fb;
extern char neutral;
extern unsigned char waitdimmer;


void test_radio() {
    PWR = 1;
    delay_ms(10);

    CE = 0;
    CSN = 0;

    SPI_TX(0x20);
    SPI_TX(0x7C);

    CSN = 1;

    delay_ms(1);

    CSN = 0;

    SPI_TX(0x00);

    int status = SPI_TX(0xFF);

    CSN = 1;
    // Na teoria, status deve ser 7C
}

void Config_Radio(unsigned char hold) {
    unsigned int power_time = 0;

    CE = 1;
    CSN = 1;

    PWR=1;

    delay_ms(10);





    CE=0;





    // Desabilitando ACK
    CSN=0;

    // Falando com radio, SPI WRITE
    Spi_TX(0x21);
    Spi_TX(0x00);


    CSN=1;
    CSN=0;

    Spi_TX(0x23);
    Spi_TX(0x03);// 5 bytes

    CSN=1;
    CSN=0;

    // Retransmissao desabilitada
    Spi_TX(0x24);
    Spi_TX(0x00);

    CSN=1;
    CSN=0;

    // Canal transmissao
    Spi_TX(0x25);
    Spi_TX(107);

    CSN=1;
    CSN=0;

    // Para ler, diminui 0x20
    int read = Spi_TX(0x05);
    // READ TEM QUE SER 0x6b

    CSN = 1;
    CSN = 0;

    // Config Transmissao
    Spi_TX(0x26);
    Spi_TX(0x26);
    CSN=1;
}

unsigned char Radio_Check() {
    Config_Radio(0);
    Configure_RX_Ack();

    if (Check_Radio(0x3F) == 1) {
        return 1;
    } else {
        Config_Radio(1);
        Configure_RX_Ack();
        return 0;
    }
}

void Configure_RX_Ack() {

      CE=0;
      CSN = 0;

      SPI_Wt(0x20);         // configuracao
      SPI_Wt(0x3D);         // desligado

      CSN = 1;
      CSN = 0;

      SPI_Wt(0x22);
      SPI_Wt(0x3f);

      CSN=1;

      // config add RX
      CSN=0;

      SPI_Wt(0x2A);
      SPI_Wt(1);
      SPI_Wt(1);
      SPI_Wt(1);
      SPI_Wt(1);
      SPI_Wt(1);

      CSN=1;

      // 6bytes de dados
      CSN=0;
      SPI_Wt(0x31);
      SPI_Wt(6);

      CSN=1;
      CSN=0;

      SPI_Wt(0x2B);
      SPI_Wt(10);
      SPI_Wt(1);
      SPI_Wt(1);
      SPI_Wt(1);
      SPI_Wt(1);

      CSN=1;
      CSN=0;

      // 11bytes de dados
      SPI_Wt(0x32);
      SPI_Wt(11);

      CSN=1;
      CSN=0;

      SPI_Wt(0x2C);
      SPI_Wt(20);

      CSN=1;
      CSN=0;

      // 15bytes de dados
      SPI_Wt(0x33);
      SPI_Wt(15);

      CSN=1;
      CSN=0;

      SPI_Wt(0x2D);
      SPI_Wt(30);

      CSN=1;
      CSN=0;

      // 20bytes de dados
      SPI_Wt(0x34);
      SPI_Wt(20);

      CSN=1;
      CSN=0;

      SPI_Wt(0x2E);
      SPI_Wt(40);

      CSN=1;
      CSN=0;

      // 25bytes de dados
      SPI_Wt(0x35);
      SPI_Wt(25);

      CSN=1;
      CSN=0;

      SPI_Wt(0x2F);
      SPI_Wt(50);

      CSN=1;
      CSN=0;

      // 30bytes de dados
      SPI_Wt(0x36);
      SPI_Wt(30);

      CSN=1;
      CSN=0;

      //Tx flush
      SPI_Wt(0xE1);

      CSN=1;
      CSN=0;

      //RX FLUSH
      SPI_Wt(0xE2);

      CSN=1;
      CSN = 0;

      //transformando em RX

      SPI_Wt(0x20);
      SPI_Wt(0x3F);// POWER UP do RX

      CSN = 1;

      delay_us(100, 1);

      CE = 1;

      delay_us(250, 1);
}

void Transmit_Radio(unsigned char bp, unsigned char addr) {
    unsigned int timmer=0;
    unsigned char status=0;
    unsigned char counter=0;
    Config_Radio_SD();
    SSN = 1;
    CE = 0;
    CSN = 0;
    SPI_Wt(0x20);//configuracao
    SPI_Wt(0x3D);// desligado
    CSN = 1;

    CSN=0;
    Spi_TX3(0x27);
    Spi_TX3(0x70);
    CSN=1;
    //Tx flush
    CSN=0;
    SPI_Wt(0xE1);
    CSN=1;

    //RX FLUSH
    CSN=0;
    SPI_Wt(0xE2);
    CSN=1;
    CSN=0;
    Spi_TX3(0x20);
    Spi_TX3(0x5C);
    CSN=1;

    CSN=0;
    Spi_TX3(0x30);
    Spi_TX3(addr);
    Spi_TX3(EEP_READ(31));
    Spi_TX3(EEP_READ(32));
    Spi_TX3(EEP_READ(33));
    Spi_TX3(EEP_READ(34));
    CSN=1;

    CSN=0;
    Spi_TX3(0xA0);

    for(counter=0;counter<bp;counter++) {
        Spi_TX3(data_send[counter]);
    }

    CSN=1;
    CSN=0;
    Spi_TX3(0x20);
    Spi_TX3(0x5E);
    CSN=1;
    INTE=0;
    INTF=0;
    timmer=0;
    delay_us(300+IDL+IDH,1);
    CE=1;
    counter=0;
    while(INTF==0) {
        counter++;
        if(counter>=250)
        {
            break;
        }
        delay_us(10,1);
    }
    CE=0;

    INTF=0;

    CSN=0;
    Spi_TX3(0x07);
    status=Spi_TX3(255);
    CSN=1;

    CSN=0;
    Spi_TX3(0x27);
    Spi_TX3(0x70);
    CSN=1;

    //TX Flush
    CSN=0;
    Spi_TX3(0xE1);
    CSN=1;

    //RX flush
    CSN=0;
    Spi_TX3(0xE2);
    CSN=1;
    CSN = 0;
    SPI_Wt(0x20);//configuracao
    SPI_Wt(0x3D);// desligado
    CSN = 1;
}

void ACK_transmit() {
    unsigned int timmer=0;
    unsigned char status=0;

      CE=0;
      CSN=0;
      Spi_TX3(0x27);
      Spi_TX3(0x70);
      CSN=1;

      //Tx flush
      CSN=0;
      SPI_Wt(0xE1);
      CSN=1;

      //RX FLUSH
      CSN=0;
      SPI_Wt(0xE2);
      CSN=1;

      CSN=0;
      Spi_TX3(0x20);
      Spi_TX3(0x5C);
      CSN=1;

      CSN=0;
      Spi_TX3(0x2A);
      Spi_TX3(1);
      Spi_TX3(0);
      Spi_TX3(0);
      Spi_TX3(0);
      Spi_TX3(0);
      CSN=1;

      CSN=0;
      Spi_TX3(0x30);
      Spi_TX3(1);
      Spi_TX3(0);
      Spi_TX3(0);
      Spi_TX3(0);
      Spi_TX3(0);
      CSN=1;

      CSN=0;
      Spi_TX3(0xA0);
      Spi_TX3(data_receive[0]);
      Spi_TX3(data_receive[1]);
      Spi_TX3(data_receive[2]);
      Spi_TX3(data_receive[3]);      
      CSN=1;

      CSN=0;
      Spi_TX3(0x20);
      Spi_TX3(0x5E);
      CSN=1;

      INTE=0;
      INTF=0;
      timmer=0;

      CE=1;
      while(INTF==0)
      {
        timmer++;
        if(timmer>=30)
        {
            break;
        }
        delti_us(50);
      }
      CE=0;
      INTF=0;
       
      CE=1;
      while(INTF==0)
      {
        timmer++;
        if(timmer>=30)
        {
            break;
        }
        delti_us(50);
      }
      CE=0;
      INTF=0;
      CE=1;
      while(INTF==0)
      {
        timmer++;
        if(timmer>=30)
        {
            break;
        }
        delti_us(50);
      }
      CE=0;
      INTF=0;
 
      CSN=0;
      Spi_TX3(0x07);
      status=Spi_TX3(255);
      CSN=1;
 
      CSN=0;
      Spi_TX3(0x27);
      Spi_TX3(0x70);
      CSN=1;
 
      //TX Flush
      CSN=0;
      Spi_TX3(0xE1);
      CSN=1;
 
      //RX flush
      CSN=0;
      Spi_TX3(0xE2);
      CSN=1;
}
 
unsigned char Receive_Data_Ack() {
    unsigned char status=0;
    unsigned char canal=0;
    unsigned char counter=0;
      CE=0;
 
      CSN=0;
      Spi_TX3(0x20);
      Spi_TX3(0x5C);
      CSN=1;
 
      // leitura do modo status
     CSN=0;
     Spi_TX3(0x07);
     status=Spi_TX3(255);
     CSN=1;
      
     status = status << 4;
     status = status >> 5;
 
      // resetando o modo status
     CSN=0;
     Spi_TX3(0x27);
     Spi_TX3(0x70);
     CSN=1;
      // resgatando dados do buffer 3bytes
     CSN=0;
     Spi_TX3(0x61);
     for(counter=0;counter<15;counter++)
     {
         data_receive[counter]=Spi_TX3(255);
         if(data_receive[counter]==255)
             break;
     }
      
//     data_receive[0]=Spi_TX3(255);//SSPBUF;// conflito de programacao(veificar)
//     data_receive[1]=Spi_TX3(255);//SSPBUF;
//     data_receive[2]=Spi_TX3(255);//SSPBUF;
//     data_receive[3]=Spi_TX3(255);//SSPBUF;
     CSN=1;
      
    
     // resetando o modo status
     CSN=0;
     Spi_TX3(0x27);
     Spi_TX3(0x70);
     CSN=1;
     // FLUSH Rx
     CSN=0;
     Spi_TX3(0xE2);
     CSN=1;
    //TX Flush
     CSN=0;
     Spi_TX3(0xE1);
     CSN=1;
//    delay_us(700, 1);
//    data_send[0]=data_receive[0];
//    data_send[1]=data_receive[1];
//    data_send[2]=data_receive[2];
//    data_send[3]=data_receive[3];
//    if(data_receive[0]==1)
//    {
//       canal=data_receive[1];
//       data_send[2]=potencia[canal];
//       data_receive[2]=potencia[canal];
//    }
//    if(IDL!=0 || IDH!=0)
//    {
//        Transmit_Radio(4,1);
//    }
//     ACK_transmit();
//       if(data_receive[0]==0 && data_receive[1]==0 )
//     {
//         UW("AQUI!!",1);
//         Radio_Check();
//     }
 
    return status;
}

// Checa em que estado o radio está
char Check_Radio(unsigned char mode) {

  unsigned char compare;
    CSN=0;
    Spi_TX(0x00);
    compare=Spi_TX(255);
    CSN=1;

    if (compare==mode) {
       return 1;
    } else {
       return 0;
    }
}

void Radio_Action() {
    unsigned char counter=0;
    unsigned char wait=0;
//    unsigned char index=0;
    if (power_radio_flag<11) {
        if (power_radio_flag==7) {
            if (data_receive[9]==200 && EEP_READ(250)==0 && EEP_READ(251)==0) {
                EEP_WRITE(250,data_receive[2]);        //config do ID individual e unico!
                EEP_WRITE(251,data_receive[3]);
                UW("ID changed",1);
            } else if(data_receive[9]==100) {
                EEP_WRITE(29,data_receive[2]);          //id low da central
                EEP_WRITE(30,data_receive[3]);          //id high da central

                EEP_WRITE(28,data_receive[4]);          //freq da central

                EEP_WRITE(31,data_receive[5]);         //addr da central
                EEP_WRITE(32,data_receive[6]);
                EEP_WRITE(33,data_receive[7]);
                EEP_WRITE(34,data_receive[8]);         //fim addr central

                UW("DEFs changed",1);

                IDL = data_receive[2];
                IDH = data_receive[3];
                data_send[0]=IDL;
                data_send[1]=IDH;
                data_send[2]=190;
                data_send[3]=190;
                data_send[4]=190;
                RetransCounter();

                Transmit_Radio(11,10);
            }
        } else if(data_receive[0]==IDL && data_receive[1]==IDH && (IDL!=0 || IDH!=0)) { //conectando a central
            if (data_receive[3]==94) {
                UL();
                Int_To_Str(data_receive[3],"RESET EEP:");

                data_send[0]=IDL;
                data_send[1]=IDH;
                data_send[2]=200;
                data_send[3]=200;
                data_send[4]=200;
                RetransCounter();

                Transmit_Radio(11,10);
                for(counter=0;counter<50;counter++)
                {
                    EEP_WRITE(counter,0xFF);
                    delay_ms(1, 1);
                }
                 EEP_WRITE(100,0xFF);

                Force_Shut_Down();
                delay_ms(2000, 1);
            } else if(data_receive[3]==33) {
                UL();
                Int_To_Str(data_receive[3],"EEP:");
                UL();
                for (counter=0;counter<50;counter++) {
                    Int_To_Str(counter,"");
                    Int_To_Str(EEP_READ(counter),":");
                    if(counter%10==0 && counter>0)
                    {
                        UL();
                    }
                    else
                    {
                        Int_To_Str(0,"  ");
                    }
                    delay_ms(1, 1);
                }
                Int_To_Str(100,"   ");
                Int_To_Str(EEP_READ(100),":");
                Int_To_Str(250,"   ");
                Int_To_Str(EEP_READ(250),":");
                Int_To_Str(251,"   ");
                Int_To_Str(EEP_READ(251),":");
            }
        //    else
        //    {
        //        start_program=1;
        //    }
        //
        //    SSN=1;
        //    if(data_receive[0]==0 && data_receive[3]!=94 && data_receive[3]!=33 && data_receive[3]!=25 && data_receive[3]!=70)
        //    {
        //        index = data_receive[1];
        //        power_radio_flag=2;
        //        if(index==0)
        //            power_radio_flag=1;
        //
        //        if(neutral==0 && index==0 && data_receive[2]>=1 )
        //        {
        //
        //             if(EEP_READ(0)<=3 && EEP_READ(run)>=55)
        //            {
        //
        //                 if(potencia[0]==0)
        //                potencia[0]=55;
        //                EEP_WRITE(20+index,55);
        //            }
        //            else if(EEP_READ(0)==5 && EEP_READ(1)>=55)
        //            {
        //
        //                 if(potencia[0]==0)
        //                potencia[0]=55;
        //                EEP_WRITE(20+index,55);
        //            }
        //            else
        //            {
        //                potencia[0]=EEP_READ(run);
        //                EEP_WRITE(20+index,potencia[0]);
        //            }
        //        }
        //        else
        //        {
        //            potencia[index]=data_receive[2];
        //            if(potencia[index])
        //            {
        //                if(!dimmer[index])
        //                    potencia[index]=100;
        //            }
        //            EEP_WRITE(20+index,potencia[index]);
        //            if(data_receive[0]==0)
        //            {
        //                if(data_receive[1]==0)
        //                {
        //                    power_radio_flag=1;
        //                }
        //                else
        //                {
        //                    power_radio_flag=2;
        //                }
        //            }
        //        }
        //        Slave_Interact(index, potencia[index], data_receive[0], 0);
        //    }
            else if(data_receive[2]==20)   //fade dimmer
            {
                data_send[2]=120;
                wait = data_receive[3] >> 6;
        //        UL();
        //        Int_To_Str(wait,"fade shift: ");
        //        Int_To_Str(data_receive[1],"                 fade full: ");
                data_receive[3]=data_receive[3] << 2;
                data_receive[3]=data_receive[3] >> 2;
                data_send[3]=data_receive[3];
                wait = wait << 2;
                waitdimmer = wait;
                wait = data_receive[4] >> 6;
        //        UL();
        //        Int_To_Str(wait,"dimmer shift: ");
        //        Int_To_Str(data_receive[2],"                 dimmer full: ");
                waitdimmer += wait;
                waitdimmer += 2;
                data_receive[4]=data_receive[4] << 2;
                data_receive[4]=data_receive[4] >> 2;
                data_send[4]=data_receive[4];
                UL();
                if(waitdimmer>10)
                    waitdimmer=10;
                Int_To_Str(waitdimmer,"Temp dimmer: ");
                EEP_WRITE(36,waitdimmer);
                data_send[5]=waitdimmer;
                if(neutral){
                    Slave_Interact(data_receive[3],150 ,2 , 0);
                    EEP_WRITE(5,data_receive[4]);
                    for(counter=0;counter<6;counter++)
                    {
                        dimmer[counter]=data_receive[4]%2;
                        data_receive[4]=data_receive[4]>>1;
                    }
                }
                else
                    dimmer[0]=1;
                data_send[6]=neutral;
 
                data_send[0]=IDL;
                data_send[1]=IDH;
                data_send[7]=0;
                data_send[8]=0;
                data_send[9]=0;
                data_send[2]=120;
                RetransCounter();
 
                Transmit_Radio(11,10);
 
            }
            else if (data_receive[2]==25)     //situacao do slave!
            {
                data_send[0]=IDL;
                data_send[1]=IDH;
                data_send[2]=130;
                data_send[3]=neutral;
                data_send[4]=EEP_READ(0);
                data_send[5]=EEP_READ(1);
                data_send[6]=EEP_READ(2);
                data_send[7]=EEP_READ(3);
                data_send[8]=EEP_READ(18);
                data_send[9]=EEP_READ(19);
                RetransCounter();
                Transmit_Radio(11,10);
            }
        }
    }

    SSN=1;
    Configure_RX_Ack();
    LED=1;
}

void RetransCounter() {
    if ((data_receive[10] & 7) == 7)
       data_send[10] = data_receive[10] & 0xf8;
    else
       data_send[10]=data_receive[10]++;
    if (meshretrans==data_send[10]&7) {
        if((data_receive[10] & 7) == 7)
           data_send[10] = data_receive[10] & 0xf8;
        else
           data_send[10]=data_receive[10]+1;
    }
}

