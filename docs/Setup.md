# Setup and Build Guide for Newton's Fall

## Overview

This guide provides steps to set up and build the **Newton's Fall** project on Linux and macOS. It uses Conan for dependency management and CMake for project configuration and build. Ensure Python and CMake are installed before starting. For more details about Conan, visit the [Conan Documentation](https://docs.conan.io/2/reference/conanfile.html).

---

## Setup Instructions

### Step 1: Clone the Repository
Clone the project repository to your local machine:
```bash
git clone https://github.com/unibuc-cs/software-engineering-product-apples.git
```
```bash
cd software-engineering-product-apples
```

### Step 2: Set Up a Virtual Environment
It is recommended to use a virtual environment to isolate dependencies:
```bash
python3 -m venv .venv
```
```bash
source .venv/bin/activate
```

### Step 3: Install dependencies
Install project dependencies:
```bash
pip3 install -r requirements.txt
```

### Step 4: Create Conan Profile and Install Dependencies
Set up the Conan default profile and fetch the necessary dependencies:
```bash
conan profile detect --force
```
```bash
conan install . --build=missing -c tools.system.package_manager:mode=install -c tools.system.package_manager:sudo=True
```

### Step 5: Configure and Build the Project
Configure and build the project using Conan:
```bash
conan build .
```

Configure Debug build:
```bash
conan install . -s build_type=Debug 
```

### Step 6: Run the Project
Execute the project binary:
```bash
./build/Release/bin/newtons_fall
```

### Step 7: Setup database
Execute in a new terminal
Install node 20
For Linux
```bash
curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.39.1/install.sh | bash
nvm install 20
```
For MacOS
```bash
brew install nvm
nvm install 20
```
```bash
cd software-engineering-product-apples/backend
```
```bash
npm install
```
```bash
./refresh.sh
```
```bash
npm run start
```
For testing the database open a new terminal in the backend folder and run
```bash
npm run test
```

### Step 8: Run the CLI for the game
Install golang
For Linux
```bash
sudo apt install golang-go
```
For MacOS
```bash
brew install go
```
Execute in a new terminal
```bash
cd software-engineering-product-apples/client
```
create an .env file in this folder with the structure: 
```.env
BACKEND=database_url
```
```bash
go run main.go
```