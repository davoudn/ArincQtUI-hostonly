# Copilot Chat Conversation Export: Creating a developer's guide for ArincQtUI-host

**User:** @davoudn
**Thread URL:** https://github.com/copilot/c/5939811b-37cf-4904-9cb0-f70d52b23262

## @davoudn

please give me comprehensive developers guide for the ArincQtUi-hostonly repository!

## Copilot

Now I have comprehensive information about the repository. Let me compile this into a detailed developers guide.

## 🚀 Comprehensive Developers Guide for ArincQtUI-hostonly

### 📋 Project Overview

**Repository**: `davoudn/ArincQtUI-hostonly`  
**Language Composition**: C++ (90.3%), Python (7.9%), QMake (1.3%), Other (0.5%)  
**Created**: September 17, 2025  
**Last Updated**: December 8, 2025  
**License**: None specified  
**Repository Size**: ~83 MB

This is a Qt-based GUI application for ARINC 429 data handling. The project processes 8-byte ARINC 429 data packets where the first 4 bytes represent rate and ancillary data, and the second 4 bytes represent 32-bit ARINC 429 data. The application displays data in both **bit representation** and **engineering representation** formats.

---

### 🏗️ Project

*System: User retried message*

## Copilot

I'll load the repository code search ability and then gather comprehensive information about the ArincQtUI-hostonly repository to create a developer's guide for you.Now let me search for more comprehensive information about the project structure and key files:Now let me get more information about the project structure and build setup:Now let me gather more information about the data structures and key components:Now I have a comprehensive understanding of the repository. Let me create a detailed developer's guide.

---

# Comprehensive Developer's Guide: ArincQtUI-hostonly

## Overview

**ArincQtUI-hostonly** is a sophisticated Qt-based GUI application designed for ARINC 429 protocol implementation and management. ARINC 429 is an aviation industry standard for transmitting and receiving data over serial communication buses. This project provides both transmitter and receiver capabilities with a rich Qt user interface for configuration and monitoring.

### Key Metrics
- **Language Composition**: 90.3% C++, 7.9% Python, 1.3% QMake, 0.5% Other
- **Repository Size**: ~83 MB
- **Last Updated**: December 8, 2025
- **Build System**: Qt/QMake and CMake

---

## Project Architecture

### High-Level Structure

```
davoudn/ArincQtUI-hostonly/
├── menus/                          # Main application source code
│   ├── *.pro                       # QMake project file
│   ├── CMakeLists.txt              # CMake configuration
│   ├── mainwindow.h/cpp            # Main application window
│   ├── transmitter.h/cpp           # Transmitter UI component
│   ├── receiver.h/cpp              # Receiver UI component
│   ├── transmitterworker.h/cpp     # Transmitter worker thread
│   ├── receiverworker.h/cpp        # Receiver worker thread
│   ├── ArincData.h/cpp             # Core ARINC data structure
│   ├── arinc_functions.h/cpp       # ARINC data conversion functions
│   ├── Equipment.h/cpp             # Equipment management
│   ├── Label.h/cpp                 # ARINC label handling
│   ├── Handler.h                   # Abstract handler interface
│   ├── DEI1016.h/cpp               # DEI1016 device control
│   ├── DEI1016RasberryConfigurations.h  # Raspberry Pi GPIO configs
│   ├── Parameter*.h/cpp            # Parameter type handlers (BCD, BNR, etc.)
│   ├── types.h                     # Type definitions and constants
│   ├── utils.h/cpp                 # Utility functions
│   ├── tests/                      # Test files
│   ├── icons/                      # Icon resources
│   ├── equipment.json              # Equipment configuration
│   └── *.ui                        # Qt Designer UI files
├── CrossPlatformIconsExample.pro   # Icon theme project
├── mainwindows.pro                 # Root QMake project
└── README                          # Basic readme
```

---

## Core Components

### 1. **ARINC Data Structure** (`ArincData.h/cpp`)

The `DArincData` class is the foundation for all ARINC 429 data handling:

```cpp
class DArincData {
    std::bitset<ARINC32_SIZE> Data;  // 32-bit ARINC word
    
public:
    // Initialize with various types
    template <typename T>
    void Init(T& t);
    
    // Get/Set with type-safe templates
    template <typename T>
    value_t Get() const;
    
    template <typename T>
    void Set(const value_t& value);
    
    // Bit-level operations
    uint8_t GetBit(int n) const;
    void SetBit(uint32_t n, uint32_t val);
    void UpdateData(std::bitset<ARINC32_SIZE>&& _Data);
};
```

**Key Features:**
- Stores 32-bit ARINC word using `std::bitset`
- Supports multiple data representations (bits, hex, engineering units)
- Label field: bits 1-8
- SDI (Source/Destination Identifier): bits 9-10
- SSM (Sign/Status Matrix): bits 30-31

### 2. **ARINC Functions** (`arinc_functions.h/cpp`)

Comprehensive namespace containing data conversion calculators:

```cpp
namespace ArincFunctions {
    // Various data encoding/decoding classes
    class CALCULATOR { };      // General calculations
    class BCD { };             // Binary Coded Decimal
    class BNR { };             // Binary Number Representation
    class FLOATBCD { };        // Floating point BCD
    class FLNOBCD { };         // Flight Number BCD
    class ATCBCD { };          // ATC BCD
    class INT { };             // Integer representation
    class BITS { };            // Raw bit representation
    class HEX { };             // Hexadecimal representation
}
```

### 3. **Equipment & Label Management** (`Equipment.h/cpp`, `Label.h/cpp`)

**Equipment Class:**
- Represents a collection of ARINC labels
- Can be configured as Receiver or Transmitter
- Loaded from JSON configuration files
- Manages label parameters and conversions

**Label Class:**
- Represents a single ARINC label
- Contains parameters with specific encoding types
- Manages data value conversions (bits ↔ engineering units)
- Supports multiple parameter types (BCD, BNR, Discrete, etc.)

### 4. **Transmitter Component** (`transmitter.h/cpp`)

**Responsibility**: Manage ARINC transmission UI and control

```cpp
class transmitter : public QWidget {
    transmitter(QWidget *parent = nullptr, int ch = 0);
    
    // Equipment/Label management
    void FillEquipmentSelector();
    void FillLabelSelector();
    void selectEquipment(str_t eqId);
    
    // Label operations
    void addLabel(bool _ifChecked);
    void removeLabel(bool _ifChecked);
    
    // Configuration
    void onSelectSaveConfigFile(bool bIfClicked);
    void onSelectLoadConfigFile(bool bIfClicked);
    
    // Enable/Disable
    void onTransmitterEnabled(bool checked);
    void onTransmitterDisabled(bool checked);
    
    // Data access
    MyDataModel * getDataModel();
    std::vector<DArincData> getListOfAvailableLabelData();
};
```

**Features:**
- Dual-channel support (instance0, instance1)
- Tree view for equipment/label hierarchy
- Combobox for ARINC configuration (speed/parity)
- Custom delegate for inline editing
- Configuration save/load

### 5. **Receiver Component** (`receiver.h/cpp`)

**Responsibility**: Monitor incoming ARINC data

```cpp
class Receiver : public QWidget {
    void makeDeviceIndex();
    void onReceiverEnabled(bool);
    void onReceiverDisabled(bool);
    
    // Data monitoring
    MyDataModel *dataModel;
    EquipmentsIds* equipmentsIds;
};
```

**Features:**
- Multiple receiver instances (0, 1, 2, 3)
- Displays received labels with data rates
- Idle label cleaning
- Data rate evaluation

### 6. **Worker Threads** (`transmitterworker.h/cpp`, `receiverworker.h/cpp`)

**TransmitterWorker:**
- Runs in separate thread
- Manages transmission queue
- Handles DEI1016 device communication
- Executes transmission tasks at scheduled intervals

**ReceiverWorker:**
- Monitors serial data reception
- Updates UI with received labels
- Tracks data rates
- Cleans up idle labels

### 7. **DEI1016 Device Control** (`DEI1016.h/cpp`, `DEI1016RasberryConfigurations.h`)

Manages DEI1016 ARINC interface card:

```cpp
class DEI1016 : public QObject {
    static DEI1016* getInstance();  // Singleton
    
    // Control word configuration
    const word_t& setControlWord_transmitter_32Bits(int dei, int index);
    const word_t& setControlWord_receiver_32Bits(int dei, int index);
    
    // SDI configuration
    void select_enable_receiver_SDIChanell(int chanell, int ifEnable, int index, word_t& control_word);
};
```

**Configurations:**
- Supports 2 DEI1016 cards (NUM_DEI1016 = 2)
- GPIO pin mappings for Raspberry Pi
- Control word parameters (parity, data rate, SDI, etc.)

