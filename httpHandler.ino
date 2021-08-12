
void handleRoot(){
  String message = "Current NTP time: ";
  message += h;
  message += " : ";
  message += m;
  server.send(200, "text/plain", message);
}
