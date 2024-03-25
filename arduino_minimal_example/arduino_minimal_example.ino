void setup()
{
  Serial.begin(9600); // Initialize serial at 9600 baud rate
}

void loop()
{
  Serial.println("Hello from Second Arduino"); // Send message
  delay(1000); // Wait for a second
}