---

## Key Data Types

```cpp
// Type aliases (from types.h)
using str_t = QString;
using value_t = QVariant;
using word_t = std::bitset<WORDSIZE>;        // 16-bit
using dword_t = std::bitset<ARINC32_SIZE>;   // 32-bit

// ARINC Size constants
#define ARINC32_SIZE 32          // 32-bit ARINC word
#define LABEL_SIZE 8             // 8-bit label
#define WORDSIZE 16              // Control word size
#define ARINC32_DATA_LENGTH 24   // Data bits (8-31)

// Parameter types
enum DArincParamType {
    BNR = 0,        // Binary Number Representation
    BCD,            // Binary Coded Decimal
    FLOATBCD,       // Floating Point BCD
    DISCRETE,       // Discrete values
    FLNOBCD,        // Flight Number BCD
    ATCBCD,         // ATC BCD
    INT,            // Integer
    UNKOWN
};
```

---

## Building and Compilation

### Prerequisites
- **Qt Framework**: Qt 6.x (Core, Gui, Widgets, SerialPort)
- **C++ Standard**: C++17
- **Build Tools**: QMake or CMake
- **Optional**: pigpio library (for Raspberry Pi GPIO)

### Build Methods

#### Method 1: QMake (Recommended)
```bash
cd menus
qmake menus.pro
make
```

#### Method 2: CMake
```bash
cd menus
mkdir build && cd build
cmake ..
make
```

#### Method 3: Direct g++ Compilation
```bash
cd menus
./build.sh  # See build.sh for full command
```

### Compiler Flags
- Optimization: `-O3`
- C++ Standard: `-std=c++17`
- Libraries: `-lpigpio` (optional, for hardware interfacing)

---

## Main Application Flow

### Application Startup
```
main()
    ↓
MainWindow::MainWindow()
    ├── createActions()
    ├── createMenus()
    └── setupUI()
        ├── Transmitter 0 & 1 instances
        ├── Receiver 0, 1, 2, 3 instances
        ├── RecorderMenu
        └── SetupMenu (Database, Serial, etc.)
```

### Transmission Flow
```
User selects Equipment → fillEquipmentSelector()
    ↓
User selects Label → fillLabelSelector()
    ↓
User adds Label → addLabel()
    ↓
TransmitterWorker::transmitTask()
    ├── Get label data
    ├── Format ARINC word
    └── Send via DEI1016 interface
```

### Reception Flow
```
Hardware → SerialPort (DEI1016)
    ↓
DataSourceBase (ARINC bit stream)
    ↓
ReceiverWorker::receiveTask()
    ├── Parse ARINC data
    ├── Update DArincData
    └── Notify UI
        ↓
    Receiver UI updates
    Label data displayed
```

---

## File Organization by Responsibility

### Core Data Classes
- `ArincData.h/cpp` - ARINC 32-bit word container
- `types.h` - Global type definitions
- `arinc_functions.h/cpp` - Encoding/decoding algorithms

### Equipment/Label Management
- `Equipment.h/cpp` - Equipment collection management
- `Label.h/cpp` - Individual label management
- `BaseParameter.h/cpp` - Parameter base class
- `*Parameter.h/cpp` - Specific parameter types (BCD, BNR, etc.)

### UI Components
- `mainwindow.h/cpp` - Main application window
- `transmitter.h/cpp` - Transmitter UI
- `receiver.h/cpp` - Receiver UI
- `basetransiverwidget.h/cpp` - Base widget for transceiver UIs
- `*.ui` - Qt Designer UI files

### Worker Threads
- `transmitterworker.h/cpp` - Transmission thread
- `receiverworker.h/cpp` - Reception thread
- `Timer.h/cpp` - Timer utility for thread scheduling

### Hardware Interface
- `DEI1016.h/cpp` - Device control
- `DEI1016RasberryConfigurations.h` - GPIO configurations
- `DataSourceBase.h/cpp` - Data source abstraction
- `Handler.h` - Handler interface

### UI/Data Models
- `TreeDataModel.h/cpp` - Equipment/label tree model
- `editordelegate.h/cpp` - Custom editor for inline editing
- `checkboxarray.h/cpp` - Checkbox array widget

### Utilities
- `utils.h/cpp` - File I/O, JSON, file dialogs
- `bitutils.h` - Bit manipulation utilities
- `generaldata.h/cpp` - Global application data
- `PointerVector.h` - Custom vector for managing pointers

### Configuration
- `equipment.json` - Equipment and label definitions

---

## JSON Configuration Format

### Equipment Configuration (`equipment.json`)

Equipment and label definitions are loaded from JSON. Structure example:

```json
{
  "equipmentId": "002",
  "equipmentName": "INS",
  "labels": [
    {
      "labelId": "310",
      "labelName": "Aircraft Position",
      "parameters": [
        {
          "name": "Latitude",
          "type": "BNR",
          "startBit": 9,
          "endBit": 29,
          "scale": 0.001
        }
      ]
    }
  ]
}
```

---

## Threading Model

### Main Thread
- Qt event loop
- UI event handling
- Menu actions

### Transmitter Worker Thread(s)
- Scheduled transmission tasks
- DEI1016 write operations
- Timer-based data transmission

### Receiver Worker Thread(s)
- Serial port reading
- Data parsing
- Label idle detection
- Data rate calculation

**Thread Safety:**
- `QMutex` for shared data access
- `QThread` for thread management
- `QtConcurrent` for parallel tasks

---

## Development Guidelines

### Adding a New Parameter Type

1. Create new class in `arinc_functions.h`:
```cpp
class MYNEWTYPE {
    template <typename T>
    static T Value(/* parameters */, const DArincData& Data);
    
    template <typename T>
    static void SetValue(const T& t, /* parameters */, DArincData& Data);
};
```

2. Implement in `arinc_functions.cpp`:
```cpp
template<>
value_t MYNEWTYPE::Value<value_t>(/* */) {
    // Conversion logic
}
```

3. Add to `DArincParamType` enum in `types.h`

4. Create parameter class (e.g., `MyNewTypeParameter.h/cpp`)

### Adding a New Equipment

1. Define in `equipment.json`
2. Equipment automatically loads on startup
3. Appears in UI equipment selector

### Extending UI

1. Create new `.ui` file in Qt Designer
2. Add corresponding `.h` and `.cpp` files
3. Include in `menus.pro` (FORMS section)
4. Integrate into `mainwindow.cpp`

---

## Performance Considerations

- **Real-time Constraints**: Receiver worker handles incoming data with minimal latency
- **Data Rate Limits**: 
  - HI rate: ~12.5 kHz (clock/10)
  - LO rate: ~1.5625 kHz (clock/80)
- **Optimization Flags**: `-O3` compiler optimization enabled
- **Thread Pooling**: Qt Concurrent used for parallel task execution
- **Memory Management**: Smart pointers via `PointerVector` for automatic cleanup

---

## Debugging Tips

### Enable Debug Output
- Use `qDebug()` for console logging
- Check `receivers/transmitters` worker threads
- Monitor data reception in `ReceiverWorker::receiveTask()`

### Common Issues
1. **No Data Reception**: Check DEI1016 device configuration and GPIO pins
2. **Incorrect Values**: Verify parameter encoding type (BCD, BNR, etc.)
3. **Performance**: Check thread workload and data rate settings
4. **UI Unresponsiveness**: Ensure heavy operations run in worker threads

---

## Key Dependencies

- **Qt Framework** (>= 6.0): Core, Gui, Widgets, SerialPort modules
- **pigpio**: GPIO control for Raspberry Pi (optional)
- **Standard C++ Library**: `<bitset>`, `<vector>`, `<map>`, `<array>`
- **Third-party**: json.h/cpp for JSON parsing (included)

---

## Testing

Test files available in `menus/tests/`:
- `test_isr.cpp` - Interrupt service routine testing
- `bittest.cpp` - Bitset operations testing
- `build.sh` - Build script for tests

---

## Resources & Standards

- **ARINC 429 Specification**: Aviation data bus standard (Part 1, Page 110 referenced)
- **DEI1016 Datasheet**: ARINC interface card specifications
- **Qt Documentation**: Official Qt Framework docs for GUI development
- **pigpio Library**: Raspberry Pi GPIO Python library docs

---

This guide provides a solid foundation for understanding and developing within the ArincQtUI-hostonly project. Start with the main application window, understand the transmitter/receiver architecture, and familiarize yourself with the ARINC data structures before making modifications.

*System: User retried message*

## @davoudn

please give it to me as a single file.

## Copilot

# Comprehensive Developer's Guide: ArincQtUI-hostonly

