#include "WiFi.h"
#include "ESPAsyncWebServer.h"

const char* ssid = "<insert wifi-name here>";
const char* password = "<insert wifi-password here>" ;

AsyncWebServer server(80);

const char INDEX_HTML_HOME[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head> 
<title>Senseo Web Server</title> 
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" href="data:,"> 
<style>
html{font-family: Helvetica; display:inline-block; margin: 0px auto; text-align: center;}
h1{color: #0F3376; padding: 2vh;}
p{font-size: 1.5rem;}
.button{display: inline-block; background-color: #e7bd3b; border: none; 
  border-radius: 4px; color: white; padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}
.button2{background-color: #4a1f15;}
</style>
</head>
<body> 
<h1>Senseo Web Server</h1> 
<p><strong>Welcome</strong></p>
<p><a href="/home/?heat=yes"><button class="button">Heat</button></a></p>
<p><a href="/home/?brew1=yes"><button class="button button2">Brew</button></a></p>
</body>
</html>
)=====";


const char BREWING_HTML[] PROGMEM = R"=====(
  <!DOCTYPE html>
  <html>
  <head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta http-equiv="refresh" content="25;URL=/home"> 
  <style>
  /* Center the loader */
  #loader {
    position: absolute;
    left: 50%;
    top: 30%;
    z-index: 1;
    width: 150px;
    height: 150px;
    margin: -75px 0 0 -75px;
    border: 16px solid #f3f3f3;
    border-radius: 50%;
    border-top: 16px solid #4f4746;
    width: 120px;
    height: 120px;
    -webkit-animation: spin 2s linear infinite;
    animation: spin 2s linear infinite;
  }
  
  @-webkit-keyframes spin {
    0% { -webkit-transform: rotate(0deg); }
    100% { -webkit-transform: rotate(360deg); }
  }
  
  @keyframes spin {
    0% { transform: rotate(0deg); }
    100% { transform: rotate(360deg); }
  }
  
  /* Add animation to "page content" */
  .animate-bottom {
    position: relative;
    -webkit-animation-name: animatebottom;
    -webkit-animation-duration: 1s;
    animation-name: animatebottom;
    animation-duration: 1s
  }
  
  @-webkit-keyframes animatebottom {
    from { bottom:-100px; opacity:0 } 
    to { bottom:0px; opacity:1 }
  }
  
  @keyframes animatebottom { 
    from{ bottom:-100px; opacity:0 } 
    to{ bottom:0; opacity:1 }
  }
  
  #myDiv {
    display: none;
    text-align: center;
  }
  
  html{font-family: Helvetica; display:inline-block; margin: 0px auto; text-align: center;}
  h1{color: #0F3376; padding: 2vh;}
  p{font-size: 1.5rem;}
  .button{display: inline-block; background-color: #ded5b8; border: none; 
    border-radius: 4px; color: white; padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}
  .button2{background-color: #4f4746;}
  </style>
  </head>
  <body onload="myFunction()" style="margin:0;">
  <p id="brewing"><a href="/home/"><button class="button button2" disable>Brewing ...</button></a></p>
  <div id="loader"></div>
  
  <div style="display:none;" id="myDiv" class="animate-bottom">
    <h1>Senseo Web Server</h1> 
  <p><strong>Brewing...</strong></p>
  <p><a href="/home/"><button class="button button2">Coffee is ready</button></a></p>
  <div class="loader"></div>
  </div>
  
  <script>
  var myVar;
  
  function myFunction() {
    myVar = setTimeout(showPage, 23000); //Set time needed to brew the coffee in milliseconds
  }
  
  function showPage() {
    document.getElementById("loader").style.display = "none";
    document.getElementById("brewing").style.display = "none";
    document.getElementById("myDiv").style.display = "block";
  }
  </script>
  
  </body>
  </html>
)=====";


const char HEATING_HTML[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<meta http-equiv="refresh" content="110;URL=/home"> 
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
/* Center the loader */
#loader {
  position: absolute;
  left: 50%;
  top: 30%;
  z-index: 1;
  width: 150px;
  height: 150px;
  margin: -75px 0 0 -75px;
  border: 16px solid #f3f3f3;
  border-radius: 50%;
  border-top: 16px solid #e7bd3b;
  width: 120px;
  height: 120px;
  -webkit-animation: spin 2s linear infinite;
  animation: spin 2s linear infinite;
}

@-webkit-keyframes spin {
  0% { -webkit-transform: rotate(0deg); }
  100% { -webkit-transform: rotate(360deg); }
}

@keyframes spin {
  0% { transform: rotate(0deg); }
  100% { transform: rotate(360deg); }
}

/* Add animation to "page content" */
.animate-bottom {
  position: relative;
  -webkit-animation-name: animatebottom;
  -webkit-animation-duration: 1s;
  animation-name: animatebottom;
  animation-duration: 1s
}

@-webkit-keyframes animatebottom {
  from { bottom:-100px; opacity:0 } 
  to { bottom:0px; opacity:1 }
}

@keyframes animatebottom { 
  from{ bottom:-100px; opacity:0 } 
  to{ bottom:0; opacity:1 }
}

#myDiv {
  display: none;
  text-align: center;
}

html{font-family: Helvetica; display:inline-block; margin: 0px auto; text-align: center;}
h1{color: #0F3376; padding: 2vh;}
p{font-size: 1.5rem;}
.button{display: inline-block; background-color: #e7bd3b; border: none; 
  border-radius: 4px; color: white; padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}
.button2{background-color: #4f4746;}
</style>
</head>
<body onload="myFunction()" style="margin:0;">
<p id="brewing"><a href="/home/"><button class="button" disable>Heating ...</button></a></p>
<div id="loader"></div>

<div style="display:none;" id="myDiv" class="animate-bottom">
  <h1>Senseo Web Server</h1> 
<p><strong></strong></p>
<p><a href="/home/"><button class="button">Machine is hot</button></a></p>
<div class="loader"></div>
</div>

<script>
var myVar;

function myFunction() {
  myVar = setTimeout(showPage, 108000);
}

function showPage() {
  document.getElementById("loader").style.display = "none";
  document.getElementById("brewing").style.display = "none";
  document.getElementById("myDiv").style.display = "block";
}
</script>

</body>
</html>
)=====";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(2, OUTPUT);
  digitalWrite(16, LOW);
  digitalWrite(17, LOW);
  digitalWrite(4, LOW);
  digitalWrite(2, LOW);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    digitalWrite(2, HIGH);
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  digitalWrite(2, HIGH);
  Serial.print("Connected\nIP address: ");
  Serial.println(WiFi.localIP());

  server.on("/home", HTTP_GET, [](AsyncWebServerRequest *request){

    int paramsNr = request->params();
    Serial.println(paramsNr);

    for(int i=0;i<paramsNr;i++){

        AsyncWebParameter* p = request->getParam(i);
        Serial.print("Param name: ");
        Serial.println(p->name());
        Serial.print("Param value: ");
        Serial.println(p->value());
        Serial.println("------");
        if(p->name()=="brew1" && p->value() == "yes"){
          Serial.println("Brewing Coffee");
          request->send(200, "text/html", BREWING_HTML);
          digitalWrite(16, HIGH);
          delay(500);
          digitalWrite(16, LOW);
        } else if(p->name()=="heat" && p->value() == "yes"){
          Serial.println("Heating Water");
          request->send(200, "text/html", HEATING_HTML);
          digitalWrite(17, HIGH);
          delay(500);
          digitalWrite(17, LOW);
        }
    }
    request->send(200, "text/html", INDEX_HTML_HOME);
  });
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // put your main code here, to run repeatedly:

}
