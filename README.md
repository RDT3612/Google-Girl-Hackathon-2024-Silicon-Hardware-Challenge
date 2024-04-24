# Google-Girl-Hackathon-2024-Silicon-Hardware-Challenge
**README**

### Introduction
This code simulates a basic system architecture comprising a Central Processing Unit (CPU), an Input/Output (IO) device, a System Memory, and a Network-on-Chip (NoC) for communication between these components. The code demonstrates how requests for reading from or writing to memory are managed and processed, including simulating latency for memory access.

### Components
1. **SystemMemory**: Represents the main memory of the system. It provides methods for reading and writing data to memory with simulated latency.

2. **CPU**: Represents the central processing unit of the system. It can send read or write requests to memory and process pending requests.

3. **IO**: Represents an input/output device in the system. Similar to the CPU, it can send read or write requests to memory and process pending requests.

4. **NoC (Network-on-Chip)**: Simulates the communication network between different components in the system. It manages request queues for different types of agents (e.g., CPU, IO) and determines the next agent to process based on predefined weights. It also adjusts the operating frequency dynamically based on the utilization of request queues.

### Usage
1. **Compilation**: Compile the code using a C++ compiler, such as g++.
   ```
   g++ -std=c++11 main.cpp -o system_simulation
   ```

2. **Execution**: Run the compiled executable.
   ```
   ./system_simulation
   ```

### Customization
You can customize the following aspects of the simulation:
- **Buffer Sizes**: Adjust the buffer sizes for different agents in the NoC.
- **Arbiter Weights**: Set weights for different agents to prioritize their access to memory.
- **Operating Frequency**: Change the operating frequency of the NoC to simulate different system conditions.

### Notes
- The code includes comments for better understanding of each component and method.
- Memory access operations are simulated with predefined read and write latencies.
- Throttling mechanism in the NoC adjusts the operating frequency based on queue utilization.

### Example
The provided example in the `main()` function demonstrates how to use the NoC APIs to set buffer sizes and arbiter weights, send read and write requests from CPU and IO, process requests, and throttle the NoC based on queue utilization.

### Dependencies
- C++ Standard Library
- Compiler supporting C++11 standard or later

### License
This code is provided under the MIT License. See LICENSE file for details.

### Author
[Your Name/Your Organization]

### Contact
For any questions or issues, please contact [Your Email/Your Organization Email].