## Overview

**ArincQtUI-hostonly** is a sophisticated Qt-based GUI application designed for ARINC 429 protocol implementation and management. ARINC 429 is an aviation industry standard for transmitting and receiving data over serial communication buses. This project provides both transmitter and receiver capabilities with a rich Qt user interface for configuration and monitoring.

### Key Metrics
- **Language Composition**: 90.3% C++, 7.9% Python, 1.3% QMake, 0.5% Other
- **Repository Size**: ~83 MB
- **Last Updated**: December 8, 2025
- **Build System**: Qt/QMake and CMake

---

## Project Architecture

### High-Level Structure

```
davoudn/ArincQtUI-hostonly/
├── menus/                          # Main application source code
│   ├── *.pro                       # QMake project file
│   ├── CMakeLists.txt              # CMake configuration
│   ├── mainwindow.h/cpp            # Main application window
│   ├── transmitter.h/cpp           # Transmitter UI component
│   ├── receiver.h/cpp              # Receiver UI component
│   ├── transmitterworker.h/cpp     # Transmitter worker thread
│   ├── receiverworker.h/cpp        # Receiver worker thread
│   ├── ArincData.h/cpp             # Core ARINC data structure
│   ├── arinc_functions.h/cpp       # ARINC data conversion functions
│   ├── Equipment.h/cpp             # Equipment management
│   ├── Label.h/cpp                 # ARINC label handling
│   ├── Handler.h                   # Abstract handler interface
│   ├── DEI1016.h/cpp               # DEI1016 device control
│   ├── DEI1016RasberryConfigurations.h  # Raspberry Pi GPIO configs
│   ├── Parameter*.h/cpp            # Parameter type handlers (BCD, BNR, etc.)
│   ├── types.h                     # Type definitions and constants
│   ├── utils.h/cpp                 # Utility functions
│   ├── tests/                      # Test files
│   ├── icons/                      # Icon resources
│   ├── equipment.json              # Equipment configuration
│   └── *.ui                        # Qt Designer UI files
├── CrossPlatformIconsExample.pro   # Icon theme project
├── mainwindows.pro                 # Root QMake project
└── README                          # Basic readme
```

---

## Core Components

### 1. **ARINC Data Structure** (`ArincData.h/cpp`)

The `DArincData` class is the foundation for all ARINC 429 data handling:

```cpp
class DArincData {
    std::bitset<ARINC32_SIZE> Data;  // 32-bit ARINC word
    
public:
    // Initialize with various types
    template <typename T>
    void Init(T& t);
    
    // Get/Set with type-safe templates
    template <typename T>
    value_t Get() const;
    
    template <typename T>
    void Set(const value_t& value);
    
    // Bit-level operations
    uint8_t GetBit(int n) const;
    void SetBit(uint32_t n, uint32_t val);
    void UpdateData(std::bitset<ARINC32_SIZE>&& _Data);
};
```

**Key Features:**
- Stores 32-bit ARINC word using `std::bitset`
- Supports multiple data representations (bits, hex, engineering units)
- Label field: bits 1-8
- SDI (Source/Destination Identifier): bits 9-10
- SSM (Sign/Status Matrix): bits 30-31

**ARINC 429 Word Structure:**
```
Bit Position:  1-8         9-10      11-29          30-31
Field:        Label       SDI       Data/Params     SSM
Size:         8 bits      2 bits    19 bits        2 bits
```

### 2. **ARINC Functions** (`arinc_functions.h/cpp`)

Comprehensive namespace containing data conversion calculators:

```cpp
namespace ArincFunctions {
    // Various data encoding/decoding classes
    class CALCULATOR { };      // General calculations
    class BCD { };             // Binary Coded Decimal
    class BNR { };             // Binary Number Representation
    class FLOATBCD { };        // Floating point BCD
    class FLNOBCD { };         // Flight Number BCD
    class ATCBCD { };          // ATC BCD
    class INT { };             // Integer representation
    class BITS { };            // Raw bit representation
    class HEX { };             // Hexadecimal representation
    class IEEEFLOT { };        // IEEE floating point
}
```

**Key Functionality:**
- Data extraction from ARINC word by bit position
- Conversion between raw bits and engineering values
- Support for different scales and precisions
- Proper handling of sign/status bits
- Two's complement support for negative values

**Usage Example:**
```cpp
DArincData arincData;
// Extract BCD value from bits 11-29
value_t bcdValue = ArincFunctions::BCD::Value<value_t>(5, arincData);

// Set BCD value
ArincFunctions::BCD::SetValue<value_t>(value, 5, arincData);

// Extract BNR value
value_t bnrValue = ArincFunctions::BNR::Value<value_t>(9, 29, arincData);
```

### 3. **Equipment & Label Management** (`Equipment.h/cpp`, `Label.h/cpp`)

**Equipment Class:**
Represents a collection of ARINC labels grouped by equipment type (e.g., INS, VOR, DME, etc.)

```cpp
class Equipment: public BaseItem {
    Equipment(str_t equipmentID, EquipmentRole _role);
    
    // Equipment operations
    Label* findLabel(str_t _labelid);
    str_t readName(str_t _equimentId);
    bool addLabel(str_t _labelid);
    void removeLabel(str_t _labelid);
    
    // Data access
    PointerVector<BaseItem>& GetLabels();
    const std::map<str_t,str_t>& getLabelsIdAndNames();
    str_t getEqipmentId();
    str_t getEqipmentIdAndName();
};
```

**Features:**
- Role-based (Receiver or Transmitter)
- JSON configuration file loading
- Label hierarchy management
- Name mapping (ID to human-readable names)

**Label Class:**
Represents a single ARINC label with its parameters

```cpp
class Label: public BaseItem {
    str_t LabelId;              // ARINC label ID (e.g., "310")
    str_t Name;                 // Human-readable name
    str_t Type;                 // Label type (BNR, BCD, etc.)
    DArincData ArincData;       // 32-bit ARINC word
    
    // Parameter management
    void addParams(jsonobj_t _jsonObj);
    const PointerVector<BaseItem>& getParameters();
    
    // Value operations
    value_t valueBits(str_t pname, DArincData& data);
    value_t valueEng(str_t pname, DArincData& data);
    void setValueEng(str_t _pname, value_t _value);
};
```

**Features:**
- Contains multiple parameters (altitude, speed, position, etc.)
- Supports multiple parameter encoding types
- Value conversion (bits ↔ engineering units)
- Idle label tracking (cleanup of unused labels)

### 4. **Parameter Types** (`BaseParameter.h/cpp`, `*Parameter.h/cpp`)

**Base Parameter Class:**
```cpp
class BaseParameter: public BaseItem {
    str_t Name;
    uint8_t IBit, FBit;    // Initial and Final bit positions
    
    // Value operations
    virtual value_t EngValue(DArincData& data) = 0;
    virtual value_t BitsValue(DArincData& data) = 0;
    virtual void setEngValue(value_t value, DArincData& data) = 0;
};
```

**Specialized Parameter Types:**
1. **BNRParameter**: Binary Number Representation (floating point)
2. **BCDParameter**: Binary Coded Decimal (5 significant digits)
3. **DiscreteParameter**: Discrete on/off values
4. **FLNOParameter**: Flight Number (4-digit BCD)
5. **ATCBCDParameter**: ATC Altitude BCD (3 bits/digit)
6. **INTParameter**: Integer values
7. **FLOATBCDParameter**: Floating point BCD for radio frequencies

### 5. **Transmitter Component** (`transmitter.h/cpp`)

**Responsibility**: Manage ARINC transmission UI and control

```cpp
class transmitter : public QWidget {
    Q_OBJECT
    
    transmitter(QWidget *parent = nullptr, int ch = 0);
    ~transmitter();
    
    // Singleton pattern
    static transmitter* getInstance(int ch);
    
    // Equipment/Label management
    void FillEquipmentSelector();
    void FillLabelSelector();
    void selectEquipment(str_t eqId);
    void OnEquipmentSelectorChanged(int _selectorindexd);
    void OnLabelSelectorChanged(int _selectorindexd);
    
    // Label operations
    void addLabel(bool _ifChecked);
    void removeLabel(bool _ifChecked);
    
    // Configuration
    void onSelectSaveConfigFile(bool bIfClicked);
    void onSelectLoadConfigFile(bool bIfClicked);
    void onLoadConfig(bool bIfClicked);
    
    // Enable/Disable
    void onTransmitterEnabled(bool checked);
    void onTransmitterDisabled(bool checked);
    void Disable();
    void Enable();
    
    // Data rate management
    void onArinc_parity_bitRate(int index);
    void incrementLabelsDataRateCounter();
    
    // Data access
    MyDataModel * getDataModel();
    std::vector<DArincData> getListOfAvailableLabelData();
    
private:
    static transmitter* instance0;      // Channel 0
    static transmitter* instance1;      // Channel 1
    int chanell;                        // Channel number
    int dei, deiChanell;                // Device indices
    
    Ui::transmitter *ui;
    MyDataModel *dataModel;
    EquipmentsIds* equipmentsIds;
    EditorDelegate* editorDelegate;
    
    QMutex modelMutex;
    bool bIfEnabled;
};
```

