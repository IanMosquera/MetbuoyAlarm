

void sortWaterData_MBotix()
{
   int i=0, j=0;
   float temp=0;
   
   for (i=0; i<=(sampleCounter_MBotix-2); i++)
   {
      for (j=0;j<=((sampleCounter_MBotix-2)-i);j++)
      {  
         if(temp2Distance_MBotix[j]>temp2Distance_MBotix[j+1])  
         {  
            temp=temp2Distance_MBotix[j];  
            temp2Distance_MBotix[j]=temp2Distance_MBotix[j+1];  
            temp2Distance_MBotix[j+1]=temp;  
         }  
      }
   }
/*   
   for ( i = 0 ; i <= (sampleCounter_MBotix-1) ; i++ )
   {
      fprintf (PC, "after: %2.2f=%d\r", temp2Distance_MBotix[i],i ) ;
      fprintf (PC "------------------\r") ;
   }
*/
}

float computeDistanceX_MBotix()
{
   int i=0;
   int mid =0;

   float median;
   float filter_low;
   float filter_high;
   float valx=0;

   mid = sampleCounter_MBotix / 2;
//   fprintf(PC,"mid=%ul\n",mid);
//   fprintf(PC,"sampleCounter_MBotix=%ul\n",sampleCounter_MBotix);
   
   median = temp2Distance_MBotix[mid];
   if (sampleCounter_MBotix % 2 == 0)
      median = (temp2Distance_MBotix[mid] + temp2Distance_MBotix[mid-1])/2;
   else
      median = temp2Distance_MBotix[mid];
      
   filter_low = median - 10;
   filter_high = median + 10;
      
/*   
   printf ( "filter_low =%2.2f\r", filter_low) ;
   printf ( "median =%2.2f\r", median) ;
   printf ( "filter_high =%2.2f\r", filter_high) ;
*/   
   for ( i = 0 ; i <= (sampleCounter_MBotix-1) ; i++ )
   {
      if ((temp2Distance_MBotix[i] >= filter_low) && (temp2Distance_MBotix[i] <= filter_high))
      {
         if (valx == 0)
            valx = temp2Distance_MBotix[i];
         else
            valx = (valx + temp2Distance_MBotix[i]) / 2;
         //fprintf(PC,"vvv = %2.2f, valx = %2.2f\r",temp2Distance_MBotix[i],valx);
      }
   }
   //fprintf(PC,"VALX = %2.2f\r",valx);
   return valx;
}



void getDistance_MBotix()
{  
    char chrx; 
    long timeOut=0;
    char datax[30];
    float tempDistance=0;
    int x=0;
    sampleCounter_MBotix = 0;
    int i=0;
   
    output_low(OUT1_PIN);
    output_high(STAT_PIN);
    delay_ms(2000);
    //disable_interrupts(INT_EXT2);
    //disable_interrupts(GLOBAL);
   
    while(sampleCounter_MBotix!=20)
    {
      x=0;
      tempDistance=0;
      while(!kbhit(SNSR_WATER_MAXBOTIX)&&++timeOut<50000)
      {delay_us(10);}
         
      if(kbhit(SNSR_WATER_MAXBOTIX))
      {
         chrx = fgetc(SNSR_WATER_MAXBOTIX);
         if (chrx == 'R')
         {
            while(1)
            {
               //chrx = fgetc(SNSR_WATER_MAXBOTIX);
               datax[x++] = fgetc(SNSR_WATER_MAXBOTIX);
               if (x > 7)
                  break;
               if (datax[x-1] == '\r')
               {
                  datax[x-1] = '\0';
                  tempDistance = atol(datax);
                  fprintf(PC,"tempDistance=%2.2f\n",tempDistance);
                  if ((tempDistance < 5000) && (tempDistance > 500))
                  {
                     tempDistance = tempDistance / 10;
                     temp2Distance_MBotix[sampleCounter_MBotix++] = tempDistance;
                  }
                  else
                     tempDistance = 0;
                  break;
               }                      
            }
         }
         else
            fprintf(PC,"testing..\n");
      }
      if (tempDistance==0)
      {
         fprintf(PC,"INVALID DATA\n");
         i++;
      }         
      if (i > 30)
      {
         strcpy(funcReturnVar,"ERR");
         return;
      }
      
      delay_ms(400);
    }      
               
         
    delay_ms(200);
    output_low(STAT_PIN);
    sortWaterData_MBotix();
    tempDistance = computeDistanceX_MBotix();
    sprintf(funcReturnVar, "%2.2f", tempDistance);
    output_high(OUT1_PIN);
   }

   

/*
void getSample()
   {char *ptr;
    char data[60];
    char delimiter[3];
    char serverNumber[15];
    char distance[10];
    char extEepromMsg[60];
    int32 i;
    int tempCounter=0;
    float tempF=0;
    char rain[5];
    char health[50];
  
    ptr=getServerNumber();
    strcpy(serverNumber,ptr);
  
    ptr=getSystemHealthStatus();
    strcpy(health,ptr);
  
    itoa(rainCount,10,rain);
    fprintf(PC,"\nRain Count: %s",rain);
   
    tempF=sensorHeight-distanceGlobal;
    sprintf(distance, "%2.2f", tempF);
    fprintf(PC,"\nWater Level Height: %s",distance);
    rainCount=0;
   
    strcpy(delimiter,">");
    strcpy(data,rain);
    strcat(data,delimiter);
    strcat(data,distance);
    strcat(data,delimiter);
    strcat(data,health);
    strcpy(delimiter,"+");
    strcat(data,delimiter);
   
    fprintf(PC,"\n\ndata: ");
    powerExtEeprom(1);
    for (i=0;i<=strlen(data)-1;i++)
      {if(addressCount==EXT_EEPROM_SIZE)
         {addressCount=0;}
      
       if (lastSentAddress==addressCount) //error handler if lastSentAddress==addressCount
         {while(readExtEeprom(lastSentAddress)!='+')
            {lastSentAddress++;}
          writeExtEeprom(lastSentAddress, '^');
         }
          
       writeExtEeprom(addressCount, data[i]);
       //fprintf(PC,"%c",readExtEeprom(addressCount));
       addressCount++;
      }
    writeExtEeprom(addressCount, '*');
   
    i=lastSentAddress;
    lastSentAddress++;
    while(lastSentAddress!=addressCount)
      {if(lastSentAddress==EXT_EEPROM_SIZE)
         {lastSentAddress=0;}
           
       extEepromMsg[tempCounter]=readExtEeprom(lastSentAddress);
       if (extEepromMsg[tempCounter]=='+')
         {extEepromMsg[tempCounter+1]=NULL;
          fprintf(PC, "\n%s",extEepromMsg);
          if(sendSMS(extEepromMsg,serverNumber))
            {writeExtEeprom(lastSentAddress, '^');
             writeExtEeprom(i,'!');
             i=lastSentAddress;               
             restart_wdt();
            }
           else
            {break;}
          tempCounter=0;
         }
        
       else
         {tempCounter++;}
      
       lastSentAddress++;
      
       if (tempCounter==60)
         {tempCounter=0;}
      }
    lastSentAddress=i; 
   
    //fprintf(PC, "\nCurrent Address: %lu",addressCount);
    //fprintf(PC, "\nSent Address: %lu",lastSentAddress);
    powerExtEeprom(0);
   }


*/




