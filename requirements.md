# 1. Application: Log Decoder

## 1.1. General description

Log decoder is a simple console application, that takes a .csv format logfile as an input and provides an output log file also in .csv format, with Payload decoded into meaningful values and additional flags if certains checks are violated for a given frame. The behavior of Log Decoder app is described by requirements collected in chapter 2. All of them should be implemented and fulfilled. For development purposes and example input log (input_log.csv) should be provided with this application description.

# 2. Log Decoder Requirements

This chapter collects all of the Log Decoder application requirements. Every requirement needs to be fulfilled, unless it is marked as *Comment* at the start of it's text. Then it is an additional piece of information.

## 2.1. Application overview

### 2.1.1. Log Decoder is a console application and shall be compilable to executable file with name *log_decoder*.

### 2.1.2. Log Decoder shall be run in the console with the following command (Windows Example): *log_decoder.exe input_log.csv output_log.csv*

### 2.1.3. When run without arguments, Log Decoder should provide help information how to use it.

## 2.2. Input detailed description

### 2.2.1. Log decoder shall accept a following format csv file as an input. Csv file is a collection of frames. Each frame is described by a single row in a file and has following columns: *ID, FrameNb, Timestamp, Payload, Checksum*. Definitions of these colums are as follows.

#### 2.2.1.1. ***ID*** - this is a frame category ID, defining what is held in payload [int] 

#### 2.2.1.2. ***FrameNb*** - this is a counter of frames for a given ID, every consecutive frame from a given ID should have this counter increased by one [int]

#### 2.2.1.3. ***Timestamp*** - this is a timestamp when the frame was received [int, ms]

#### 2.2.1.4. ***Payload*** - hex coded data in big endian format, how the data is coded depends on ID and is described in other requirements [hex]

#### 2.2.1.5. ***Checksum*** - sum complement checksum of four bytes in payload field, size of the checksum is one byte [hex]

### 2.2.2. Frame description: **ID = 15 Position Frame**

#### 2.2.2.1. This frame is used to send position data of the target object. First 16 bits of payload is a hex coded x_position data. Further 16 bits is y_position data

#### 2.2.2.2. **x_position**: Two digit precision number [0; 655,35] coded as uint16. To receive proper x_position value, divide the integer in payload by 100. Value is in [m]

#### 2.2.2.3. **y_position**: Three digit precision number [-32,767; 32,768] coded as uint16. To receive proper y_position value, add -32767 and divide the integer in payload by 1000. Value is in [m]

#### 2.2.2.4. Position frame shall be sent every 25ms [+/- 2ms]

### 2.2.3. Frame description: **ID = 78 Velocity Frame**

#### 2.2.3.1. This frame is used to send velocity data of the target object. First 16 bits of payload is a hex coded x_velocity data. Further 16 bits is y_velocity data

#### 2.2.3.2. **x_velocity**: Three digit precision number [-32,767; 32,768] coded as uint16. To receive proper x_velocity value, add -32767 and divide the integer in payload by 1000. Value is in [m/s]

#### 2.2.3.3. **y_velocity**: Three digit precision number [-32,767; 32,768] coded as uint16. To receive proper y_velocity value, add -32767 and divide the integer in payload by 1000. Value is in [m/s]

#### 2.2.3.4. Velocity frame shall be sent every 50ms [+/- 3ms]

## 2.3. Output detailed description

### 2.3.1. Log decoder shall output data as a .csv file with the name provided as a second argument when the application is run from command line.

#### 2.3.1.1. If the output file already exists, it should be overwritten. 

### 2.3.2. Output csv file shall be delimited with commas. 

### 2.3.3. Output csv file shall have following columns: *ID, FrameNb, Timestamp, PositionX, PositionY, VelocityX, VelocityY, ChecksumOK, TimeoutOK, FrameDrop*

#### 2.3.3.1. ***ID*** Id of the frame type copied from input data [int]

#### 2.3.3.2. ***FrameNb*** Frame count, copied from input data [int]

#### 2.3.3.3. ***Timestamp*** timestamp in [ms], copied from input data [int]

#### 2.3.3.4. ***PositionX*** decoded from payload, position of a target in x-axis [m], applicable only if the frame is a Position Frame, in other cases leave empty [float]

#### 2.3.3.5. ***PositionY*** decoded from payload, position of a target in y-axis [m], applicable only if the frame is a Position Frame, in other cases leave empty [float]

#### 2.3.3.6. ***VelocityX*** decoded from payload, velocity of a target in x-axis [m], applicable only if the frame is a Velocity Frame, in other cases leave empty [float]

#### 2.3.3.7. ***VelocityY*** decoded from payload, velocity of a target in y-axis [m], applicable only if the frame is a Velocity Frame, in other cases leave empty [float]

#### 2.3.3.8. ***ChecksumOK*** bool value describing if the frame passed checksum test [bool]

#### 2.3.3.9. ***TimeoutOK*** bool value describing if the frame passed timeout test [bool]

#### 2.3.3.10. ***FrameDropCnt*** int value describing how many frames were dropped (cumulative) [int]

## 2.4. Description of behavior

### 2.4.1. Log decoder (called also *Application*) shall decode input csv logfile and save the decoded data as a new csv file.

### 2.4.2. Application shall decode Payload data according to information provided in Chapter 2.2 Input detailed description and 2.3 Output detailed description.

### 2.4.3. Application shall check if the data within single frame (stored as a csv. file row) is valid and provide this info in ***ChecksumOK*** bool. (True if checksum is ok)

### 2.4.4. Validity of the data shall be checked by 8bit sum complement checksum on payload field.

### 2.4.5. Application shall check if the frame was sent with proper timing and provide this information in ***Timeout OK*** field.

### 2.4.6. *Comment* For the timing check, if the frame came within cycle time relative to previous frame of the same type, ***TimeoutOK*** shall be True, otherwise, False.

### 2.4.7. Application shall check if any frames were dropped (using FrameNb field). 

### 2.4.8. Dropped frames should for each frame type should be counted and provided as ***FrameDropCnt*** as a cumulative value.

## 2.5. Non-functional constraints

### 2.5.1. Application shall be written in C (C17).

### 2.5.2. Application shall be built without any errors and compiler warnings using gcc compiler with -Wall and -Wextra flags.

### 2.5.3. Application shall use only standard libraries.

### 2.5.4. Upon finishing the task, the application shall be delivered as a .zip archive with compilable source code. All files and information needed for successful build shall be included. It ***should not*** include compiled binaries.