**Features:**
- Dual-channel support via singleton pattern
- Tree view UI for equipment/label hierarchy
- Combobox for ARINC configuration (12.5 kbps/100 kbps, odd/even parity)
- Custom inline editor for parameter values
- Configuration persistence (save/load JSON)
- Data rate tracking for each label

**UI Layout:**
- Equipment selector dropdown
- Label selector dropdown
- Tree view showing labels and parameters
- Buttons: Add Label, Remove Label, Save Config, Load Config
- Checkboxes: Enable/Disable transmitter

### 6. **Receiver Component** (`receiver.h/cpp`)

**Responsibility**: Monitor and display incoming ARINC data

```cpp
class Receiver : public QWidget {
    Q_OBJECT
    
    Receiver(QWidget *parent = nullptr, int ch = 0);
    ~Receiver();
    
    // Singleton pattern
    static Receiver* getInstance(int ch);
    
    // Data operations
    void setLabelData(str_t labelid, float rate, value_t value);
    void idleLabelCleaner();
    void evalDataRates(uint32_t evalTime);
    
    // Device management
    void makeDeviceIndex();
    
    // UI state
    void onReceiverEnabled(bool);
    void onReceiverDisabled(bool);
    
private:
    static Receiver* instance0;         // Channel 0
    static Receiver* instance1;         // Channel 1
    static Receiver* instance2;         // Channel 2
    static Receiver* instance3;         // Channel 3
    int chanell;                        // Channel number
    int dei, deiChanell;                // Device indices
    
    Ui::Receiver *ui;
    MyDataModel *dataModel;
    EquipmentsIds* equipmentsIds;
    QMutex modelMutex;
    bool bIfEnabled;
};
```

**Features:**
- Supports up to 4 receiver channels
- Real-time label data display
- Data rate calculation (frames per second)
- Idle label cleanup (removes labels not updated within threshold)
- Equipment selector for data interpretation

### 7. **Worker Threads** (`transmitterworker.h/cpp`, `receiverworker.h/cpp`)

**TransmitterWorker:**
Manages transmission scheduling and device communication

```cpp
class TransmitterWorker: public QObject {
    Q_OBJECT
    
    TransmitterWorker(str_t _equipment, int ch);
    ~TransmitterWorker();
    
    static TransmitterWorker* getInstance(int ch);
    
    // Equipment access
    Equipment* getEquipment();
    PointerVector<BaseItem>& getEquipments();
    
    // Task management
    void startTasks();
    
public slots:
    void taskTransmitData();
    void incrementLabelsDataRateCounter();
    void actionListCleaner();
    
signals:
    bool sendData(BaseAction* ac);
    
private:
    static TransmitterWorker* instance0;    // Channel 0
    static TransmitterWorker* instance1;    // Channel 1
    
    int chanell;
    PointerVector<BaseItem> equipments;
    DArincData* ArincData;
    
    QThread* mainThread;
    QThread* dataRateThread;
    Timer* dataRateTimer;
    
    bool bIfDataUpdated;
    bool bIfDataHandled;
};
```

**Execution Model:**
1. Runs in separate QThread
2. Timer-based task scheduling (TRANSMITTER_INTERVAL_TIME = 2ms)
3. Retrieves available labels from Equipment
4. Formats ARINC words
5. Sends to DEI1016 device via SerialPort
6. Tracks data rates per label

**ReceiverWorker:**
Monitors serial port for incoming ARINC data

```cpp
class ReceiverWorker: public QObject {
    Q_OBJECT
    
    ReceiverWorker(str_t _equipment, uint8_t ch);
    ~ReceiverWorker();
    
    static ReceiverWorker* getInstance(uint8_t ch);
    
    // Equipment access
    Equipment* getEquipment();
    PointerVector<BaseItem>& getEquipments();
    DArincData& getArincData();
    
    // Task management
    void startTasks();
    void makeDeviceIndex();
    
public slots:
    void receiveTask();
    void idleLabelCleaner();
    void evalDataRates();
    void update(uint8_t& dei, uint8_t& chanell, float& _rate, std::bitset<ARINC32_SIZE>& arincBitsData);
    
private:
    static ReceiverWorker* instance0;       // Channel 0
    static ReceiverWorker* instance1;       // Channel 1
    static ReceiverWorker* instance2;       // Channel 2
    static ReceiverWorker* instance3;       // Channel 3
    
    uint8_t chanell;
    uint8_t dei, deiChanell;
    
    PointerVector<BaseItem> equipments;
    DArincData* ArincData;
    
    QThread* mainThread;
    QThread* idlecleanerThread;
    QThread* dataRateThread;
    
    Timer* idleLabelCleanerTimer;
    Timer* dataRateTimer;
    
    float rate;
    bool bIfDataUpdated;
    bool bIfDataHandled;
    bool bIfToClean;
};
```

**Execution Model:**
1. Runs in separate QThread
2. Monitors DEI1016 serial data reception
3. Parses received 32-bit ARINC words
4. Updates DArincData with received values
5. Notifies Receiver UI component
6. Periodically cleans idle labels (not updated within timeout)
7. Calculates data rates

### 8. **DEI1016 Device Control** (`DEI1016.h/cpp`, `DEI1016RasberryConfigurations.h`)

**DEI1016 Class:**
Manages ARINC interface card (DEI1016 hardware)

```cpp
class DEI1016 : public QObject {
    Q_OBJECT
    
    explicit DEI1016(QObject* parent = nullptr);
    static DEI1016* getInstance();
    
    // Control word configuration
    const word_t& setControlWord_transmitter_32Bits(int dei, int index);
    const word_t& setControlWord_receiver_32Bits(int dei, int index);
    void setControlWord_transmitter_32Bits(int chanell, int index, word_t& control_word);
    void setControlWord_receiver_32Bits(int receiveChanell, int index, word_t& control_word);
    
    // SDI (Source/Destination Identifier) configuration
    void select_enable_receiver_SDIChanell(int chanell, int ifEnable, int index, word_t& control_word);
    
    // Serial port management
    void openSerialPort();
    void setControlInstruction(uint8_t instruction);
    void createMaps();
    void updateTask();
    
public:
    bool bIfSerialOpen;
    bool bIfDataReceived;
    
public slots:
    void dataReceived();
    
signals:
    void update(uint8_t dei, uint8_t chanell, float rate, std::bitset<ARINC32_SIZE> data);
    
private:
    static DEI1016* instance;
    std::vector<word_t> controlWords;   // Control words per device
    QSerialPort* serialPort;
};
```

**Control Word Structure:**
```cpp
struct ControlWord {
    uint16_t reserved : 4;              // Bits 0-3: Reserved
    uint16_t paren : 1;                 // Bit 4: Parity Enable
    uint16_t selfTest : 1;              // Bit 5: Self Test
    uint16_t sdi_enb1 : 1;              // Bit 6: SDI Enable Channel 1
    uint16_t sdi_x1 : 1;                // Bit 7: SDI X1 Channel 1
    uint16_t sdi_y1 : 1;                // Bit 8: SDI Y1 Channel 1
    uint16_t sdi_enb2 : 1;              // Bit 9: SDI Enable Channel 2
    uint16_t sdi_x2 : 1;                // Bit 10: SDI X2 Channel 2
    uint16_t sdi_y2 : 1;                // Bit 11: SDI Y2 Channel 2
    uint16_t parck : 1;                 // Bit 12: Parity Check (Odd/Even)
    uint16_t txsel3 : 1;                // Bit 13: TX Data Rate (HI/LO)
    uint16_t rcvsel4 : 1;               // Bit 14: RX Data Rate (HI/LO)
    uint16_t wordLength : 1;            // Bit 15: Word Length (32/25 bits)
};
```

