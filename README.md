**SWASTHI** is a wearable TENS (Transcutaneous Electrical Nerve Stimulation) device integrated with Machine Learning to provide personalized period pain relief for women. The project uses EMG sensor data to predict cramping episodes and pain levels, delivering customized TENS therapy accordingly.

**Background Research and Survey**:

Google form : https://docs.google.com/forms/d/e/1FAIpQLSe8OiSM0lFfRg46Y8NLGstNKTqxEuhQJLiHiHcdDb_ZsRtxoA/viewform?usp=sf_link

Responses : https://docs.google.com/spreadsheets/d/1R7PQS3YQPm_RBV2VIW749AZy8LearTjYBM5kFcjOqLw/edit?usp=sharing


**Hardware:**
A TENS device controlled via an ESP8266 microcontroller connected to an EMG sensor for real-time data acquisition.

**Software:**
Machine learning algorithms trained on EMG data to detect muscle cramps and predict pain levels, enabling adaptive TENS therapy.


**Features:**
1. Real-time cramping detection and pain level prediction.
2. Customizable TENS therapy based on individual pain thresholds.
3. Slim, aesthetically pleasing, and ergonomic casing designed for daily wear.


**Block Diagram**

![WhatsApp Image 2024-11-08 at 06 29 33_931b36ef](https://github.com/user-attachments/assets/c0ee4da4-3024-46af-9d0f-23d7c0d47d32)

Flow Chart:
![WhatsApp Image 2024-11-08 at 08 22 35_307c1739](https://github.com/user-attachments/assets/8a32c608-22a5-4149-a162-faa6e47444fd)


**Files in this Repository**
1. Dataset_TENS.csv: A dataset containing raw EMG sensor data.
2. Dataset_TENS_with_status.csv: Supervised version of the dataset with an added status column indicating cramping (1 for cramping, 0 for not cramping) and Pain Level (Ranging from 1 to 10)
3. Dataset_creation_and_model_training.ipynb: A Jupyter notebook for preprocessing the dataset and training machine learning models to detect cramping and predict pain levels.
4. tens.ino: Arduino code for controlling the ESP8266 microcontroller, reading data from the EMG sensor, and triggering the TENS device.
5. README.md: This README file.



**Prerequisites**
Python: Install Python (version 3.7 or higher).
Required Libraries: pip install pandas scikit-learn matplotlib
Arduino IDE: Required for uploading the tens.ino code to the ESP8266 microcontroller.

**How to Run the Project**
**Step 1:** Set Up Hardware
Connect the EMG sensor to the ESP8266 microcontroller.
Connect the output of the TENS device to the ESP8266.
Make sure the device's power supply is stable and adequate for continuous data collection and TENS operation.

**Step 2:** Upload the Code to ESP8266
Open the Arduino IDE.
Open the tens.ino file.
Configure the board settings for ESP8266 and upload the code to the microcontroller.

**Step 3:** Data Collection and Model Training
Run the Dataset_creation_and_model_training.ipynb Jupyter notebook to:
Load and preprocess the dataset.
Train machine learning models for cramping detection and pain level prediction.
This notebook uses Random Forest and other classifiers to predict status (cramping or not) and pain_level.

**Step 4:** Real-time Prediction
Stream real-time EMG data from the ESP8266 to your Python environment (e.g., via serial communication).
Use the trained models to predict status and pain_level based on incoming data.
Adjust the TENS device output according to the predictions, delivering customized therapy.


**Machine Learning Details**
The machine learning component in this project serves two main purposes: Cramping Detection and Pain Level Prediction. The approach uses both Supervised Learning (Random Forest) and Reinforcement Learning for personalization and dynamic adjustments of TENS therapy.

**Supervised Learning (Random Forest)**
Dataset: The EMG data in Dataset_TENS_with_status.csv includes features like RMS, Mean Frequency, Variance, Amplitude, and status.
Model Training: In the Dataset_creation_and_model_training.ipynb notebook, we preprocess the data and train supervised models using the Random Forest algorithm to predict:
Cramping Detection (status): Determines whether the user is experiencing cramping (1 for cramping, 0 for not cramping).
Pain Level Prediction (pain_level): Predicts the pain level based on EMG features.
Evaluation: Accuracy, precision, recall, and F1-score are calculated to assess the model’s performance.

**Key Models**
Cramping Detection (status prediction): A Random Forest model trained to detect if the user is experiencing cramping.
Pain Level Prediction (pain_level prediction): A Random Forest model trained to predict pain levels based on EMG features, allowing the TENS device to adjust output intensity dynamically.

**Reinforcement Learning for Real-time Adaptation**
To further personalize and adapt the TENS therapy over time, Reinforcement Learning (RL) is incorporated. This approach allows the device to learn from user feedback and adjust its settings to achieve optimal pain relief.

**Overview of Reinforcement Learning Setup**
Agent: The TENS device controller that selects TENS intensity and frequency levels.
Environment: The user's response to the TENS therapy, represented by EMG sensor readings and feedback on pain relief.
Actions: Adjustments in TENS intensity and frequency.
Reward: Positive feedback if pain relief is achieved without causing discomfort, negative feedback otherwise.


**Reinforcement Learning Process**
Feedback Loop: The device starts with default settings and, based on user feedback, learns to adjust intensity and frequency. User feedback can be gathered manually (e.g., pain ratings) or inferred from EMG responses.
Model: A simple RL algorithm (like Q-learning or a Deep Q-Network) can be implemented to find optimal TENS settings. The agent iteratively improves its policy to deliver effective pain relief with minimal discomfort.
Personalization: As the agent interacts with the user, it learns their unique pain threshold and cramping patterns, tailoring the TENS therapy to provide individualized pain relief.
Implementation in Dataset_creation_and_model_training.ipynb

**The Jupyter notebook contains code for initializing and training the RL model using a reward-based system. Key components include:**
State Representation: EMG readings combined with pain feedback create the state space.
Action Selection: The agent explores different TENS settings and gradually learns to select optimal settings based on accumulated rewards.
Reward Shaping: Rewards are designed to encourage effective pain relief and discourage settings that cause discomfort.
Benefits of Reinforcement Learning in SWASTHI-TENS
Continuous Adaptation: The device adjusts to the user's evolving pain levels and tolerance.
Improved Pain Management: RL fine-tunes the therapy over time, achieving better pain relief with minimal intervention.
User Empowerment: By continuously learning, the device becomes more attuned to individual needs, providing a personalized pain relief solution.


**Hardware Setup**
ESP8266 Microcontroller: Programmed to read EMG data, trigger TENS pulses, and communicate with the Python environment for real-time analysis.
EMG Sensor: Captures muscle signals in real-time, which are processed for cramping detection and pain level prediction.
TENS Device: Provides electrical stimulation for pain relief, adjusted based on the machine learning model’s predictions.


**Hardware Requirements**
EMG Sensor: Capable of outputting reliable muscle activity data.
ESP8266: Handles data acquisition, control, and communication.
TENS Device: Configured to respond to varying intensity levels based on detected pain levels.
Acknowledgments
This project was developed to provide a customizable, machine-learning-driven solution for period pain relief, empowering women with better management options for menstrual discomfort. Special thanks to the contributors and all who supported the development of SWASTHI.



**Results:**
User Interface
![image](https://github.com/user-attachments/assets/f052c3f4-716b-41d2-ab43-7aa7f4e0dd82)


Machine learning model 
![result](https://github.com/user-attachments/assets/6470c9fd-4336-489c-9888-1c96fc902b39)

Working Video: 
https://youtu.be/eumGtb6jNPw?si=CdDGU12TB37VykUc

Presentation:
https://docs.google.com/presentation/d/1lsFhmmt5Vo_rLNfBoMJ8X6fjp1KpN6Zj/edit?usp=sharing&ouid=102105982643312255830&rtpof=true&sd=true

