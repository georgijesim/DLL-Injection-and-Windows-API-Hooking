# DLLInjectionProject

## Description
This project demonstrates DLL injection and API hooking in Windows. It includes three main components:
- `SimpleDLL` (TriggerDLL): A DLL that triggers a `MessageBoxW` call when injected into a process.
- `HookDLL`: A DLL that hooks the `MessageBoxW` function and modifies its behavior.
- `Injector`: An executable that injects the DLLs into a target process.

## How It Works
1. **SimpleDLL (TriggerDLL)**: This DLL, when injected into a process (e.g., Notepad), calls the `MessageBoxW` function to display a message box.
2. **HookDLL**: This DLL hooks the `MessageBoxW` function and modifies the message displayed in the message box.
3. **Injector**: This executable injects the `SimpleDLL` and `HookDLL` into the target process.

## Project Structure
- `SimpleDLL`: Contains the code for the SimpleDLL (TriggerDLL).
- `HookDLL`: Contains the code for the HookDLL.
- `Injector`: Contains the injector code to inject the DLLs into a target process.

## Building the Project

### Prerequisites
- Visual Studio
- Microsoft Detours library

### Steps
1. Clone the repository:
    ```sh
    git clone hhttps://github.com/georgijesim/DLL-Injection-and-Windows-API-Hooking.git
    cd DLLInjectionProject
    ```

2. Build `SimpleDLL` and `HookDLL` using Visual Studio:
    - Open each project (`SimpleDLL` and `HookDLL`) in Visual Studio.
    - Build the projects to generate the respective DLL files.

3. Build `Injector` using Visual Studio:
    - Open the `Injector` project in Visual Studio.
    - Build the project to generate the `injector.exe` file.
    - The `injector.exe` file will be located in the `Injector\x64\Debug` (or `Release`) directory after building.

## Running the Project
1. Start Notepad (or any target application).

2. Inject `HookDLL` into the target application:
    ```sh
    Injector\x64\Debug\injector.exe HookDLL\x64\Debug\HookDLL.dll notepad.exe
    ```

3. Inject `SimpleDLL` (TriggerDLL) into the target application:
    ```sh
    Injector\x64\Debug\injector.exe SimpleDLL\x64\Debug\SimpleDLL.dll notepad.exe
    ```

4. The modified message box should appear in Notepad.