**GPIO Configuration (Raspberry Pi):**
```cpp
// Data pins (D0-D15)
#define D0  17      #define D8  12
#define D1  18      #define D9  13
#define D2  27      #define D10 19
#define D3  22      #define D11 16
#define D4  23      #define D12 26
#define D5  24      #define D13 20
#define D6  25      #define D14 21
#define D7  8       #define D15 9

// Control pins
#define DR1  11     // Data Ready Channel 1 (Input)
#define DR2  5      // Data Ready Channel 2 (Input)
#define TXR  6      // Transmitter Ready (Input)
#define SEL  7      // Select (Output)
#define OE1  4      // Output Enable Channel 1 (Output)
#define OE2  2      // Output Enable Channel 2 (Output)
#define LD1  3      // Load Channel 1 (Output)
#define LD2  10     // Load Channel 2 (Output)
#define ENTX 14     // Enable TX (Output)
#define LDCW 15     // Load Control Word (Output)
```

**Data Rates:**
- HI: 100 kbps (clock/10)
- LO: 12.5 kbps (clock/80)

---

## Key Data Types and Constants

```cpp
// Type aliases (from types.h)
using str_t = QString;
using value_t = QVariant;
using word_t = std::bitset<WORDSIZE>;        // 16-bit control word
using dword_t = std::bitset<ARINC32_SIZE>;   // 32-bit ARINC word
using jsonobj_t = QJsonObject;
using jsondoc_t = QJsonDocument;
using jsonarray_t = QJsonArray;

// Size constants
#define ARINC32_SIZE 32              // 32-bit ARINC word
#define LABEL_SIZE 8                 // 8-bit label field
#define WORDSIZE 16                  // 16-bit control word
#define NUM_WORDS_PER_DATA 2         // 2 words = 1 ARINC data
#define ARINC32_DATA_LENGTH 24       // 24 data bits (8-31)

// Bit positions
#define SDI_LSB 9                    // SDI least significant bit
#define SDI_MSB 10                   // SDI most significant bit
#define SSM_LSB 30                   // SSM least significant bit
#define SSM_MSB 31                   // SSM most significant bit
#define BCDBNR_MSB 29                // Most significant data bit
#define LABEL_SIZE 8                 // Label width in bits

// Timing constants
#define CLEANING_CHECK_TIME 1000             // ms - Label cleanup interval
#define DATA_RATE_EVAL_TIME 10000            // ms - Data rate evaluation interval
#define TRANSMITTER_INTERVAL_TIME 2          // ms - TX task interval
#define IDLE_THRESHOLD 2000                  // ms - Label idle timeout
#define ACTION_CLEANER_TIME 2000             // ms - Action queue cleanup
#define MAX_TIME 10000                       // ms - Maximum timer value
#define MIN_TICK 1000                        // µs - Minimum thread delay

// Serial communication
#define RECEIVE_PACKET_SIZE 8                // bytes
#define TRANSMMIT_PACKET_SIZE 8              // bytes
#define WAIT_FOR_SERIAL_READ_READY 1000      // ms

// Parameter types
enum DArincParamType {
    BNR = 0,        // Binary Number Representation (floating point)
    BCD,            // Binary Coded Decimal
    FLOATBCD,       // Floating Point BCD
    DISCRETE,       // Discrete on/off values
    FLNOBCD,        // Flight Number BCD (4 digits)
    ATCBCD,         // ATC Altitude BCD (3 bits/digit)
    INT,            // Integer values
    UNKOWN
};

// Equipment roles
enum class EquipmentRole {
    Receiver,
    Transmitter
};

// Predefined receiver configurations
enum PREDEFINED_RECEIVER {
    SLOW_SDI_DISABLED,
    FAST_SDI_DISABLED,
    SLOW_SDI0, SLOW_SDI1, SLOW_SDI2, SLOW_SDI3,
    FAST_SDI0, FAST_SDI1, FAST_SDI2, FAST_SDI3
};

// Predefined transmitter configurations
enum PREDEFINED_TRANSMITTER {
    SLOW_ODD_PARITY,
    FAST_ODD_PARITY,
    SLOW_EVEN_PARITY,
    FAST_EVEN_PARITY
};

// Device and channel identifiers
#define NUM_DEI1016 2                        // Two DEI1016 cards supported
#define CHANELL0 0
#define CHANELL1 1
#define CHANELL2 2
#define CHANELL3 3

#define SET_ENABLE 1
#define SET_DISABLE 0
```

---

## Building and Compilation

### Prerequisites
- **Qt Framework**: Qt 6.x (Core, Gui, Widgets, SerialPort modules)
- **C++ Compiler**: GCC 9+ or Clang 10+
- **C++ Standard**: C++17 minimum
- **Build Tools**: QMake or CMake
- **Optional**: pigpio library (for Raspberry Pi GPIO control)

### Build Methods

#### Method 1: QMake (Recommended for Qt Creator)
```bash
cd menus
qmake menus.pro
make -j4
```

**Qt Creator Steps:**
1. File → Open File or Project
2. Select `menus/menus.pro`
3. Click Configure Project
4. Build → Build Project

#### Method 2: CMake
```bash
cd menus
mkdir build
cd build
cmake ..
make -j4
```

**CMakeLists.txt Configuration:**
```cmake
cmake_minimum_required(VERSION 3.16)
project(menus LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS 
    Core 
    Gui 
    Widgets 
    SerialPort
)

qt_standard_project_setup()

qt_add_executable(menus
    # List of source files
)

target_link_libraries(menus PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::SerialPort
)
```

#### Method 3: Direct g++ Compilation
```bash
cd menus
g++ -std=c++17 -O3 \
    main.cpp \
    DEI1016.cpp \
    Handler.cpp \
    DataSourceBase.cpp \
    Equipment.cpp \
    Label.cpp \
    DiscreteParameter.cpp \
    BCDParameter.cpp \
    BNRParameter.cpp \
    BaseParameter.cpp \
    ArincData.cpp \
    arinc_functions.cpp \
    transmitter.cpp \
    receiver.cpp \
    transmitterworker.cpp \
    receiverworker.cpp \
    -lQt6Core -lQt6Gui -lQt6Widgets -lQt6SerialPort \
    -lpigpio \
    -o arinc_app
```

### Compiler Flags and Options

```makefile
# From menus.pro
QMAKE_CXXFLAGS += -O3                    # Optimization level 3
CONFIG += c++17                          # C++ standard

# Qt modules
QT += core gui widgets serialport

# Platform-specific
unix: LIBS += -lpigpio                   # Raspberry Pi GPIO library
```

### Running the Application

```bash
# From Qt Creator
Build → Run (Ctrl+R)

# From command line
cd menus
./menus

# With debug output
./menus -platform offscreen  # Headless mode for testing
```

---

## Main Application Flow

### Application Startup Sequence

```
main()
    │
    ├─→ QApplication app(argc, argv)
    │
    ├─→ MainWindow window
    │   │
    │   ├─→ MainWindow::MainWindow()
    │   │   │
    │   │   ├─→ setupUi(this)
    │   │   │   ├─→ menuBar creation
    │   │   │   └─→ statusBar setup
    │   │   │
    │   │   ├─→ createActions()
    │   │   │   ├─→ transmitterAction (ch0, ch1)
    │   │   │   ├─→ receiverAction (ch0-3)
    │   │   │   ├─→ recorderActions (record, playback, import, export)
    │   │   │   └─→ setupActions (database, serial, etc.)
    │   │   │
    │   │   └─→ createMenus()
    │   │       ├─→ Transmitter menu
    │   │       ├─→ Receiver menu
    │   │       ├─→ Recorder menu
    │   │       ├─→ Acquisition menu
    │   │       ├─→ Setup menu
    │   │       └─→ Help menu
    │   │
    │   └─→ Initialize singleton workers
    │       ├─→ DEI1016::getInstance()
    │       ├─→ TransmitterWorker::getInstance(0, 1)
    │       └─→ ReceiverWorker::getInstance(0, 1, 2, 3)
    │
    └─→ window.show()
        └─→ app.exec()  // Event loop
```

### Transmitter Data Flow

