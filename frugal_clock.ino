

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char *ssid = "Andromeda";
const char *password = "Europa30";

char hstring[3] = "12";          // init
char mstring[3] = "1";          // init

int iHr = 1;
int iMin = 0;

ESP8266WebServer server ( 80 );


void handleRoot() {
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;
  server.send ( 200, "text/html", "Hello from the Frugal Clock!" );
}

void setTime() {
    if (server.arg("h")== "" && server.arg("m") == ""){     //Parameter not found
      Serial.println("Parameter 'h' (hour) and/or 'm' (minute) not specified");
      server.send(200, "text/html", "<H3>Parameter 'h' (hour) and/or 'm' (minute) not specified</H3>");
    } else {     //Parameter found
      if (server.arg("h") != "") { (server.arg("h")).toCharArray(hstring, 1 + (server.arg("h")).length());  iHr = String(hstring).toInt(); }
      if (server.arg("m") != "") { (server.arg("m")).toCharArray(mstring, 1 + (server.arg("m")).length());  iMin = String(mstring).toInt(); }
       
      Serial.println("time set to " + String(hstring) + ":" + String(mstring));
      server.send(200, "text/html", "<H3>time set to " + String(hstring) + ":" + String(mstring) + "</H3>");
    }
}



void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }

  server.send ( 404, "text/plain", message );
}



// bits representing segments A through G (and decimal point) for numerals 0-9
const int numeral[10] = {
   //ABCDEFG /dp
   B11111100, // 0
   B01100000, // 1
   B11011010, // 2
   B11110010, // 3
   B01100110, // 4
   B10110110, // 5
   B10111110, // 6
   B11100000, // 7
   B11111110, // 8
   B11110110, // 9
};

// pins for decimal point and each segment
// DP,G,F,E,D,C,B,A
const int segmentPins[] = { D1,D1,D2,D3,D4,D5,D6,D8 }; 
const int nbrDigits= 2; // the number of digits in the LED display
//dig 0 1 
const int digitPins[nbrDigits] = { D0,D7 };
const int dp_pin = 3;


void setup()
{
   Serial.begin(115200);
   WiFi.begin ( ssid, password );

   Serial.println ( "" );

   // Wait for connection
   while ( WiFi.status() != WL_CONNECTED ) {
     delay ( 500 );
     Serial.print ( "." );
   }

   Serial.println ( "" );
   Serial.print ( "Connected to " );
   Serial.println ( ssid );
   Serial.print ( "IP address: " );
   Serial.println ( WiFi.localIP() );

   if ( MDNS.begin ( "esp8266" ) ) {
     Serial.println ( "MDNS responder started" );
   }

   server.on("/", handleRoot);
   server.on("/set", setTime);
   server.onNotFound ( handleNotFound );
   server.begin();
  
   for(int i=0; i < 8; i++) {
      pinMode(segmentPins[i], OUTPUT); // set segment and DP pins to output
   }
   pinMode(dp_pin, FUNCTION_3);
   pinMode(dp_pin, OUTPUT);
   for(int i=0; i < nbrDigits; i++) {
      pinMode(digitPins[i], OUTPUT);
   }
} 

// Displays given number on a 7-segment display at the given digit position
void showDigit( int number, int digit)
{
   if(number == -1) digitalWrite( digitPins[digit], LOW );
   else digitalWrite( digitPins[digit], HIGH );
   for(int segment = 1; segment < 8; segment++)  {
      boolean isBitSet = bitRead(numeral[number], segment);
      // isBitSet will be true if given bit is 1
      // isBitSet = ! isBitSet; // Code Option*
      // uncomment the above Code Option line for common anode display
      digitalWrite( segmentPins[segment], isBitSet);
   }
   delay(5);
   digitalWrite( digitPins[digit], LOW );
}

void showNumber( int number, boolean zPad)
{
   if(number < 10) {
      if(zPad) showDigit( 0, nbrDigits-2) ;
   }
   if(number == 0) {
      showDigit( 0, nbrDigits-1) ;
//      showDigit( 0, nbrDigits-2) ;
   } else if(number == -1) {
      showDigit(number, 0);
      showDigit(number, 1);
   } else {
      // display the value corresponding to each digit
      // leftmost digit is 0, rightmost is one less than the number of places
      for( int digit = nbrDigits-1; digit >= 0; digit--)  {
         if(number > 0)  {
            showDigit( number % 10, digit) ;
            
            number = number / 10;
         }
      }
      //if(number < 10) showDigit( 0, 0) ;
   }
} 
 
void loop()
{
    digitalWrite( dp_pin, LOW );
   for(int i=0; i<200; i++) { showNumber(iHr, false); server.handleClient(); }   // Display Hours
   digitalWrite( dp_pin, HIGH );
   for(int i=0; i<200; i++) { showNumber(iMin, true); server.handleClient(); }   // Display Minutes
   for(int i=0; i<100; i++) { showNumber(-1, false); server.handleClient(); }    // Switch OFF bboth digits
 
} 
 

 
