int1 getAlarmData(char *commandx)
{
   int finalCnt=0;
   int16 cnz=0;
   int ii=0;
   char command_[35]={};
   char data_[35]={};
   char cx;
   
   strcpy(data_,"err");
   strcpy(command_,commandx);
   
choy:
   cnz=0;
   ii=0;
   fprintf(ALARM, "%s\n",command_);
   fprintf(PC, "%s\n",command_);
   delay_ms(10);
   
   do
   {
      while(!kbhit(ALARM))
      {
         cnz++;
         delay_us(10);
         if (cnz >= 64000)
         {
            if (finalCnt++ > 4)
            {
               strcpy(funcReturnVar,"ER+ER");
               return 0;
            }               
            goto choy;
         }               
      }
      
      cx = fgetc(ALARM);
      if (cx != ' ')
         data_[ii++] = cx;
      if (data_[ii-1] == '*')
      {
         data_[ii-1] = '\0';
         break;
      }            
    }while (ii < 100);
    strcpy(funcReturnVar,data_);
    fprintf(PC,"ALARM:=%s\n",funcReturnVar);
    return 1;
}