```
User Interface Layer
    │
    ├─→ User selects Equipment (Transmitter)
    │   │
    │   └─→ transmitter::OnEquipmentSelectorChanged()
    │       ├─→ resetDataModel(equipmentId)
    │       └─→ FillLabelSelector()
    │
    ├─→ User selects Label
    │   │
    │   └─→ transmitter::OnLabelSelectorChanged()
    │       └─→ selectedLabelId = labelId
    │
    └─→ User adds Label (clicks Add button)
        │
        └─→ transmitter::addLabel()
            ├─→ Disable()  // Pause transmissions
            ├─→ dataModel->addLabel(selectedLabelId)
            ├─→ Enable()   // Resume transmissions
            └─→ emit dataModel->layoutChanged()

Data Model Layer
    │
    ├─→ MyDataModel manages tree of Equipment/Labels
    │   ├─→ Equipment
    │   │   └─→ Label1, Label2, Label3...
    │   │       └─→ Parameter1, Parameter2...
    │   │
    │   └─→ Provides data to QTreeView UI

Transmission Layer
    │
    ├─→ transmitterworker::taskTransmitData() [periodic, every 2ms]
    │   │
    │   ├─→ Get available labels from Equipment
    │   │   │
    │   │   └─→ transmitter::getListOfAvailableLabelData()
    │   │       │
    │   │       └─→ dataModel->getListOfAvailableLabelData()
    │   │
    │   ├─→ For each label:
    │   │   ├─→ Get Label ARINC data
    │   │   ├─→ For each parameter:
    │   │   │   ├─→ Get engineering value from UI
    │   │   │   ├─→ Convert to ARINC bits using appropriate encoder
    │   │   │   │   (BCD::SetValue, BNR::SetValue, etc.)
    │   │   │   └─→ Update DArincData
    │   │   │
    │   │   └─→ Prepare control word:
    │   │       ├─→ DEI1016::setControlWord_transmitter_32Bits()
    │   │       ├─→ Configure: data rate (HI/LO), parity (odd/even)
    │   │       └─→ Create BaseAction object
    │   │
    │   └─→ emit sendData(BaseAction)
    │
    ├─→ Hardware Layer (Raspberry Pi)
    │   │
    │   ├─→ DEI1016 device receives action
    │   │
    │   ├─→ Set GPIO pins for control word
    │   │   ├─→ Set D0-D15 data pins
    │   │   ├─→ Pulse LDCW (Load Control Word)
    │   │   └─→ Wait for TXR (Transmitter Ready)
    │   │
    │   └─→ Transmit ARINC data on serial bus

Data Rate Tracking
    │
    └─→ transmitterworker::incrementLabelsDataRateCounter()
        ├─→ Called periodically from timer
        └─→ Updates label->dataRateCounter (frames/sec)
```

### Receiver Data Flow

```
Hardware Layer (Serial Bus)
    │
    └─→ DEI1016 device monitors ARINC serial bus
        ├─→ Receives 32-bit ARINC word on serial line
        └─→ Reads via GPIO pins (D0-D15)

DEI1016 Device Driver
    │
    └─→ DEI1016::dataReceived()
        ├─→ Parse serial packet
        │   ├─→ Byte 0: Instruction
        │   ├─→ Byte 1: Rate
        │   ├─→ Bytes 2-3: Control word
        │   ├─→ Bytes 4-7: ARINC 32-bit data
        │   │
        │   └─→ Extract:
        │       ├─→ Label ID (bits 1-8)
        │       ├─→ ARINC data word (bits 1-31)
        │       └─→ Data rate
        │
        └─→ emit update(dei, chanell, rate, bitset_data)

Receiver Worker Thread
    │
    └─→ ReceiverWorker::update() [slot - queued connection]
        │
        ├─→ Store received data
        │   └─→ ArincData->UpdateData(bitset_data)
        │
        ├─→ Set flags
        │   ├─→ bIfDataUpdated = true
        │   └─→ bIfDataHandled = false
        │
        └─→ Wake up receiveTask()

Receiver Task (Running in QThread)
    │
    └─→ ReceiverWorker::receiveTask() [continuous loop]
        │
        ├─→ Wait for data (100µs sleep)
        │
        ├─→ Check bIfDataUpdated flag
        │   │
        │   ├─→ Extract label ID from ARINC data
        │   │   │
        │   │   └─→ labelid = ArincData.Get<LabelIdOctal>()
        │   │
        │   ├─→ Extract value bits
        │   │   │
        │   │   └─→ value = ArincData.Get<DataBits>()
        │   │
        │   └─→ Update UI
        │       │
        │       └─→ Receiver::getInstance(chanell)->setLabelData(labelid, rate, value)
        │           │
        │           ├─→ Find label in Equipment
        │           │
        │           ├─→ Update label ARINC data
        │           │   └─→ label->ArincData = ArincData
        │           │
        │           ├─→ Notify data model
        │           │   └─→ dataModel->layoutChanged()
        │           │
        │           ├─→ Update label timestamp (for idle cleanup)
        │           │
        │           └─→ Increment data rate counter
        │
        └─→ Periodic Tasks:
            ├─→ Idle Label Cleanup (every 1000ms)
            │   ├─→ Check timestamps
            │   ├─→ Remove labels not updated for > 2000ms
            │   └─→ Update UI
            │
            └─→ Data Rate Evaluation (every 10000ms)
                ├─→ Calculate frames/second for each label
                └─→ Update UI display
```

---

## Main Window (`mainwindow.h/cpp`)

```cpp
class MainWindow : public QMainWindow {
    Q_OBJECT
    
public:
    MainWindow();
    
protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    
private slots:
    // Transmitter slots
    void transmitter0();
    void transmitter1();
    
    // Receiver slots
    void receiver0();
    void receiver1();
    void receiver2();
    void receiver3();
    
    // Recorder slots
    void record();
    void playback();
    void importRecordFile();
    void exportRecordFile();
    
    // Setup slots
    void receiverSetup();
    void transmitterSetup();
    void arinc429Database();
    void serialBusSetup();
    
    // Help slots
    void about();
    
private:
    void createActions();
    void createMenus();
    
    // Menus
    QMenu *transmitterMenu;
    QMenu *receiverMenu;
    QMenu *recorderMenu;
    QMenu *graphMenu;
    QMenu *acquisitionMenu;
    QMenu *setupMenu;
    QMenu *helpMenu;
    
    // Actions
    QAction *transmitter0Act;
    QAction *transmitter1Act;
    QAction *receiver0Act;
    QAction *receiver1Act;
    QAction *receiver2Act;
    QAction *receiver3Act;
    QAction *recordAct;
    QAction *playbackAct;
    QAction *importRecordFileAct;
    QAction *exportRecordFileAct;
    QAction *arinc429DatabaseAct;
    QAction *receiverSetupAct;
    QAction *transmitterSetupAct;
    QAction *serialBusSetupAct;
    QAction *aboutAct;
    
    QLabel *infoLabel;
};
```

**Menu Structure:**
```
File
├─ Transmitter
│  ├─ Transmitter 0
│  └─ Transmitter 1
├─ Receiver
│  ├─ Receiver 0
│  ├─ Receiver 1
│  ├─ Receiver 2
│  └─ Receiver 3
├─ Recorder
│  ├─ Record
│  ├─ Playback
│  ├─ Import Record File
│  └─ Export Record File
├─ Acquisition
├─ Setup
��  ├─ ARINC 429 Database
│  ├─ Receiver Setup
│  ├─ Transmitter Setup
│  └─ Serial Bus Setup
└─ Help
   └─ About
```

---

## Data Models and UI

### TreeDataModel (`TreeDataModel.h/cpp`)

Provides tree structure for Equipment/Labels in QTreeView:

```cpp
class MyDataModel : public QAbstractItemModel {
    Q_OBJECT
    
public:
    MyDataModel(QObject *parent, PointerVector<BaseItem>& equipments, bool bIfTransmitter);
    
    // Qt model interface
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    
    // Label management
    void addLabel(str_t labelId);
    void removeRow(const QModelIndex &index);
    void setLabelData(str_t labelid, value_t value);
    
    // Data access
    std::vector<DArincData> getListOfAvailableLabelData();
    void cleanTimeoutList();
    void evalDataRates(uint32_t evalTime);
    
    // Idle cleanup
    void incrementLabelsDataRateCounter();
    void addReservedLabel();
};
```

**Tree Columns:**
- Column 0: Label/Parameter Name
- Column 1: Bit Position or Value
- Column 2: Engineering Value or Type
- Column 3: Scale or Units
- Column 4: Raw Bits
- Column 5: Data Rate

### Editor Delegate (`editordelegate.h/cpp`)

Provides custom editors for inline value editing:

```cpp
class EditorDelegate : public QStyledItemDelegate {
    Q_OBJECT
    
    EditorDelegate(QWidget *parent = nullptr);
    
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const override;
    
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                     const QModelIndex &index) const override;
};
```

---

## JSON Configuration Format

### Equipment Configuration Structure

Example `equipment.json`:
```json
{
    "002": [
        "310",
        "311", 
        "312",
        "320"
    ],
    "001": [
        "100",
        "101"
    ]
}
```

Example label file format (e.g., `310_002.json`):
```json
{
    "Id": "002",
    "Label": "310",
    "Type": "BNR",
    "Name": "Aircraft Position - Latitude",
    "Params": {
        "0": {
            "Name": "Latitude",
            "Type": "BNR",
            "IBit": 9,
            "FBit": 29,
            "Details": {
                "Units": "degrees",
                "Scale-": "-90.0",
                "Scale+": "+90.0",
                "SigBits": 19,
                "Resolution": "0.0001"
            }
        },
        "1": {
            "Name": "SSM",
            "Type": "DISCRETE",
            "IBit": 30,
            "FBit": 31,
            "Details": {
                "0": "Failure",
                "1": "Valid",
                "2": "Functional Test",
                "3": "Normal Operation"
            }
        }
    }
}
```

