long buttonTimer = 0;
long longPressTime = 250;

boolean buttonActive = false;
boolean longPressActive = false;

int longPressCount = 0;

void handleButton() {
  if (digitalRead(BTN) == LOW) {
    if (buttonActive == false) {
      buttonActive = true;
      buttonTimer = millis();
    }
    if ((millis() - buttonTimer > longPressTime) && (longPressActive == false)) {
      //long press
      clear_display();
      longPressActive = true;
      setupMode = true;
      longPressCount++;
      switch(longPressCount){
        case 1: 
          if(h>9)
          {
            disp_buffer[8]=c2h(h%10);
            disp_buffer[6]=c2h((h/10)%10);
            if(dot==1){
              disp_buffer[8] += 0x01;
            }
          }
          else
          {
            disp_buffer[8]=c2h(h%10);
            disp_buffer[6]=0x00;
            if(dot==1){
              disp_buffer[8] += 0x01;
            }
          }
          break;
        case 2:
          disp_buffer[12]=c2h(m%10);
          disp_buffer[10]=c2h((m/10)%10);
          break;
        case 3:
          setupMode = false;
          longPressCount = 0;
          break;
      }
    }

  } else {
    if (buttonActive == true) {
      if (longPressActive == true) {
        longPressActive = false;
      } else {
        
        //short press
        switch(longPressCount){
          case 1: 
            h+=1;
            h=h>12?1:h;
            if(h>9)
            {
              disp_buffer[8]=c2h(h%10);
              disp_buffer[6]=c2h((h/10)%10);
              if(dot==1){
                disp_buffer[8] += 0x01;
              }
            }
            else
            {
              disp_buffer[8]=c2h(h%10);
              disp_buffer[6]=0x00;
              if(dot==1){
                disp_buffer[8] += 0x01;
              }
            }
            break;
          case 2:
            m+=1;
            m=m>59?0:m;
            disp_buffer[12]=c2h(m%10);
            disp_buffer[10]=c2h((m/10)%10);
            break;
        }
      }
      buttonActive = false;
    }
  }
}
