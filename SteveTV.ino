#include <TVout.h>
#include <fontALL.h>




TVout TV;

uint8_t h=120;
uint8_t v=96;


// ball types
const uint8_t BALL_TYPE=0;
const uint8_t SNOW_TYPE=1;
const uint8_t QIX_TYPE=2;
const uint8_t RAIN_TYPE=3;
const uint8_t BUBBLE_TYPE=4;
const uint8_t FIRE_TYPE=5;
const uint8_t WARP_TYPE=6;
const uint8_t GRID_TYPE=7;
const uint8_t CLOUDS_TYPE=8;
const uint8_t MAX_TYPE=CLOUDS_TYPE;

const uint8_t COUNTER_TYPE=9;
uint8_t ballType=FIRE_TYPE;

// ball
const uint8_t MAX_BALL_R=8;
const uint8_t BOUNCE_LEN=100;
const uint8_t BSIZE=20;

float x[BSIZE];
float y[BSIZE];
float xadd[BSIZE];
float yadd[BSIZE];
uint8_t r[BSIZE];

uint8_t BALLS=10;

uint8_t loopcount=0;
uint8_t loops=100; // # loops until reset

// snow
const uint8_t FLAKE_HEIGHT=6;
const uint8_t FLAKE_WIDTH=3;
const uint8_t FLAKETYPES=4;
char flakes[FLAKETYPES];    
uint8_t flakesLanded = 0;
uint8_t driftHeight=1;
const uint8_t FLAKES_PER_DRIFT=40;
const uint8_t MAX_DRIFT=8;


// rain
const uint8_t SPLASHTYPES=4;
char splashes[SPLASHTYPES];

//bubbles
const uint8_t BUBBLE_GRPH=h/60;
const uint8_t BUBBLE_GRPV=v/2;
const uint8_t BUBBLETYPES=4;
char bubbles[BUBBLETYPES];


// fire
const uint8_t MIN_SPARKSIZE=0;
const uint8_t MAX_SPARKSIZE=18;
const uint8_t FIRE_WIDTH = 26;
uint8_t fire_center;

// counter
const uint8_t FONT_SIZE=3;
const uint8_t COUNTER_SPEED=7;//  more = slower


// warp 
const uint8_t STAR_DIV=20;

// grid
const uint8_t HORIZON=v/2;
const uint8_t BG_HEIGHT=10;
const uint8_t BG_WIDTH=25;
const uint8_t NUM_GRID_LINES=6;
const uint8_t NUM_PEAKS=6;
const uint8_t NUM_STARS=5;
const uint8_t MOON_R=10;

// bg types
const uint8_t MOUNTAIN=1;
const uint8_t GRIDLINE=2;
const uint8_t MOON=3;
const uint8_t STAR=4;


int BUTTON_PIN = 2;   // input pin for pushbutton

void setup() {  // this is run only once
  TV.begin(NTSC,h, v);

  randomSeed(analogRead(0));
 
  // initialize the pushbutton pin as an input
  pinMode(BUTTON_PIN, INPUT_PULLUP);


  // set up characters used as graphics
  bubbles[0]='.';
  bubbles[1]='*';
  bubbles[2]='o';
  bubbles[3]='O';

  flakes[0]='.';
  flakes[1]='.';
  flakes[2]=',';
  flakes[3]='\'';

  splashes[0]='+';
  splashes[1]='@';
  splashes[2]='+';
  splashes[3]='%';
   
  mySetup();

} // end of setup



