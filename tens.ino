#include <ESP8266WiFi.h>
#include <WiFiClient.h>

// Wi-Fi credentials
const char* ssid = "Projecty";
const char* password = "12345678";

// ThingSpeak settings
const char* server = "api.thingspeak.com";
const String writeAPIKey = "EQ803APP72XO5G18"; // Replace with your Write API Key
const int channelID = 2733660; // Replace with your Channel ID

// Parameters
const int samplingRate = 1000;      // Sampling rate in Hz
const int windowSizeMs = 200;       // Window size in milliseconds
const int bufferSize = samplingRate * (windowSizeMs / 1000.0); // Number of samples in one window
int emgBuffer[bufferSize];          // Buffer to hold EMG readings

// PWM Parameters
const int pwmPin = D1;              // Set this to the desired GPIO pin (e.g., D1)
int minFrequency = 2;               // Minimum PWM frequency in Hz
int maxFrequency = 10;              // Maximum PWM frequency in Hz
int painLevel = 0;                  // Placeholder for predicted pain level

// Function prototypes
void movingAverage(int* input, float* output, int size, int windowSize = 5);
float calculateRMS(float* data, int size);
float calculateMeanFrequency(float* data, int size);
float calculateVariance(float* data, int size);
float calculateAmplitude(float* data, int size);
void generatePWM(int painLevel);

WiFiClient client;

void setup() {
  Serial.begin(115200); // Start Serial communication for monitoring
  pinMode(pwmPin, OUTPUT); // Set up PWM pin as output

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");
}

void loop() {
  // Fill the buffer with EMG readings
  for (int i = 0; i < bufferSize; i++) {
    emgBuffer[i] = analogRead(A0);
    delayMicroseconds(1000000 / samplingRate); // Wait to match the sampling rate
  }

  // Apply Moving Average Filter
  float filteredData[bufferSize];
  movingAverage(emgBuffer, filteredData, bufferSize);

  // Feature Extraction
  float rms = calculateRMS(filteredData, bufferSize);
  float meanFrequency = calculateMeanFrequency(filteredData, bufferSize);
  float variance = calculateVariance(filteredData, bufferSize);
  float amplitude = calculateAmplitude(filteredData, bufferSize);

  // Display the results
  Serial.print("RMS: ");
  Serial.println(rms);
  Serial.print("Mean Frequency: ");
  Serial.println(meanFrequency);
  Serial.print("Variance: ");
  Serial.println(variance);
  Serial.print("Amplitude: ");
  Serial.println(amplitude);

  // Map mean frequency (100-200 Hz) to pain level (0-10)
  painLevel = map(meanFrequency, 100, 200, 0, 10); 
  painLevel = constrain(painLevel, 0, 10); // Ensure pain level stays within 0-10

  // Send data to ThingSpeak
  if (client.connect(server, 80)) {
    String postData = String("field1=") + String(rms) +
                      "&field2=" + String(meanFrequency) +
                      "&field3=" + String(variance) +
                      "&field4=" + String(amplitude) +
                      "&field5=" + String(painLevel) +
                      "&api_key=" + writeAPIKey;

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: " + String(server) + "\n");
    client.print("Connection: close\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: " + String(postData.length()) + "\n\n");
    client.print(postData);

    Serial.println("Data sent to ThingSpeak");
    client.stop();
  } else {
    Serial.println("Connection to ThingSpeak failed");
  }

  // Generate PWM based on the pain level
  generatePWM(painLevel);

  delay(5000); // Wait 5 seconds before the next data send
}

// Moving Average Filter
void movingAverage(int* input, float* output, int size, int windowSize) {
  for (int i = 0; i < size - windowSize + 1; i++) {
    float sum = 0;
    for (int j = 0; j < windowSize; j++) {
      sum += input[i + j];
    }
    output[i] = sum / windowSize;
  }
}

// Calculate RMS (Root Mean Square)
float calculateRMS(float* data, int size) {
  float sum = 0;
  for (int i = 0; i < size; i++) {
    sum += data[i] * data[i];
  }
  return sqrt(sum / size);
}

// Calculate Mean Frequency (Simple Approximation)
float calculateMeanFrequency(float* data, int size) {
  float meanFreq = 0;
  float totalEnergy = 0;

  for (int i = 1; i < size; i++) {
    float diff = data[i] - data[i - 1];
    meanFreq += abs(diff) * i; // Weighted by index for a simple approximation
    totalEnergy += abs(diff);
  }

  return totalEnergy > 0 ? meanFreq / totalEnergy : 0;
}

// Calculate Variance
float calculateVariance(float* data, int size) {
  float mean = 0;
  for (int i = 0; i < size; i++) {
    mean += data[i];
  }
  mean /= size;

  float variance = 0;
  for (int i = 0; i < size; i++) {
    variance += (data[i] - mean) * (data[i] - mean);
  }
  return variance / size;
}

// Calculate Amplitude (max - min)
float calculateAmplitude(float* data, int size) {
  float minVal = data[0];
  float maxVal = data[0];
  for (int i = 1; i < size; i++) {
    if (data[i] < minVal) minVal = data[i];
    if (data[i] > maxVal) maxVal = data[i];
  }
  return maxVal - minVal;
}

// Generate PWM based on pain level
void generatePWM(int painLevel) {
  // Map pain level to frequency range (2Hz to 10Hz)
  int pwmFrequency = map(painLevel, 0, 10, minFrequency, maxFrequency);
  int delayTime = 1000 / (2 * pwmFrequency); // Calculate half-period delay in ms

  // Generate PWM signal with calculated frequency
  digitalWrite(pwmPin, HIGH);
  delay(delayTime);
  digitalWrite(pwmPin, LOW);
  delay(delayTime);
}