### Equipment Names Configuration

Example `equipment_names.json`:
```json
{
    "001": "VOR Receiver",
    "002": "INS (Inertial Navigation System)",
    "003": "Altimeter",
    "004": "Attitude Indicator"
}
```

---

## Threading Model

### Thread Architecture

```
Main Thread (GUI Event Loop)
├─ Handles user input
├─ Updates UI widgets
└─ Manages QAction signals

Transmitter Channel 0 - mainThread
├─ Transmitter UI widget
├─ Signal/slot handling
└─ Model data updates

Transmitter Channel 0 - dataRateThread
└─ Data rate calculation timer

Transmitter Channel 1 - mainThread
├─ Transmitter UI widget
└─ Parallel to Channel 0

Receiver Channel 0 - mainThread
├─ Receiver UI widget
└─ Signal/slot handling

Receiver Channel 0 - idlecleanerThread
└─ Label cleanup timer (every 1000ms)

Receiver Channel 0 - dataRateThread
└─ Data rate evaluation timer (every 10000ms)

Receiver Channels 1, 2, 3 - Similar structure

DEI1016 Serial Monitor
└─ Monitors serial port for incoming data
   └─ Signals ReceiverWorker instances on new data
```

### Thread Safety Mechanisms

**QMutex Usage:**
```cpp
QMutex modelMutex;

// Critical section
{
    QMutexLocker<QMutex> locker(&modelMutex);
    // Access shared data
    dataModel->setData(...);
}
```

**Qt Signal/Slot Connections:**
```cpp
// Queued connection (thread-safe, asynchronous)
connect(this, &DEI1016::update, 
        receiverWorker, &ReceiverWorker::update,
        Qt::QueuedConnection);

// Direct connection (synchronous, same thread)
connect(timer, &Timer::onTimeout,
        this, &ReceiverWorker::idleLabelCleaner,
        Qt::DirectConnection);

// Blocking queued (wait for slot to finish)
connect(serialPort, &QSerialPort::readyRead,
        this, &Handler::handleData,
        Qt::BlockingQueuedConnection);
```

**Data Synchronization Flags:**
```cpp
bool bIfDataUpdated;    // Producer set when data available
bool bIfDataHandled;    // Consumer clear when processing
bool bIfToClean;        // Cleanup flag

// Pattern
if (bIfDataHandled) {
    ArincData->UpdateData(newData);
    bIfDataUpdated = true;
    bIfDataHandled = false;
}

// Consumer
if (bIfDataUpdated) {
    processData(ArincData);
    bIfDataUpdated = false;
    bIfDataHandled = true;
}
```

---

## File Organization by Responsibility

### Core Data Structures
```
menus/
├── ArincData.h/cpp              # 32-bit ARINC word container
├── types.h                      # Type definitions and constants
├── arinc_functions.h/cpp        # Data encoding/decoding algorithms
│                                 # (BCD, BNR, DISCRETE, etc.)
├── bitutils.h/cpp               # Bit manipulation utilities
└── PointerVector.h              # Template container for smart ptr management
```

### Equipment and Label Management
```
menus/
├── Equipment.h/cpp              # Equipment collection (group of labels)
├── Label.h/cpp                  # Individual ARINC label
├── BaseParameter.h/cpp          # Abstract parameter base class
├── BCDParameter.h/cpp           # Binary Coded Decimal parameter
├── BNRParameter.h/cpp           # Binary Number Representation parameter
├── DiscreteParameter.h/cpp      # Discrete on/off parameter
├── FLNOParameter.h/cpp          # Flight Number parameter
├── ATCBCDParameter.h/cpp        # ATC Altitude parameter
├── FLOATBCDParameter.h/cpp      # Floating Point BCD parameter
├── INTParameter.h/cpp           # Integer parameter
└── baseitem.h/cpp               # Base class for tree items
```

### User Interface Components
```
menus/
├── mainwindow.h/cpp             # Main application window
├── transmitter.h/cpp            # Transmitter UI widget (1 per channel)
├── receiver.h/cpp               # Receiver UI widget (1 per channel)
├── basetransiverwidget.h/cpp    # Base class for transmitter/receiver UI
├── *.ui                         # Qt Designer UI files
│   ├── transmitter.ui
│   ├── receiver.ui
│   ├── checkboxarray.ui
│   ├── settingsdialog.ui
│   └── ...
├── TreeDataModel.h/cpp          # Equipment/label tree view model
├── editordelegate.h/cpp         # Custom inline editor for tree cells
├── checkboxarray.h/cpp          # Checkbox array widget
├── labelfor.h/cpp               # Label for parameters
└── basetransiverwidget.h/cpp    # Base widget for transceiver UIs
```

### Worker Threads
```
menus/
├── transmitterworker.h/cpp      # Transmission scheduling thread (1 per channel)
├── receiverworker.h/cpp         # Reception monitoring thread (1 per channel)
├── Timer.h/cpp                  # Timer utility for thread scheduling
└── Handler.h                    # Abstract handler interface for data processing
```

### Hardware Interface and Device Control
```
menus/
├── DEI1016.h/cpp                # DEI1016 ARINC interface card control
├── DEI1016RasberryConfigurations.h  # Raspberry Pi GPIO pin configuration
├── DataSourceBase.h/cpp         # Abstract data source interface
├── bitutils.h/cpp               # Bit-level I/O operations
└── tests/
    ├── test_isr.cpp             # GPIO interrupt testing
    └── build.sh                 # Compilation script
```

### Data Models and Viewers
```
menus/
├── TreeDataModel.h/cpp          # QAbstractItemModel for tree view
├── editordelegate.h/cpp         # QStyledItemDelegate for inline editing
├── generaldata.h/cpp            # Global application state
└── action.h/cpp                 # ARINC transmission action object
```

### Utilities and Helpers
```
menus/
├── utils.h/cpp                  # File I/O, JSON handling, dialogs
├── equipmentsids.h/cpp          # Equipment ID ↔ Name mapping
├── settingsdialog.h/cpp         # Serial port configuration dialog
└── json.h/cpp                   # JSON parsing (3rd party)
```

### Configuration Files
```
menus/
├── equipment.json               # Equipment definitions
├── equipment_names.json         # Equipment ID to name mapping
├── menus.pro                    # QMake project file
├── CMakeLists.txt               # CMake build configuration
└── icons.qrc                    # Qt resource file (icons)
```

### Resource Files
```
menus/
├── icons/
│   └── zafiro/                  # Icon theme directory
│       ├── README.md
│       ├── LICENSE.md
│       └── [icon files]
└── icons.qrc                    # Qt resource manifest
```

---

## Key Algorithms and Conversions

### BCD (Binary Coded Decimal) Encoding

```cpp
// BCD splits data into 4-bit chunks representing decimal digits
// Bit layout: [... D4D3D2D1 D0 ...]
// Each 4-bit group: 0000-1001 → 0-9

namespace ArincFunctions {
    // Bit positions for 5-digit BCD (from ARINC spec page 89)
    constexpr std::array<pair_t, 5> BCDSplit = {
        {27,29},     // Digit 4 (MSD)
        {23,26},     // Digit 3
        {19,22},     // Digit 2
        {15,18},     // Digit 1
        {11,14}      // Digit 0 (LSD)
    };
    
    template<>
    uint32_t BCD::Value<uint32_t>(const uint8_t& sigdigits, const DArincData& Data) {
        double t{ 0.0 };
        int N = sigdigits - 1;
        if (0 < sigdigits && sigdigits <= 5) {
            for (uint8_t x = 0; x <= N; x++) {
                // Extract 4 bits, interpret as decimal digit
                uint32_t digit = INT::Value<uint32_t>(
                    BCDSplit[x].first, 
                    BCDSplit[x].second, 
                    Data
                );
                t += digit * std::pow(10, N - x);
            }
        }
        return t;
    }
}
```

### BNR (Binary Number Representation)