void mySetup() { // this resets everything


  loops=10; // # loops until reset

  TV.clear_screen();



  
  if (ballType==BALL_TYPE ) {
    BALLS=10;
    loops=15; // # loops until reset
  }
  
  if (ballType==QIX_TYPE ) {
    BALLS=2;
  }
  
  if (ballType==SNOW_TYPE ) {
    BALLS=20;
    flakesLanded = 0;
    driftHeight=1;
    loops=MAX_DRIFT*4;
  }

  if (ballType==RAIN_TYPE ) {
    BALLS=20;
    
  }

  if (ballType==FIRE_TYPE ) {
    BALLS=20;
    fire_center = random( (h/2)-FIRE_WIDTH,(h/2)+FIRE_WIDTH-MAX_SPARKSIZE);
  }

  if (ballType==BUBBLE_TYPE ) {
    BALLS=15;

  }

  if (ballType==COUNTER_TYPE ) {
    BALLS=4;
  }

  if (ballType==WARP_TYPE ) {
    BALLS=20;
  }

  if (ballType==GRID_TYPE ) {
    BALLS=NUM_GRID_LINES+NUM_PEAKS+1+NUM_STARS;
  } // end of if grid


  if (ballType==CLOUDS_TYPE ) {
    BALLS=5;
  } // end of if CLOUDS_TYPE

  //char errcode;
  //errcode= 
  
  TV.select_font(font4x6);
    
  /*
  String text=String("Tv.begin:");
 if (errcode == 0) {
      text=text+"no error.";
 }
  if (errcode == 1) {
      text=text+"X is not a multiple of 8.";
  }
 if (errcode == 2) {
      text=text+"Y is too large";
 }
 if (errcode == 4) {
      text=text+"there is not enough memory for the frame buffer.";
  }
  Serial.begin(57600);
  Serial.println(text);
  

 */





 
  // set up balls  
  for (uint8_t b = 0; b < BALLS; b++) {


    
   if (ballType == BALL_TYPE) {
    xadd[b]=.001*random(100,1000);
    yadd[b]=.001*random(100,1000);
    r[b]=random(0,MAX_BALL_R);
    y[b]=0;
    x[b]=random(0,h-r[b]);
    if (random(0,2) == 1) {  // randmonly opposite diretion
      xadd[b]=xadd[b]*-1;
    }
   }

   if (ballType == BUBBLE_TYPE) {
       r[b]=random(0,BUBBLETYPES);
       x[b]=random(0,h*BUBBLE_GRPH)/BUBBLE_GRPH;  // group bubbles closer toger on x by only allowing multiples of n
       y[b]=random(0,v*BUBBLE_GRPV)/BUBBLE_GRPV;  // group bubbles closer toger on y by only allowing multiples of n
       xadd[b]=0;       
       yadd[b]=-1;
       xadd[b]=.001*random(10,200);
       if (random(0,2) == 1) {  // randmonly opposite diretion
          xadd[b]=xadd[b]*-1;
       }
   }
   
   if (ballType == SNOW_TYPE) {
       xadd[b]=.001*random(10,200);
       if (random(0,2) == 1) {  // randmonly opposite diretion
          xadd[b]=xadd[b]*-1;
       }
       yadd[b]=0;//.001*random(10,200);
       x[b]=random(0,h-FLAKE_WIDTH);    
       y[b]=random(0,v-1);
      r[b]=random(0,FLAKETYPES-1);
   }

   if (ballType == QIX_TYPE) {
        x[b]=random(0,h-r[b]);
        y[b]=random(0,v-1);
        r[b]=1;
        xadd[b]=.003*random(500,1000);
        yadd[b]=.003*random(500,1000);
        if (random(0,2) == 1) {  // randmonly opposite diretion
          xadd[b]=xadd[b]*-1;
        }
   }

   if (ballType == RAIN_TYPE) {
        x[b]=random(0,h-1);
        y[b]=random(0,v-1);
        r[b]=1;
        yadd[b]=10;
        xadd[b]=0;
   }

   if (ballType == FIRE_TYPE) {
        r[b]=random(1,MAX_SPARKSIZE);        
        if ( (b%2) == 1) {  // put half of fire at center area
          x[b]=random(fire_center-FIRE_WIDTH/2,fire_center+FIRE_WIDTH/2);
        } else {
          x[b]=random(fire_center-FIRE_WIDTH-1,fire_center+FIRE_WIDTH);
        }
        
        
        y[b]=v-(r[b]/2);
        yadd[b]=-2;
        xadd[b]=0.6;
   }


   if (ballType == COUNTER_TYPE) {
      switch (FONT_SIZE) {
      case 0:
        TV.select_font(font4x6);
      break;
      case 1:
        TV.select_font(font6x8);
      break;
      case 2:
        TV.select_font(font8x8);
      break;
      case 3:
        TV.select_font(font8x8ext);
      break;
   }
   }

   
   if (ballType == WARP_TYPE) {
      xadd[b]=.01*random(0,500);
      yadd[b]=.01*random(0,500);
      r[b]=0;
      switch (random(1,5)) {
        case 1:
          xadd[b]=xadd[b]*-1;
        break;
        case 2:
          yadd[b]=yadd[b]*-1;
        break;
        case 3:
          xadd[b]=xadd[b]*-1;
          yadd[b]=yadd[b]*-1;
        break;
      }
      
      y[b]=v/2+2*yadd[b]; // start them off-center
      x[b]=h/2+2*xadd[b];
  } // end of warp



    if (ballType == GRID_TYPE) {

      if (b<NUM_GRID_LINES) {
        xadd[b]=0;
        yadd[b]=1;
        // set up grid-lines evenly-spaced.  Lines will only be shown /sped up below the horizon
        y[b]=HORIZON-(NUM_GRID_LINES*3)+(b*3);
        r[b]=GRIDLINE;
      }
      if (b>=NUM_GRID_LINES and b<(NUM_GRID_LINES+NUM_PEAKS) ) {
        xadd[b]=1;
        yadd[b]=0;        
        y[b]=HORIZON-BG_HEIGHT;
        x[b]=(b-NUM_GRID_LINES+1)*BG_WIDTH;
        r[b]=MOUNTAIN;
      }

      if (b>=(NUM_GRID_LINES+NUM_PEAKS) and b<(NUM_GRID_LINES+NUM_PEAKS+NUM_STARS)) {
        xadd[b]=.02;
        yadd[b]=0;        
        y[b]=random(0,HORIZON-BG_HEIGHT);
        x[b]=random(0,h-1);
        r[b]=STAR;
      }

      // put moon last so it will cover stars
      if (b ==(NUM_GRID_LINES+NUM_PEAKS+NUM_STARS)) {      
        xadd[b]=.1;
        yadd[b]=0;        
        y[b]=random(MOON_R,HORIZON-BG_HEIGHT-MOON_R-1);
        x[b]=random(MOON_R,h-MOON_R-1);
        r[b]=MOON;
      }
      
    
    } // end of grid


   if (ballType == CLOUDS_TYPE) {
        xadd[b]=random(1,5)*.1;
        yadd[b]=0;        
        r[b]=random(9,21);
 
        x[b]=random(r[b]+1,h-r[b]-1);
        y[b]=random(r[b]+1,v-r[b]-1);  
   }
    

  
  }//  end of for balls

  loopcount =0;
}  // end of mySetup


