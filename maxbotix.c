

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

   
   median = temp2Distance_MBotix[mid];
   if (sampleCounter_MBotix % 2 == 0)
      median = (temp2Distance_MBotix[mid] + temp2Distance_MBotix[mid-1])/2;
   else
      median = temp2Distance_MBotix[mid];
      
   filter_low = median - 10;
   filter_high = median + 10;
      
  
   for ( i = 0 ; i <= (sampleCounter_MBotix-1) ; i++ )
   {
      if ((temp2Distance_MBotix[i] >= filter_low) && (temp2Distance_MBotix[i] <= filter_high))
      {
         if (valx == 0)
            valx = temp2Distance_MBotix[i];
         else
            valx = (valx + temp2Distance_MBotix[i]) / 2;

      }
   }
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

   






