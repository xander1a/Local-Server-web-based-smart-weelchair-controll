#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>

//const char* ssid = "I have no life";
//const char* password = "1234567890";

const char* ssid = "A";
const char* password = "0786687893";

ESP8266WebServer server(80);

// Motor control pins




int ena = D0; 
int enb = D6;
int enbc=D8; 

int in11=D4;
int in22=D7;

int in1 = D1; 
int in2 = D2; 
int in3 = D3; 
int in4 = D5;

//int up_motor1=D2;
//int up_motor2=D7;



// Motor speed (0-255)
int motorSpeed = 0;

void setup() {
  Serial.begin(115200);

  // Initialize motor control pins


  pinMode(ena, OUTPUT); 
  pinMode(enb, OUTPUT); 
    pinMode(enbc, OUTPUT); 

  pinMode(in11, OUTPUT); 
  pinMode(in22, OUTPUT);

  pinMode(in1, OUTPUT); 
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
//  pinMode(up_motor1, OUTPUT);
//  pinMode(up_motor2, OUTPUT);


  

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Print the ESP8266's IP address to the serial monitor
  Serial.print("Connected to WiFi. IP address: ");
  Serial.println(WiFi.localIP());

  // Rest of your setup code here...



  // Define web server routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/forward", HTTP_GET, handleForward);
  server.on("/backward", HTTP_GET, handleBackward);
  server.on("/left", HTTP_GET, handleLeft);
  server.on("/right", HTTP_GET, handleRight);
  server.on("/speed", HTTP_GET, handleSpeed);
   server.on("/stop", HTTP_GET, handleStop);
  server.on("/up", HTTP_GET, handleUp);
  server.on("/down", HTTP_GET, handleDown);

  server.begin();
}

void loop() {
  server.handleClient();
}

void handleRoot() {

String htmlContent = R"html(
  <!DOCTYPE html>
  <html>
  <head>
    <meta name='viewport' content='width=device-width, initial-scale=1'>
    <style>
      body {
        font-family: Arial, sans-serif;
        background-color: #f2f2f2;
        text-align: center;
        margin: 0;
        padding: 0;
      }
      h1 {
        color: #333;
        background-color: #209e48;
        padding: 10px;
        margin: 0;
      }
      .container {
        display: flex;
        flex-direction: column;
        align-items: center;
        justify-content: center;
        padding: 10px;
      }
      .speed-label {
        font-size: 20px;
        color: #333;
        margin-bottom: 10px;
      }
      #speedValue {
        font-size: 24px;
        color: #c20a0a;
      }
      #speedSlider {
        width: 80%;
        max-width: 300px;
      }
      .button-container {
        display: flex;
        flex-wrap: wrap;
        justify-content: space-around;
        margin-top: 20px;
      }
      .button {
        background-color: #209e48;
        color: white;
        padding: 10px 20px;
        font-size: 18px;
        border: none;
        border-radius: 5px;
        margin: 5px;
        cursor: pointer;
        text-align: center;
        text-decoration: none;
        width: 45%;
      }
      .stop-button {
        background-color: #c20a0a;
      }
    </style>
  </head>
  <body>
    <h1>Car Robot Control</h1>
    <div class="container">
      <p class="speed-label">Speed: <span id="speedValue">0</span></p>
      <input type="range" id="speedSlider" min="100" max="255" onchange="updateSpeed()">
      <div class="button-container">
       <button class="button" onclick="moveBackward()">Forward</button>
        <button class="button" onclick="moveForward()">Back</button>
        <button class="button" onclick="turnLeft()">Right</button>
        <button class="button" onclick="turnRight()">Left</button>
        <button class="button stop-button" onclick="stopRobot()">Stop</button>
        <button class="button" onclick="moveUp()">Turn Right
        </button>
        <button class="button" onclick="moveDown()">Turn Left</button>
      </div>
    </div>
    <script>
      function updateSpeed() {
        var speed = document.getElementById("speedSlider").value;
        document.getElementById("speedValue").textContent = speed;
        var xhr = new XMLHttpRequest();
        xhr.open('GET', '/speed?value=' + speed, true);
        xhr.send();
      }
      function moveForward() {
        var xhr = new XMLHttpRequest();
        xhr.open('GET', '/forward', true);
        xhr.send();
      }
      function moveBackward() {
        var xhr = new XMLHttpRequest();
        xhr.open('GET', '/backward', true);
        xhr.send();
      }
      function turnLeft() {
        var xhr = new XMLHttpRequest();
        xhr.open('GET', '/left', true);
        xhr.send();
      }
      function turnRight() {
        var xhr = new XMLHttpRequest();
        xhr.open('GET', '/right', true);
        xhr.send();
      }
      function stopRobot() {
        var xhr = new XMLHttpRequest();
        xhr.open('GET', '/stop', true);
        xhr.send();
      }
      function moveUp() {
        var xhr = new XMLHttpRequest();
        xhr.open('GET', '/up', true);
        xhr.send();
      }
      function moveDown() {
        var xhr = new XMLHttpRequest();
        xhr.open('GET', '/down', true);
        xhr.send();
      }
    </script>
  </body>
  </html>
)html";


  server.send(200, "text/html", htmlContent);
}

void handleForward() {
  // Move forward
   Serial.println("Chair has moved forward");
   
  digitalWrite(in1, HIGH); 
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  server.send(200, "text/plain", "Forward");
}

void handleBackward() {
  // Move backward
  Serial.println("Chair has moved backward");
  digitalWrite(in1, LOW); 
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  server.send(200, "text/plain", "Backward");
}

void handleLeft() {
  // Turn left
  Serial.println("Chair has turned left");
  digitalWrite(in1, LOW); 
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  server.send(200, "text/plain", "Left");
}

void handleRight() {
  // Turn right
   Serial.println("Chair has turned right");
  digitalWrite(in1, HIGH); 
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  server.send(200, "text/plain", "Right");
}

void handleSpeed() {
  // Adjust motor speed
  if (server.hasArg("value")) {
    motorSpeed = server.arg("value").toInt();
      analogWrite(ena, motorSpeed);
         analogWrite(enb, motorSpeed);
  }
  server.send(200, "text/plain", "Speed set to " + String(motorSpeed));
}


void handleStop(){
  Serial.println("Chair has been stopped");
  analogWrite(ena, 0); 
  analogWrite(enb, 0);
   analogWrite(enbc, 0);
  digitalWrite(in1, LOW); 
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  server.send(200, "text/plain", "Stop");
}



void handleUp(){
   analogWrite(enbc, 180);
    digitalWrite(in11, HIGH); 
  digitalWrite(in22, LOW);
  server.send(200, "text/plain", "Up");
}



void handleDown(){
  analogWrite(enbc, 180);
    digitalWrite(in11, LOW); 
    digitalWrite(in22, HIGH);
  server.send(200, "text/plain", "Down");
}