void loop() { // this does the main display

 for (uint16_t l=0; l < loops*100; l++) {

 for (uint8_t b = 0; b < BALLS; b++) {
   boolean restart=false;
  
  if  (digitalRead(BUTTON_PIN) == LOW)  { // check pushbutton 
    ballType++;
    if (ballType > MAX_TYPE ) {
      ballType =0;
    }

    TV.delay_frame(50);
    mySetup();  
   }

      // erase previous cloud
      if (ballType == CLOUDS_TYPE) {  
        TV.draw_circle(x[b],y[b],r[b]/3,BLACK, BLACK);
        TV.draw_circle(x[b]+r[b]/3,y[b],r[b]/3+1,BLACK, BLACK);
        TV.draw_circle(x[b]+r[b]/3*2,y[b],r[b]/3,BLACK, BLACK);
        TV.draw_circle(x[b]+r[b]/2,y[b]-r[b]/3,r[b]/4,BLACK, BLACK);
      }


      // show counter
      if (ballType == COUNTER_TYPE) {
        TV.print(h/2,v/2, b+1);
        TV.delay_frame(10*COUNTER_SPEED);
      }

      // erase previous grid element
      if (ballType == GRID_TYPE) {     
  
        switch (r[b]) {
            case MOUNTAIN:
              if ( (x[b]+BG_WIDTH/2) < h-1) {
                TV.draw_line(x[b],y[b],x[b]+BG_WIDTH/2,y[b]+BG_HEIGHT,BLACK);
              }
              if ((x[b]-BG_WIDTH/2)< h-1) {
                TV.draw_line(x[b],y[b],x[b]-BG_WIDTH/2,y[b]+BG_HEIGHT,BLACK);
              }
            break;

            case MOON:
                TV.draw_circle(x[b],y[b],MOON_R,BLACK,BLACK);
            break;

             case STAR:
                TV.set_pixel(x[b],y[b],BLACK);
             break;
        } // end of switch
                
            
        if (r[b] == GRIDLINE) {          
          if (y[b] > HORIZON) {   //will only be shown /sped up below the horizon         
            TV.draw_line(0,y[b],h-1,y[b],BLACK);        
            // accelrte toward bottom
            yadd[b]=yadd[b]+.3;
          }
        } // end of gridline
        
      }  // end of grid

      // erase previous star
      if (ballType == WARP_TYPE) {
        TV.draw_circle(x[b],y[b],r[b]/STAR_DIV,BLACK, BLACK);

        // accelrte toawrd edge
        const float accel=.02;
        if (yadd[b]<0) {
          yadd[b]=yadd[b]-accel;  
        } else {
          yadd[b]=yadd[b]+accel;  
        }

        if (xadd[b]<0) {
          xadd[b]=xadd[b]-accel;  
        } else {
          xadd[b]=xadd[b]+accel;  
        }
        
        r[b]++;
        
      }


      
   // erase the previous ball 

      if (ballType == BALL_TYPE) {
        TV.draw_circle(x[b],y[b],r[b],BLACK);
        yadd[b]=yadd[b]+.02;  // gravity
      } 

      
      if (ballType == BUBBLE_TYPE) {
           TV.print_char(x[b],y[b], ' ');
        yadd[b]=yadd[b]-.02;  // accelation upward
        if (random(0,v/5) == 1) {  // randmonly flip direction
          xadd[b]=xadd[b]*-1;
       }
      }
   

   // erase the previous spark
   if (ballType == FIRE_TYPE) {
    // linger sparks at center-bottom
      if (
        (r[b]!=MAX_SPARKSIZE) or
        (x[b]<fire_center-FIRE_WIDTH/2) or
        (x[b]>fire_center+FIRE_WIDTH/2 ))  {        
         TV.draw_rect(x[b],
          y[b],
          r[b],r[b],
          BLACK, BLACK);
        }
     
       yadd[b]=yadd[b]-.02;  // accerlation upward
       //gets smaller
       r[b]=r[b]-1;                
       if (r[b] <= MIN_SPARKSIZE) {
        if (random(0,2) == 1) {  // random extra spark at top
          r[b]=1;
          if (random(0,60) == 1) {
            // fire-crackle sound
            TV.tone(1,20); 
          }
        } else {
         restart=true;
        }
       } 

   } //end of if fire

  
   // erase previous flake/handle drift
   if (ballType == SNOW_TYPE) {
      yadd[b]=yadd[b]+.006;// gravity

 
      if (y[b]>(v-driftHeight-FLAKE_HEIGHT/2))  {        // bottom
        restart= true;
        flakesLanded++;
        TV.draw_circle(x[b],v, driftHeight-1,WHITE,WHITE);  

        if (flakesLanded > FLAKES_PER_DRIFT) {
          flakesLanded =0;
          if (driftHeight <MAX_DRIFT) {
            driftHeight++;
          }
        }
        
      } else {           
        TV.print_char(x[b],y[b],' ');          
      }

      
   } // end of snow

 



  // erase the previous QIX
   if ( (ballType == QIX_TYPE) and (b==0) ){
        TV.draw_line(x[0],y[0],x[1],y[1],BLACK);
   }

   
   // move the ball
   x[b]=x[b]+xadd[b];
   y[b]=y[b]+yadd[b];

  // draw cloud
  if (ballType == CLOUDS_TYPE) {
    // right
    if (x[b]>(h-r[b]-1)) {
      // reset cloud
        xadd[b]=random(1,5)*.1;
        r[b]=random(9, 21);
        x[b]=r[b]/2;
        y[b]=random(r[b]+1,v-r[b]-1);        
      
    } else {
        TV.draw_circle(x[b],y[b],r[b]/3,WHITE, WHITE);
        TV.draw_circle(x[b]+r[b]/3,y[b],r[b]/3+1,WHITE, WHITE);
        TV.draw_circle(x[b]+r[b]/3*2,y[b],r[b]/3,WHITE, WHITE);
       TV.draw_circle(x[b]+r[b]/2,y[b]-r[b]/3,r[b]/4,
        WHITE, WHITE);

      TV.delay_frame(1);
    }
  }



  if (ballType == GRID_TYPE) {
    // background elememts
    if ( (r[b]==MOUNTAIN)
      or (r[b]==MOON)
      or (r[b]==STAR)
      ) {
      // detect left/right
      if (x[b] <0) {
          x[b]=h-1;          
          restart=true;
        }
        if (x[b] > h-1) {
          // reset       
          x[b]=0;
          restart=true;
        }        

        if (!restart) {
          switch (r[b]) {
            case MOUNTAIN:        
              if ( (x[b]+BG_WIDTH/2) < h-1) {
                TV.draw_line(x[b],y[b],x[b]+BG_WIDTH/2,y[b]+BG_HEIGHT,WHITE);
              }
              if ( (x[b]-BG_WIDTH/2) < h-1) {
                TV.draw_line(x[b],y[b],x[b]-BG_WIDTH/2,y[b]+BG_HEIGHT,WHITE);
              }
             break;
                   
             case MOON:
                if (x[b] > h-1-MOON_R) {
                  // reset       
                  x[b]=MOON_R;
                  y[b]=random(MOON_R,HORIZON-BG_HEIGHT-MOON_R-1);        
                  restart=true;
                } else {
                  TV.draw_circle(x[b],y[b],MOON_R,WHITE,BLACK);
                }
             break;
             
             case STAR:
                TV.set_pixel(x[b],y[b],WHITE);
             break;
             
             
          } // end of switch
          
        } // end of !restart
        
    } // end of bg elements


    // line elememts
    if (r[b] == GRIDLINE) {
      // detect bottom
       if (y[b]>(v-1)) {
        // reset line
        y[b]=HORIZON;
        yadd[b]=1;
       } else {      
        //will only be shown /sped up below the horizon
        if (y[b] > HORIZON) {
          // draw gridline
          TV.draw_line(0,y[b],h-1,y[b],WHITE);
        }
        TV.delay_frame(1);        
      }
    } //end of gridline
    
  } // end of grid
 

           

  if (ballType == WARP_TYPE) {
    
    // detect edges
    if (
      // right
      (x[b]>(h-1-(r[b]/STAR_DIV)))
       or
       //left
      (x[b]<(r[b]/STAR_DIV)) 
          or
       //top   
      (y[b]<(r[b]/STAR_DIV)) 
          or
      // bottom
      (y[b]>(v-1-(r[b]/STAR_DIV)))
      ) {  
        // restart star
        xadd[b]=.01*random(0,500);
        yadd[b]=.01*random(0,500);
        r[b]=0;
        switch (random(1,5)) {
          case 1:
           xadd[b]=xadd[b]*-1;
          break;
          case 2:
            yadd[b]=yadd[b]*-1;
          break;
          case 3:
            xadd[b]=xadd[b]*-1;
            yadd[b]=yadd[b]*-1;
          break;
        }
      y[b]=v/2+2*yadd[b]; // start them off-center
      x[b]=h/2+2*xadd[b];

    } else { // show star
        TV.draw_circle(x[b],y[b],r[b]/STAR_DIV,WHITE,WHITE);
    }
    
  } // end of if warp

  if (ballType == SNOW_TYPE) {
    
    // detect edges

    // right
    if (x[b]>=(h-(FLAKE_WIDTH/2))) {
      restart=true;
    }

    //left
    if (x[b]<0) {
      restart=true;
    }


    if (restart) {
      // restart flake
      xadd[b]=.001*random(10,200);
      if (random(0,2) == 1) {  // randomly opposite diretion
        xadd[b]=xadd[b]*-1;
      }
      yadd[b]=.001*random(10,200);
      y[b]=0;
      x[b]=random(0,h-FLAKE_WIDTH);    
      r[b]=random(0,FLAKETYPES-1);
    } else { // show flake
      TV.print_char(x[b],y[b],flakes[r[b]]);
    }
    
  } // end of if SNOW


  if ( (ballType==FIRE_TYPE) ){          
      if (restart) {
        // restart spark
        if ((b%2) == 1) {  // put half of fire at center area
          x[b]=random(fire_center-FIRE_WIDTH/2,fire_center+FIRE_WIDTH/2);
          r[b]=MAX_SPARKSIZE;
        } else {
          r[b]=MAX_SPARKSIZE/2;
          x[b]=random(fire_center-FIRE_WIDTH-1,fire_center+FIRE_WIDTH);

        }
               
        y[b]=v-(r[b]/2);
        yadd[b]=-2;
        xadd[b]=0.6;
        

      } else {
        // show spark
       TV.draw_rect(x[b],
        y[b],
        r[b],r[b],
        WHITE, WHITE); 
        
      }

  }// end of FIRE 

  if ((ballType==BALL_TYPE) or (ballType==QIX_TYPE) or (ballType==RAIN_TYPE) or (ballType==BUBBLE_TYPE)
  ) {
   // detect bounce

   // right
   if (x[b]>(h-r[b]-1)) {
    x[b]=h-r[b]-1;
    xadd[b]= xadd[b]*-1;   
    if (ballType!=BUBBLE_TYPE) {
      TV.tone(MAX_BALL_R-r[b]+1,BOUNCE_LEN);
    }
   }

   // left
   if (x[b]<(r[b]+1)) {
    x[b]=r[b]+1;
    xadd[b]= xadd[b]*-1;  
    if (ballType!=BUBBLE_TYPE) {
      TV.tone(MAX_BALL_R-r[b]+1,BOUNCE_LEN); 
    }
   }

    // bottom
   if (  (y[b]>(v-r[b]-1)) and (ballType!=BUBBLE_TYPE))  {
    y[b]=v-r[b]-1;
    if (abs(yadd[b]) > 0.1) { // only make bounce noice if its still moving up/down
      TV.tone(MAX_BALL_R-r[b]+1,BOUNCE_LEN);
    }
    if (ballType != QIX_TYPE) {
      float p = 100-(4*(r[b]+1));  // the larger the ball, the less % of energy returned 
      yadd[b]= yadd[b]*-1*(p/100);    
    } else {
      yadd[b]= yadd[b]*-1;
    }


   
    if (ballType == RAIN_TYPE) {
      // rain has reached bottom, erase and reset
      TV.draw_line(x[b],0,
        x[b],v-1,
        BLACK);
        
        // draw splash
        TV.print_char(x[b],v-3,splashes[random(0,SPLASHTYPES-1)]);
        TV.delay_frame(1);
        TV.print_char(x[b],v-3,' ');
       
        
        x[b]=random(0,h-1);
        y[b]=0;
        r[b]=1;
        yadd[b]=10;
        xadd[b]=0;
        restart=true;
    } // end of rain

    
   } // end of bottom

   // top
   if(y[b]<(r[b]+1)) {
    if (ballType==BUBBLE_TYPE) {
      // reset bubble
      r[b]=random(0,BUBBLETYPES);
      x[b]=random(0,h*BUBBLE_GRPH)/BUBBLE_GRPH;  // group bubbles closer toger on x by only allowing multiples of n
      y[b]=v-1;
       xadd[b]=0;       
       yadd[b]=-1;

       xadd[b]=.001*random(10,200);
       if (random(0,2) == 1) {  // randmonly opposite diretion
          xadd[b]=xadd[b]*-1;
       }
       // bubble pop sound
       TV.tone(1,5); 
    } else {
      y[b]=r[b]+1;
      yadd[b]= yadd[b]*-1;    
      TV.tone(MAX_BALL_R-r[b]+1,BOUNCE_LEN);
    }
   } // end of top

    // draw the ball
    if ((ballType == BALL_TYPE) ) {
        TV.draw_circle(x[b],y[b],r[b],WHITE);
    }
    if (ballType==BUBBLE_TYPE) {
        TV.print_char(x[b],y[b], bubbles[r[b]]);
    }

    // show the QIX
    if (ballType == QIX_TYPE) {
      if (b==1){
        TV.draw_line(x[0],y[0],x[1],y[1],WHITE);
        TV.delay_frame(1);
      }
    }


    // draw raindrop / trail
    if ((ballType==RAIN_TYPE) and !restart) {
  
      TV.draw_line(x[b],y[b]-random(10,30),
        x[b],y[b],
        WHITE);

    }  // end of rain


  }// end of BALL or QIX or rain or bubble


        
 }// end of for
 


}// end of loops

// reset verything
mySetup();

}// end of loop()