```cpp
// Two's complement fractional representation
// Range: -2.0 to +1.999...
// Format: [S][sign bit][fraction bits]

template<>
double BNR::Value<double>(const uint8_t& bi, const uint8_t& bf, const DArincData& Data) {
    double t{ 0.0 };
    
    // Check sign bit (MSB)
    if (Data.GetBit(bf) == 0) {
        // Positive: sum fractional bits
        for (uint8_t f = bf - 1; f >= bi; f--) {
            t += std::pow(0.5, bf - f) * (double)Data.GetBit(f);
        }
    } else {
        // Negative: use two's complement
        for (uint8_t f = bf - 1; f >= bi; f--) {
            t += std::pow(0.5, bf - f) * (double)Data.GetBit(f);
        }
        t = 1.0 - t;  // Two's complement conversion
    }
    return t;
}

// Conversion to BNR
template<>
void BNR::SetValue<double>(const double& t, const uint8_t& bi, const uint8_t& bf, DArincData& Data) {
    double __t{0.f};
    double _t{std::abs(t)};
    
    // Convert to fractional binary
    for (int f = bf - 1; f >= bi; f--) {
        __t = 2 * _t;
        Data.SetBit(f, (uint8_t)__t);
        __t -= (uint8_t)__t;
        _t = __t;
    }
    
    // Handle negative (set sign bit and apply two's complement)
    if (t < 0.0) {
        TwosComplement(bi, Data);
        AddOneAt(bi, Data);
        Data.SetBit(bf, 1);  // Set sign bit
    }
}
```

### Discrete Values

```cpp
// Simple on/off or multi-state values
enum {
    Failure = 0,
    Valid = 1,
    FunctionalTest = 2,
    NormalOperation = 3
};

template<>
uint32_t BITS::Value<uint32_t>(const uint8_t& bi, const uint8_t& bf, const DArincData& Data) {
    uint32_t d{ 0 };
    for (uint8_t f = bi; f <= bf; f++) {
        d += Data.GetBit(f) * std::pow(2, f - bi);
    }
    return d;
}
```

---

## Development Guidelines

### Adding a New Parameter Type

**Step 1: Define in `arinc_functions.h`**
```cpp
namespace ArincFunctions {
    class MYNEWTYPE {
    public:
        template <typename T>
        static T Value(const uint8_t& bi, const uint8_t& bf, const DArincData& Data);
        
        template <typename T>
        static void SetValue(const T& t, const uint8_t& bi, const uint8_t& bf, DArincData& Data);
    };
}
```

**Step 2: Implement in `arinc_functions.cpp`**
```cpp
template<>
uint32_t ArincFunctions::MYNEWTYPE::Value<uint32_t>(
    const uint8_t& bi, const uint8_t& bf, const DArincData& Data) {
    // Conversion logic from bits to value
    uint32_t result = 0;
    // ...implementation...
    return result;
}

template<>
void ArincFunctions::MYNEWTYPE::SetValue<value_t>(
    const value_t& t, const uint8_t& bi, const uint8_t& bf, DArincData& Data) {
    // Conversion logic from value to bits
    auto bits = std::bitset<16>(t.toUInt());
    // ...set bits in Data...
}
```

**Step 3: Add to type enumeration in `types.h`**
```cpp
enum DArincParamType {
    BNR = 0,
    BCD,
    FLOATBCD,
    DISCRETE,
    FLNOBCD,
    ATCBCD,
    INT,
    MYNEWTYPE,      // Add here
    UNKOWN
};
```

**Step 4: Create Parameter class (`MyNewTypeParameter.h/cpp`)**
```cpp
#include "BaseParameter.h"

class MyNewTypeParameter : public BaseParameter {
public:
    MyNewTypeParameter(jsonobj_t json);
    
    value_t EngValue(DArincData& data) override;
    value_t BitsValue(DArincData& data) override;
    void setEngValue(value_t value, DArincData& data) override;
};
```

**Step 5: Register in `Label::addParams()` (`Label.cpp`)**
```cpp
void Label::addParams(jsonobj_t _jsonObj) {
    // ... existing code ...
    
    if (paramObj["Type"].toString() == "MYNEWTYPE") {
        auto param = new Param<DArincParamType::MYNEWTYPE>(paramObj);
        param->parent = this;
        Params.push_back(param);
    }
}
```

**Step 6: Add to `menus.pro` if new header/source**
```makefile
HEADERS += MyNewTypeParameter.h
SOURCES += MyNewTypeParameter.cpp
```

### Adding a New Equipment

**Step 1: Update `equipment.json`**
```json
{
    "010": [
        "400",
        "401",
        "402"
    ]
}
```

**Step 2: Update `equipment_names.json`**
```json
{
    "010": "VHF Radio"
}
```

**Step 3: Create label definition files**
- `400_010.json` - Label 400 for equipment 010
- `401_010.json` - Label 401 for equipment 010
- `402_010.json` - Label 402 for equipment 010

Each label file contains parameters and encoding information.

**Step 4: Application loads automatically**
The Equipment constructor automatically loads definitions:
```cpp
Equipment::Equipment(str_t _equipmentId, EquipmentRole _role) {
    path = GeneralData::getInstance()->LABELS_PATH;
    init(_equipmentId);  // Loads from JSON
}
```

### Extending the UI

**Step 1: Create UI in Qt Designer**
```bash
# Create new dialog or widget in Qt Designer
File → New → Qt Designer Form
```

**Step 2: Create wrapper class**
```cpp
// mywidget.h
#include <QWidget>

namespace Ui {
    class MyWidget;
}

class MyWidget : public QWidget {
    Q_OBJECT
    
public:
    explicit MyWidget(QWidget *parent = nullptr);
    ~MyWidget();
    
private:
    Ui::MyWidget *ui;
};

// mywidget.cpp
#include "mywidget.h"
#include "ui_mywidget.h"

MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MyWidget) {
    ui->setupUi(this);
}

MyWidget::~MyWidget() {
    delete ui;
}
```

**Step 3: Add to `menus.pro`**
```makefile
HEADERS += mywidget.h
SOURCES += mywidget.cpp
FORMS += mywidget.ui
```

**Step 4: Integrate into MainWindow**
```cpp
// mainwindow.cpp
void MainWindow::createActions() {
    myAction = new QAction(tr("&My Feature"), this);
    connect(myAction, &QAction::triggered, this, &MainWindow::mySlot);
}

void MainWindow::mySlot() {
    MyWidget *widget = new MyWidget(this);
    widget->show();
}
```

---

## Performance Considerations

### Real-Time Constraints

**Data Reception Latency:**
- Hardware interrupt → DEI1016 data received
- Serial port signal → DEI1016::dataReceived() slot
- ReceiverWorker::update() queued signal (Qt::QueuedConnection)
- ReceiverWorker::receiveTask() processes data
- **Target:** < 10ms end-to-end

**Data Transmission Latency:**
- User input in UI
- dataModel->setData() updates model
- transmitterworker::taskTransmitData() (every 2ms timer)
- Format ARINC word
- DEI1016 GPIO write
- Serial bus transmission
- **Target:** < 20ms

### Data Rate Limits

```cpp
#define HI_DATA_RATE    100000   // 100 kbps (clock/10)
#define LO_DATA_RATE     12500   // 12.5 kbps (clock/80)

// ARINC frame structure
// 32 bits per word
// Typical label update: 1-4 words per second (HI rate)
// Maximum: ~3125 labels at HI rate
```

### Optimization Techniques

**Compiler Optimization:**
```makefile
# From menus.pro
QMAKE_CXXFLAGS += -O3                    # Level 3 optimization
# -O3 enables: inline functions, loop unrolling, vectorization
```

**Memory Management:**
```cpp
// Use custom PointerVector for efficient allocation
PointerVector<BaseItem> labels;      // RAII container
labels.push_back(new Label(...));    // Automatic cleanup

// Avoid unnecessary copies
const std::bitset<ARINC32_SIZE>& data = arincData.getBitSet();
```

**Thread Pooling:**
```cpp
// Qt Concurrent for parallel execution
QFuture<void> future = QtConcurrent::map(labelList, [](Label* label) {
    label->updateDataRate();
});
```

**Lock Granularity:**
```cpp
// Keep critical sections small
{
    QMutexLocker<QMutex> locker(&modelMutex);
    // Only lock what's necessary
    dataModel->updateLabel(labelId, value);
    // Release lock immediately after
}
// Don't hold lock while doing I/O
```

---

## Debugging and Troubleshooting

### Enable Debug Output

**Qt Debugging:**
```cpp
// Add to mainwindow.cpp
QLoggingCategory::setFilterRules("*.debug=true");

// In code
qDebug() << "Label received:" << labelId << "Value:" << value;
qWarning() << "Warning: Parameter not found";
qCritical() << "Error: Device not responding";
```

**Environment Variables:**
```bash
# Enable Qt internal debug messages
export QT_DEBUG_PLUGINS=1
export QT_DEBUG_WINDOWS=1

# Run application with debugging
./menus -platform off
