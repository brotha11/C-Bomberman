# C-Bomberman
**A Bomberman game made in C**

This is an open-source Bomberman game I'm developing to learn more about the C programming language and SDL2, and also for fun, of course.

---

### Compiling

This project uses *gcc* to compile the project, make sure it's installed in your machine in order to use the provided tools.

#### Using Make (Linux)
This program can be compiled using the provided Makefile.

1. **Install SDL2 (on Ubuntu/Debian):**  
If you're using a Linux distribution such as Ubuntu or Debian, make sure you have SDL2 installed. You can do this by running the following commands:

    ~~~
    sudo apt-get update  
    sudo apt-get install libsdl2-dev
    ~~~

2. **Compiling the game:**  
Once SDL2 is installed, run the following command to compile the program using the included `Makefile`:

    ~~~
    make
    ~~~

   This will build the project, and you'll be able to run the game.

#### Using .bat file (Windows)

On your Windows machine, you can use the provided **build.bat** file to compile the project.

1. **Check files:**
For Windows builds, make sure that the `include`, `lib` folders, and the dll files: *SDL2.dll*, *SDL2_image.dll* and *SDL2_mixer.dll* are present in the root folder.

2. **Compile the game:**
You can put the following command in a terminal to execute the .bat file:

    ~~~
    ./build.bat
    ~~~  
    This will build the project for your Windows machine.

Aditionally, there is an option present in the .bat file to run the game after a succesfull compilation. 
This option is **disabled** by default. To enable it, simply change the following line in `build.bat`:

  ~~~
  :: Execute game after compiling
  set RUN_AFTER_COMPILE=true
  ~~~


---

### Running the game

After compiling, you can run the game with:

1. **Makefile**
    ~~~
    make run
    ~~~
2. **Terminal**
    ~~~
    ./C-Bomberman
    ~~~

Make sure you have the necessary assets (sprites, sounds, etc.) in the `res` folder.

---

### Credits

This project is purely for educational and personal purposes, and **is not meant to be sold**. All characters, graphics, sounds, and other protected materials are owned by their respective creators. I do not own any of these trademarks.

#### Sprites
- **The Sprites Resource (Rippers):**
  - Gors
  - T_M

- **The Sounds Resource (Rippers):**
  - SteamyJ
  - Phongpon
